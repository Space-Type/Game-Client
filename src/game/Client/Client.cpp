/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Client class
*/

#include "Client.hpp"
#include <rtype/errors.hpp>
#include <exception>
#include <iostream>
#include <variant>


using Protecs::Networking::Packet;
namespace Packets = Protecs::Networking::Packets;


RType::Game::Client::Client(Client&& other)
    : m_Client()
    , m_ServerAddr()
    , m_PlayerId(0)
    , m_Attempt()
    , m_NetworkThread()
{
    swap(other);
}

RType::Game::Client::Client()
    : m_Client("0.0.0.0", 0)  // every interface: the server may be on another machine
    , m_ServerAddr()
    , m_PlayerId(0)
    , m_Attempt()
    , m_NetworkThread()
{
}

RType::Game::Client::~Client()
{
    // The network thread is joined either way, only the goodbye may have failed
    try {
        disconnect();
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
    }
}

auto RType::Game::Client::player() const -> Protecs::NetworkEntity
{
    if (!isConnected())
        throw Errors::NetworkError("attempted to retrieve player ID without being connected");

    return m_PlayerId;
}

void RType::Game::Client::beginConnect(const Protecs::Networking::Address& server, std::string_view name,
                                       std::chrono::milliseconds timeout)
{
    disconnect();

    m_ServerAddr = server;

    auto now = std::chrono::steady_clock::now();
    auto joinRequest = Packet::serialize(Packets::PlayerJoin::fromStr(name));

    if (!m_Client.send(joinRequest, m_ServerAddr))
        throw Errors::NetworkError("failed to send join request to server");

    m_Attempt = ConnectAttempt{
        .joinRequest = joinRequest,
        .deadline = now + timeout,
        .nextJoin = now + JOIN_RESEND_INTERVAL,
    };
}

bool RType::Game::Client::pollConnect()
{
    if (isConnected())
        return true;
    if (!m_Attempt)
        throw Errors::NetworkError("no connection attempt in progress");

    for (std::size_t i = 0; i < MAX_DATAGRAMS_PER_POLL && m_Client.pollReceive(0); ++i) {
        std::optional<Protecs::NetworkEntity> playerId = readJoinAnswer();

        if (!playerId)
            continue;

        if (*playerId == 0) {
            abandonAttempt();
            throw Errors::NetworkError("the server refused the connection");
        }

        m_PlayerId = *playerId;
        m_Attempt.reset();
        m_NetworkThread = std::thread([this](){ run(); });
        return true;
    }

    // A failed socket operation closes the socket
    if (!m_Client.isOpen()) {
        m_Attempt.reset();
        throw Errors::NetworkError("lost the network socket");
    }

    auto now = std::chrono::steady_clock::now();

    if (now >= m_Attempt->deadline) {
        abandonAttempt();
        throw Errors::NetworkError("no answer from the server");
    }

    if (now >= m_Attempt->nextJoin) {
        m_Attempt->nextJoin = now + JOIN_RESEND_INTERVAL;

        // PlayerJoin in case the request was lost. PlayerId in case the answer was:
        // the server ignores a second join from a registered player, but sends the
        // id again when asked with a PlayerId (dropped if the sender is unknown)
        bool sent = m_Client.send(m_Attempt->joinRequest, m_ServerAddr)
            && m_Client.send(Packet::serialize(Packets::PlayerId{ 0 }), m_ServerAddr);

        if (!sent) {
            m_Attempt.reset();
            throw Errors::NetworkError("failed to send join request to server");
        }
    }
    return false;
}

void RType::Game::Client::disconnect()
{
    bool wasActive = m_Attempt.has_value() || isConnected();
    bool success = !wasActive || m_Client.send(Packet::serialize(Packets::PlayerLeave()), m_ServerAddr);

    m_Attempt.reset();
    m_PlayerId = 0;

    // Even if no longer connected: a closed socket also ends the thread
    if (m_NetworkThread.joinable())
        m_NetworkThread.join();

    if (!success)
        throw Errors::NetworkError("failed to disconnect from server");
}

/**
 * Reads one datagram. Returns the player's id if it is
 * a PlayerId from the server, 0 if the server refuses
 * (PlayerLeave or id 0), nothing for anything else.
 */
auto RType::Game::Client::readJoinAnswer() -> std::optional<Protecs::NetworkEntity>
{
    Packet packet;
    Protecs::Networking::Address sender = {};

    if (m_Client.receive(packet, &sender) == 0 || sender != m_ServerAddr)
        return std::nullopt;

    try {
        auto data = packet.deserialize();

        if (auto* playerId = std::get_if<Packets::PlayerId>(&data))
            return playerId->id;
        if (std::holds_alternative<Packets::PlayerLeave>(data))
            return 0;
    } catch (const std::exception&) {
        // Not a valid packet: the real answer may still come
    }
    return std::nullopt;
}

/**
 * The server may have registered the player even if
 * its answer never arrived, which would leave a ghost.
 * Best effort: the attempt failed anyway.
 */
void RType::Game::Client::abandonAttempt()
{
    m_Attempt.reset();
    m_Client.send(Packet::serialize(Packets::PlayerLeave()), m_ServerAddr);
}

void RType::Game::Client::send(Packets::PacketData&& data)
{
    m_OutgoingPackets.push(std::move(data));
}

auto RType::Game::Client::receive() -> std::optional<Packets::PacketData>
{
    return m_IncomingPackets.pop();
}

void RType::Game::Client::swap(Client& other) noexcept
{
    std::swap(m_Client, other.m_Client);
    std::swap(m_ServerAddr, other.m_ServerAddr);
    std::swap(m_PlayerId, other.m_PlayerId);
    std::swap(m_Attempt, other.m_Attempt);
    std::swap(m_OutgoingPackets, other.m_OutgoingPackets);
    std::swap(m_IncomingPackets, other.m_IncomingPackets);
    std::swap(m_NetworkThread, other.m_NetworkThread);
}

void RType::Game::Client::run()
{
    while (isConnected()) {
        try {
            update();
        } catch (const Errors::Error& err) {
            std::cerr << err.what() << std::endl;
        } catch (const std::exception& err) {
            std::cerr << err.what() << std::endl;
        } catch (...) {
            std::cerr << "unknown error" << std::endl;
        }
    }
}

void RType::Game::Client::update()
{
    auto packetToSend = m_OutgoingPackets.pop();
    while (packetToSend) {
        auto packet = Packet::serialize(*packetToSend);
        m_Client.send(packet, m_ServerAddr);
        packetToSend = m_OutgoingPackets.pop();
    }

    if (!m_Client.pollReceive(1000))
        return;

    // TODO: ensure packet is from server, not from John Mckligzgy
    Packet packet;
    m_Client.receive(packet);

    Packets::PacketData data;
    try {
        data = packet.deserialize();
    } catch (const std::exception& err) {
        throw Errors::NetworkError("failed to deserialize server data", err);
    }
    m_IncomingPackets.push(std::move(data));
}
