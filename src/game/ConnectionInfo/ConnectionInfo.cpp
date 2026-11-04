/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** ConnectionInfo struct
*/

#include "ConnectionInfo.hpp"
#include <rtype/errors.hpp>
#include <algorithm>
#include <charconv>
#include <cstdint>
#include <exception>
#include <string_view>


namespace {
    constexpr std::string_view LOCALHOST = "localhost";
    constexpr std::string_view LOOPBACK = "127.0.0.1";
    constexpr std::string_view UNSPECIFIED = "0.0.0.0";

    std::optional<std::uint16_t> parsePort(std::string_view text) noexcept
    {
        std::uint16_t port = 0;
        const char* end = text.data() + text.size();
        auto [last, error] = std::from_chars(text.data(), end, port);

        if (error != std::errc{} || last != end || port == 0)
            return std::nullopt;
        return port;
    }

    // 0 to 255 in decimal. No leading zero: some platforms would read it as octal
    bool isQuadPart(std::string_view part) noexcept
    {
        std::uint8_t value = 0;
        const char* end = part.data() + part.size();
        auto [last, error] = std::from_chars(part.data(), end, value);

        return error == std::errc{} && last == end && (part.size() == 1 || part.front() != '0');
    }

    bool isDottedQuad(std::string_view host) noexcept
    {
        for (int parts = 1; parts <= 4; ++parts) {
            std::size_t dot = host.find('.');

            if (!isQuadPart(host.substr(0, dot)))
                return false;
            if (dot == std::string_view::npos)
                return parts == 4;
            host.remove_prefix(dot + 1);
        }
        return false;
    }

    bool isPrintableAscii(char character) noexcept
    {
        return character >= ' ' && character <= '~';
    }

    /**
     * Stricter than Address::construct, whose system functions
     * accept other forms depending on the platform ("127.1",
     * hexadecimal, what follows a whitespace...). It stays the
     * final conversion, and reads `host` as a C string.
     */
    Protecs::Networking::Address buildAddress(const std::string& host, std::uint16_t port)
    {
        if (host != LOCALHOST && (!isDottedQuad(host) || host == UNSPECIFIED))
            throw RType::Errors::NetworkError("invalid host: \"" + host + "\"");
        return Protecs::Networking::Address::construct(host == LOCALHOST ? std::string(LOOPBACK) : host, port);
    }

    bool isOption(std::string_view argument) noexcept
    {
        return argument == "-h" || argument == "-p" || argument == "-n";
    }
}


auto RType::Game::ConnectionInfo::validate() const -> std::optional<std::string>
{
    if (host.empty())
        return "Server IP is empty";

    // Answers would come from the real address, and be ignored until the timeout
    if (host == UNSPECIFIED)
        return "Server IP cannot be 0.0.0.0: use the address of the server";

    try {
        (void)buildAddress(host, 1);
    } catch (const std::exception&) {
        return "Server IP must be an IPv4 address or localhost";
    }

    if (!parsePort(port))
        return "Port must be a number from 1 to 65535";

    if (name.empty() || name.size() > NAME_MAX_LENGTH)
        return "Nickname must be 1 to " + std::to_string(NAME_MAX_LENGTH) + " characters long";

    if (!std::ranges::all_of(name, isPrintableAscii))
        return "Nickname must only use unaccented letters, digits and symbols";

    if (std::ranges::all_of(name, [](char character){ return character == ' '; }))
        return "Nickname cannot be only spaces";

    return std::nullopt;
}

auto RType::Game::ConnectionInfo::address() const -> Protecs::Networking::Address
{
    std::optional<std::uint16_t> parsedPort = parsePort(port);

    if (!parsedPort)
        throw Errors::NetworkError("invalid port: \"" + port + "\"");

    try {
        return buildAddress(host, *parsedPort);
    } catch (const Errors::Error&) {
        throw;
    } catch (const std::exception& err) {
        throw Errors::NetworkError("invalid host: \"" + host + "\"", err);
    }
}

auto RType::Game::parseArguments(int argc, const char* const* argv) -> ConnectionInfo
{
    ConnectionInfo info;

    for (int i = 1; i < argc; ++i) {
        std::string_view option = argv[i];

        if (!isOption(option))
            throw Errors::Error("unknown option: " + std::string(option) + " (usage: -h <host> -p <port> -n <name>)");
        if (i + 1 >= argc || isOption(argv[i + 1]))
            throw Errors::Error("missing value for option " + std::string(option));

        ++i;
        if (option == "-h")
            info.host = argv[i];
        else if (option == "-p")
            info.port = argv[i];
        else
            info.name = argv[i];
    }
    return info;
}
