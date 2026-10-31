/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** Client class
*/

#include <gtest/gtest.h>
#include <rtype/errors.hpp>
#include <rtype/game.hpp>
#include <chrono>
#include <cstdint>
#include <optional>
#include <random>
#include <stdexcept>
#include <thread>
#include <variant>

namespace {

    using namespace std::chrono_literals;
    using Protecs::Networking::Address;
    using Protecs::Networking::Packet;
    using Protecs::Networking::UDPSocket;
    namespace Packets = Protecs::Networking::Packets;
    using RType::Game::Client;

    constexpr std::chrono::milliseconds LONG_TIMEOUT = 2s;
    constexpr int RECEIVE_TIMEOUT_MS = 2000;
    constexpr Protecs::NetworkEntity PLAYER_ID = 42;

    // UDPSocket cannot tell which port the system gave it: try ports of the ephemeral range
    UDPSocket bindLoopback()
    {
        std::random_device random;
        std::uniform_int_distribution<int> ports(49152, 65535);

        for (int attempt = 0; attempt < 64; ++attempt) {
            try {
                return UDPSocket("127.0.0.1", static_cast<std::uint16_t>(ports(random)));
            } catch (const std::exception&) {
                continue;  // port taken
            }
        }
        throw std::runtime_error("no free port on the loopback interface");
    }

    /** Plays the server's part on the loopback interface, driven by each test */
    class ClientTest : public testing::Test {
        protected:
            UDPSocket m_Server = bindLoopback();
            Address m_ClientAddress = {};
            Client m_Client;

            [[ nodiscard ]] Address serverAddress() const { return m_Server.address(); }

            // Next datagram received by the server, nothing if none comes in time
            std::optional<Packets::PacketData> receive(int timeoutMs = RECEIVE_TIMEOUT_MS)
            {
                if (!m_Server.pollReceive(timeoutMs))
                    return std::nullopt;

                Packet packet;

                m_Server.receive(packet, &m_ClientAddress);
                return packet.deserialize();
            }

            // Starts an attempt, and waits for its first join request
            void begin(std::chrono::milliseconds timeout = LONG_TIMEOUT)
            {
                m_Client.beginConnect(serverAddress(), "Bob", timeout);

                auto request = receive();

                ASSERT_TRUE(request.has_value());
                ASSERT_TRUE(std::holds_alternative<Packets::PlayerJoin>(*request));
                ASSERT_EQ(std::get<Packets::PlayerJoin>(*request).str(), "Bob");
            }

            void reply(const Packets::PacketData& data)
            {
                m_Server.send(Packet::serialize(data), m_ClientAddress);
            }

            // Polls until connected: datagrams take a moment even on the loopback interface
            bool pollFor(std::chrono::milliseconds duration)
            {
                auto end = std::chrono::steady_clock::now() + duration;

                while (std::chrono::steady_clock::now() < end) {
                    if (m_Client.pollConnect())
                        return true;
                    std::this_thread::sleep_for(1ms);
                }
                return false;
            }

            // Skips the join retries still in flight: what follows must be the goodbye
            void expectLeaveAmongNextPackets()
            {
                auto packet = receive();

                while (packet && (std::holds_alternative<Packets::PlayerJoin>(*packet) || std::holds_alternative<Packets::PlayerId>(*packet)))
                    packet = receive();
                ASSERT_TRUE(packet.has_value());
                EXPECT_TRUE(std::holds_alternative<Packets::PlayerLeave>(*packet));
            }

            /**
             * The network thread only sees the disconnection when its
             * poll ends (up to a second): a late packet ends it sooner.
             */
            void disconnect()
            {
                std::jthread waker([this](){
                    std::this_thread::sleep_for(20ms);
                    reply(Packets::Ping{});
                });

                m_Client.disconnect();
            }
    };
}


TEST_F(ClientTest, notConnectedAtFirst)
{
    EXPECT_FALSE(m_Client.isConnected());
    EXPECT_THROW((void)m_Client.player(), RType::Errors::NetworkError);
}

TEST_F(ClientTest, pollWithoutAttemptThrows)
{
    EXPECT_THROW(m_Client.pollConnect(), RType::Errors::NetworkError);
}

TEST_F(ClientTest, connects)
{
    begin();
    EXPECT_FALSE(m_Client.pollConnect());
    EXPECT_FALSE(m_Client.isConnected());

    reply(Packets::PlayerId{ PLAYER_ID });

    EXPECT_TRUE(pollFor(LONG_TIMEOUT));
    EXPECT_TRUE(m_Client.isConnected());
    EXPECT_EQ(m_Client.player(), PLAYER_ID);
    EXPECT_TRUE(m_Client.pollConnect());

    disconnect();
    EXPECT_FALSE(m_Client.isConnected());
    expectLeaveAmongNextPackets();
}

TEST_F(ClientTest, pollDoesNotBlock)
{
    begin();

    auto start = std::chrono::steady_clock::now();

    EXPECT_FALSE(m_Client.pollConnect());
    EXPECT_LT(std::chrono::steady_clock::now() - start, 100ms);
}

TEST_F(ClientTest, resendsJoinRequest)
{
    begin();

    auto start = std::chrono::steady_clock::now();
    std::optional<Packets::PacketData> resent;

    while (!resent && std::chrono::steady_clock::now() - start < LONG_TIMEOUT) {
        m_Client.pollConnect();
        resent = receive(1);
    }

    ASSERT_TRUE(resent.has_value());
    EXPECT_TRUE(std::holds_alternative<Packets::PlayerJoin>(*resent));
    EXPECT_GE(std::chrono::steady_clock::now() - start, 900ms);

    auto askedId = receive();

    ASSERT_TRUE(askedId.has_value());
    EXPECT_TRUE(std::holds_alternative<Packets::PlayerId>(*askedId));
}

