/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Client class
*/

#pragma once

#include <protecs/networking.hpp>
#include <rtype/utils.hpp>
#include <chrono>
#include <cstddef>
#include <optional>
#include <string_view>
#include <thread>


namespace RType::Game {
    class Client {
        // UDP may lose the join request
        static constexpr std::chrono::milliseconds JOIN_RESEND_INTERVAL{1000};

        // A flood of datagrams must not freeze the frame: the next poll reads the rest
        static constexpr std::size_t MAX_DATAGRAMS_PER_POLL = 256;

        struct ConnectAttempt {
            Protecs::Networking::Packet joinRequest;
            std::chrono::steady_clock::time_point deadline;
            std::chrono::steady_clock::time_point nextJoin;
        };

        Protecs::Networking::UDPSocket m_Client;
        Protecs::Networking::Address m_ServerAddr;
        Protecs::NetworkEntity m_PlayerId;
        std::optional<ConnectAttempt> m_Attempt;

        Utils::ThreadQueue<Protecs::Networking::Packets::PacketData> m_OutgoingPackets;
        Utils::ThreadQueue<Protecs::Networking::Packets::PacketData> m_IncomingPackets;
        std::thread m_NetworkThread;

        public:
            static constexpr std::chrono::milliseconds CONNECT_TIMEOUT{5000};

            Client(const Client& cpy) = delete;
            Client(Client&& other);
            Client();
            ~Client();

            [[ nodiscard ]] constexpr bool isConnected() const noexcept { return m_Client.isOpen() && m_PlayerId != 0; }
            [[ nodiscard ]] Protecs::NetworkEntity player() const;

            /**
             * Starts joining `server` as `name`, dropping
             * any previous connection or attempt. The
             * attempt then progresses through `pollConnect()`.
             */
            void beginConnect(const Protecs::Networking::Address& server, std::string_view name,
                              std::chrono::milliseconds timeout = CONNECT_TIMEOUT);

            /**
             * Never blocks. Returns true once connected.
             * Throws Errors::NetworkError if the attempt failed
             * (timeout, refused by the server, closed socket)
             * or if there is none.
             */
            bool pollConnect();

            /**
             * Tells the server the player leaves, connected or
             * still connecting: it may already have registered them.
             */
            void disconnect();

            void send(Protecs::Networking::Packets::PacketData&& data);
            std::optional<Protecs::Networking::Packets::PacketData> receive();

            void swap(Client& other) noexcept;
            inline Client& operator=(Client&& other) noexcept { swap(other); return *this; }

        private:
            std::optional<Protecs::NetworkEntity> readJoinAnswer();
            void abandonAttempt();

            void run();
            void update();
    };
}
