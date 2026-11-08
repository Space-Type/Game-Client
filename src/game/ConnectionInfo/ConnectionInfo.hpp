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
#include <string_view>


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

    inline constexpr std::string_view USAGE =
        "USAGE: r-type_client [options]\n"
        "\n"
        "Opens the R-Type client. The connection screen is filled\n"
        "with these values, which can still be changed there.\n"
        "\n"
        "Options:\n"
        "  -h <host>    server IP address (default: 127.0.0.1)\n"
        "  -p <port>    server port (default: 5432)\n"
        "  -n <name>    nickname (default: Player)\n"
        "  --help       display this help and exit\n";

    // True if one of the arguments is --help (-h is the host)
    bool wantsHelp(int argc, const char* const* argv) noexcept;

    /**
     * Reads -h <host>, -p <port> and -n <name>. Values are
     * only checked by `ConnectionInfo::validate()`. Throws
     * Errors::Error on an unknown option or a missing value.
     */
    ConnectionInfo parseArguments(int argc, const char* const* argv);
}