// A server that registered the player and lost its answer only replies to the retry's PlayerId
TEST_F(ClientTest, recoversFromLostPlayerId)
{
    begin();

    auto start = std::chrono::steady_clock::now();

    while (!m_Client.pollConnect() && std::chrono::steady_clock::now() - start < LONG_TIMEOUT) {
        auto packet = receive(1);

        if (packet && std::holds_alternative<Packets::PlayerId>(*packet))
            reply(Packets::PlayerId{ PLAYER_ID });
    }

    ASSERT_TRUE(m_Client.isConnected());
    EXPECT_EQ(m_Client.player(), PLAYER_ID);

    disconnect();
}

TEST_F(ClientTest, refusesPlayerIdZero)
{
    begin();
    reply(Packets::PlayerId{ 0 });

    EXPECT_THROW(pollFor(LONG_TIMEOUT), RType::Errors::NetworkError);
    EXPECT_FALSE(m_Client.isConnected());
    expectLeaveAmongNextPackets();
}

TEST_F(ClientTest, ignoresOtherSenders)
{
    begin();

    UDPSocket stranger = bindLoopback();

    stranger.send(Packet::serialize(Packets::PlayerId{ 666 }), m_ClientAddress);
    EXPECT_FALSE(pollFor(50ms));

    reply(Packets::PlayerId{ PLAYER_ID });
    EXPECT_TRUE(pollFor(LONG_TIMEOUT));
    EXPECT_EQ(m_Client.player(), PLAYER_ID);

    disconnect();
}

TEST_F(ClientTest, ignoresInvalidPackets)
{
    begin();

    Packet unknownType = {};
    unknownType.raw[0] = 0xFF;
    unknownType.filled = 1;

    Packet truncatedPlayerId = Packet::serialize(Packets::PlayerId{ 666 });
    truncatedPlayerId.filled = 2;

    Packet empty = {};
    empty.filled = 0;

    m_Server.send(unknownType, m_ClientAddress);
    m_Server.send(truncatedPlayerId, m_ClientAddress);
    m_Server.send(empty, m_ClientAddress);
    EXPECT_FALSE(pollFor(50ms));

    reply(Packets::PlayerId{ PLAYER_ID });
    EXPECT_TRUE(pollFor(LONG_TIMEOUT));
    EXPECT_EQ(m_Client.player(), PLAYER_ID);

    disconnect();
}

TEST_F(ClientTest, ignoresOtherPacketTypes)
{
    begin();
    reply(Packets::Ping{});
    EXPECT_FALSE(pollFor(50ms));

    reply(Packets::PlayerId{ PLAYER_ID });
    EXPECT_TRUE(pollFor(LONG_TIMEOUT));

    disconnect();
}

TEST_F(ClientTest, readsAllAvailableDatagramsInOnePoll)
{
    begin();
    reply(Packets::Ping{});
    reply(Packets::Ping{});
    reply(Packets::PlayerId{ PLAYER_ID });
    std::this_thread::sleep_for(50ms);

    EXPECT_TRUE(m_Client.pollConnect());

    disconnect();
}

TEST_F(ClientTest, timesOut)
{
    begin(50ms);

    EXPECT_THROW(pollFor(LONG_TIMEOUT), RType::Errors::NetworkError);
    EXPECT_FALSE(m_Client.isConnected());
    expectLeaveAmongNextPackets();

    // The attempt is over
    EXPECT_THROW(m_Client.pollConnect(), RType::Errors::NetworkError);
}

TEST_F(ClientTest, cancellingTellsTheServer)
{
    begin();

    m_Client.disconnect();
    expectLeaveAmongNextPackets();
    EXPECT_THROW(m_Client.pollConnect(), RType::Errors::NetworkError);
}

TEST_F(ClientTest, destructionDuringAttemptTellsTheServer)
{
    {
        Client client;

        client.beginConnect(serverAddress(), "Alice");
        ASSERT_TRUE(receive().has_value());
    }
    expectLeaveAmongNextPackets();
}

TEST_F(ClientTest, disconnectWithoutAttemptSendsNothing)
{
    m_Client.disconnect();
    EXPECT_FALSE(m_Server.pollReceive(20));
}

TEST_F(ClientTest, canTryAgainAfterTimeout)
{
    begin(10ms);
    EXPECT_THROW(pollFor(LONG_TIMEOUT), RType::Errors::NetworkError);
    expectLeaveAmongNextPackets();

    begin();
    reply(Packets::PlayerId{ PLAYER_ID });
    EXPECT_TRUE(pollFor(LONG_TIMEOUT));

    disconnect();
}

TEST_F(ClientTest, newAttemptDropsPreviousOne)
{
    begin();
    m_Client.beginConnect(serverAddress(), "Carol");
    expectLeaveAmongNextPackets();
}

// Issue #50 and Packets::PlayerJoin: a refusal is a PlayerLeave, it must not wait for the timeout
TEST_F(ClientTest, playerLeaveRefusesTheAttempt)
{
    begin(500ms);
    reply(Packets::PlayerLeave{});
    std::this_thread::sleep_for(50ms);

    EXPECT_THROW(m_Client.pollConnect(), RType::Errors::NetworkError);
}
