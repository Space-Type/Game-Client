/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** ConnectionInfo struct
*/

#pragma once

#include <protecs/networking.hpp>
#include <cstddef>
#include <optional>
#include <string>


namespace RType::Game {
    /**
     * Where to connect and as whom, as typed by the user:
     * kept as text so that a bad value can be shown and fixed.
     */
    struct ConnectionInfo {
        static constexpr std::size_t HOST_MAX_LENGTH = 15;  // "255.255.255.255"
        static constexpr std::size_t PORT_MAX_LENGTH = 5;   // "65535"
        static constexpr std::size_t NAME_MAX_LENGTH = 32;  // Packets::PlayerJoin

        std::string host = "127.0.0.1";
        std::string port = "5432";
        std::string name = "Player";

        // Returns what is wrong in a message for the user, nothing if all is valid
        [[ nodiscard ]] std::optional<std::string> validate() const;

        // Throws Errors::NetworkError if the host or the port is invalid
        [[ nodiscard ]] Protecs::Networking::Address address() const;
    };

    /**
     * Reads -h <host>, -p <port> and -n <name>. Values are
     * only checked by `ConnectionInfo::validate()`. Throws
     * Errors::Error on an unknown option or a missing value.
     */
    ConnectionInfo parseArguments(int argc, const char* const* argv);
}
