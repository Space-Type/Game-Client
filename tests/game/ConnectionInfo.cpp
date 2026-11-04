/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** ConnectionInfo struct
*/

#include <gtest/gtest.h>
#include <rtype/errors.hpp>
#include "../../src/game/ConnectionInfo/ConnectionInfo.hpp"
#include <string>
#include <vector>

namespace {

    using RType::Game::ConnectionInfo;
    using RType::Game::parseArguments;

    ConnectionInfo parse(std::vector<const char*> arguments)
    {
        arguments.insert(arguments.begin(), "r-type_client");
        return parseArguments(static_cast<int>(arguments.size()), arguments.data());
    }

    ConnectionInfo withHost(std::string host)
    {
        ConnectionInfo info;

        info.host = std::move(host);
        return info;
    }

    ConnectionInfo withPort(std::string port)
    {
        ConnectionInfo info;

        info.port = std::move(port);
        return info;
    }

    ConnectionInfo withName(std::string name)
    {
        ConnectionInfo info;

        info.name = std::move(name);
        return info;
    }
}


TEST(game_ConnectionInfo, defaultsAreValid)
{
    ConnectionInfo info;

    EXPECT_EQ(info.host, "127.0.0.1");
    EXPECT_EQ(info.port, "5432");
    EXPECT_EQ(info.name, "Player");
    EXPECT_EQ(info.validate(), std::nullopt);
}

TEST(game_ConnectionInfo, validHosts)
{
    for (const char* host : {"127.0.0.1", "192.168.1.42", "255.255.255.255", "localhost"})
        EXPECT_EQ(withHost(host).validate(), std::nullopt) << host;
}

TEST(game_ConnectionInfo, invalidHosts)
{
    for (const char* host : {"", "not an ip", "example.com", "256.0.0.1", "1.2.3.4.5", "1.2.3.4 ", "1.2.3.4 junk", " 1.2.3.4", "Localhost"})
        EXPECT_NE(withHost(host).validate(), std::nullopt) << "\"" << host << "\"";
}

// inet_aton accepts these on Linux only (inet_pton on Windows refuses them):
// a truncated address silently targets another machine
TEST(game_ConnectionInfo, invalidShortOrNonDecimalHosts)
{
    for (const char* host : {"1", "127.1", "192.168.1", "0x7f.0.0.1", "0177.0.0.1"})
        EXPECT_NE(withHost(host).validate(), std::nullopt) << host;
}

// Answers come from the real address: Client ignores them, the attempt can only time out
TEST(game_ConnectionInfo, invalidUnspecifiedHost)
{
    EXPECT_NE(withHost("0.0.0.0").validate(), std::nullopt);
}

TEST(game_ConnectionInfo, invalidDottedQuads)
{
    for (const char* host : {"01.2.3.4", "1.2.3.04", "00.0.0.1", "1.2.3.256", "1.2.3.1000", "1..3.4", ".1.2.3", "1.2.3.", "1.2.3.-4", "1.2.3.+4", "a.b.c.d"})
        EXPECT_NE(withHost(host).validate(), std::nullopt) << host;
}

TEST(game_ConnectionInfo, validDottedQuadBounds)
{
    for (const char* host : {"0.0.0.1", "1.0.0.0", "10.20.30.40", "100.200.255.0"})
        EXPECT_EQ(withHost(host).validate(), std::nullopt) << host;
}

TEST(game_ConnectionInfo, addressFollowsTheSameHostRules)
{
    for (const char* host : {"127.1", "0.0.0.0", "1.2.3.4 junk"})
        EXPECT_THROW((void)withHost(host).address(), RType::Errors::NetworkError) << host;
}

TEST(game_ConnectionInfo, validPorts)
{
    for (const char* port : {"1", "80", "5432", "65535"})
        EXPECT_EQ(withPort(port).validate(), std::nullopt) << port;
}

