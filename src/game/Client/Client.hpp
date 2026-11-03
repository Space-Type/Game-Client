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
#include <thread>


namespace RType::Game {
    class Client {
        static constexpr std::string_view DEFAULT_SERVER_HOST = "127.0.0.1";
        static constexpr std::uint16_t    DEFAULT_SERVER_PORT = 5432;

        Protecs::Networking::UDPSocket m_Client;
        Protecs::Networking::Address m_ServerAddr;
        Protecs::NetworkEntity m_PlayerId;

        Utils::ThreadQueue<Protecs::Networking::Packets::PacketData> m_OutgoingPackets;
        Utils::ThreadQueue<Protecs::Networking::Packets::PacketData> m_IncomingPackets;
        std::thread m_NetworkThread;

        public:
            Client(const Client& cpy) = delete;
            Client(Client&& other);
            Client();
            ~Client();

            [[ nodiscard ]] constexpr bool isConnected() const noexcept { return m_Client.isOpen() && m_PlayerId != 0; }
            [[ nodiscard ]] Protecs::NetworkEntity player() const;

            void connect(std::string_view name);
            void disconnect();

            void send(Protecs::Networking::Packets::PacketData&& data);
            std::optional<Protecs::Networking::Packets::PacketData> receive();

            void swap(Client& other) noexcept;
            inline Client& operator=(Client&& other) noexcept { swap(other); return *this; }

        private:
            void run();
            void update();
    };
}
