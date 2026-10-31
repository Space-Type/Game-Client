/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Client class
*/

#include "Client.hpp"
#include <rtype/errors.hpp>
#include <iostream>


using Protecs::Networking::Packet;
namespace Packets = Protecs::Networking::Packets;


RType::Game::Client::Client(Client&& other)
    : m_Client()
    , m_ServerAddr()
    , m_PlayerId(0)
    , m_NetworkThread()
{
    swap(other);
}

RType::Game::Client::Client()
    : m_Client("127.0.0.1", 0)
    , m_ServerAddr(Protecs::Networking::Address::construct(DEFAULT_SERVER_HOST, DEFAULT_SERVER_PORT))
    , m_PlayerId(0)
    , m_NetworkThread()
{
}

RType::Game::Client::~Client()
{
    disconnect();
}

auto RType::Game::Client::player() const -> Protecs::NetworkEntity
{
    if (!isConnected())
        throw Errors::NetworkError("attempted to retrieve player ID without being connected");

    return m_PlayerId;
}

void RType::Game::Client::connect(std::string_view name)
{
    auto joinRequest = Packet::serialize(Packets::PlayerJoin::fromStr(name));
    if (!m_Client.send(joinRequest, m_ServerAddr))
        throw Errors::NetworkError("failed to send join request to server");

    // TODO: ensure data is from server, not someone else

    Packet response;
    m_Client.receive(response);
    auto data = response.deserialize();

    // TODO: allow packets to come through in a different order
    if (!std::holds_alternative<Packets::PlayerId>(data))
        throw Errors::NetworkError("server replied with unexpected packet");

    auto& playerId = std::get<Packets::PlayerId>(data);
    m_PlayerId = playerId.id;

    m_NetworkThread = std::thread([&](){ run(); });
}

void RType::Game::Client::disconnect()
{
    if (!isConnected())
        return;

    bool success = m_Client.send(Packet::serialize(Packets::PlayerLeave()), m_ServerAddr);
    m_PlayerId = 0;
    m_NetworkThread.join();

    if (!success)
        throw Errors::NetworkError("failed to disconnect from server");
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