TEST(game_ConnectionInfo, invalidPorts)
{
    for (const char* port : {"", "0", "65536", "99999999999", "-1", "+1", "12a", "a12", " 12", "12 ", "1.5"})
        EXPECT_NE(withPort(port).validate(), std::nullopt) << "\"" << port << "\"";
}

TEST(game_ConnectionInfo, nameLength)
{
    EXPECT_NE(withName("").validate(), std::nullopt);
    EXPECT_EQ(withName("a").validate(), std::nullopt);
    EXPECT_EQ(withName(std::string(32, 'a')).validate(), std::nullopt);
    EXPECT_NE(withName(std::string(33, 'a')).validate(), std::nullopt);
}

TEST(game_ConnectionInfo, nameCharacters)
{
    for (const char* name : {"   ", " ", "caf\xC3\xA9", "\xFF", "a\tb", "a\x7F"})
        EXPECT_NE(withName(name).validate(), std::nullopt) << name;
    for (const char* name : {" a ", "Bob the 2nd", "~!@#"})
        EXPECT_EQ(withName(name).validate(), std::nullopt) << name;
}

TEST(game_ConnectionInfo, errorMessageIsPlainText)
{
    auto error = withPort("0").validate();

    ASSERT_TRUE(error.has_value());
    EXPECT_NE(error->find("Port"), std::string::npos);
    EXPECT_EQ(error->find('\033'), std::string::npos);
}

TEST(game_ConnectionInfo, address)
{
    ConnectionInfo info;

    info.host = "192.168.1.42";
    info.port = "1234";

    auto address = info.address();
    EXPECT_EQ(address.host(), "192.168.1.42");
    EXPECT_EQ(address.port(), 1234);
}

TEST(game_ConnectionInfo, addressConvertsLocalhost)
{
    auto address = withHost("localhost").address();

    EXPECT_EQ(address.host(), "127.0.0.1");
    EXPECT_EQ(address.port(), 5432);
}

TEST(game_ConnectionInfo, addressRejectsInvalidValues)
{
    EXPECT_THROW((void)withHost("nope").address(), RType::Errors::NetworkError);
    EXPECT_THROW((void)withPort("nope").address(), RType::Errors::NetworkError);
}

TEST(game_parseArguments, noArgumentsGiveDefaults)
{
    ConnectionInfo info = parse({});

    EXPECT_EQ(info.host, ConnectionInfo{}.host);
    EXPECT_EQ(info.port, ConnectionInfo{}.port);
    EXPECT_EQ(info.name, ConnectionInfo{}.name);
}

TEST(game_parseArguments, noArgumentAtAll)
{
    EXPECT_EQ(parseArguments(0, nullptr).host, ConnectionInfo{}.host);
}

TEST(game_parseArguments, readsEveryOption)
{
    ConnectionInfo info = parse({"-n", "Bob", "-h", "10.0.0.1", "-p", "4242"});

    EXPECT_EQ(info.host, "10.0.0.1");
    EXPECT_EQ(info.port, "4242");
    EXPECT_EQ(info.name, "Bob");
}

TEST(game_parseArguments, lastValueWins)
{
    EXPECT_EQ(parse({"-p", "1", "-p", "2"}).port, "2");
}

TEST(game_parseArguments, keepsInvalidValuesForValidate)
{
    EXPECT_EQ(parse({"-p", "nope"}).port, "nope");
}

TEST(game_parseArguments, unknownOptionThrows)
{
    EXPECT_THROW(parse({"-x"}), RType::Errors::Error);
    EXPECT_THROW(parse({"-h", "10.0.0.1", "stray"}), RType::Errors::Error);
}

TEST(game_parseArguments, missingValueThrows)
{
    for (const char* option : {"-h", "-p", "-n"})
        EXPECT_THROW(parse({option}), RType::Errors::Error) << option;
    EXPECT_THROW(parse({"-h", "-p", "1234"}), RType::Errors::Error);
}
