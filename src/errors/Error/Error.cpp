/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Error class
*/

#include "Error.hpp"


RType::Errors::Error::Error(std::string_view msg, const Error& error, const std::source_location loc)
    : m_ParentError(error.clone())
    , m_Location(loc)
    , m_Message(msg)
    , m_Status(1)
{

}

RType::Errors::Error::Error(std::string_view msg, const std::exception& error, const std::source_location loc)
    : m_ParentError(std::make_unique<ErrorWrapper>(error))
    , m_Location(loc)
    , m_Message(msg)
    , m_Status(1)
{

}

RType::Errors::Error::Error(std::string_view msg, const std::source_location loc)
    : m_ParentError()
    , m_Location(loc)
    , m_Message(msg)
    , m_Status(1)
{

}

RType::Errors::Error::Error(const Error& cpy)
    : m_Message(cpy.m_Message)
    , m_Location(cpy.m_Location)
    , m_Status(cpy.m_Status)
    , m_ParentError(cpy.m_ParentError ? cpy.m_ParentError->clone() : nullptr)
{

}

auto RType::Errors::Error::clone() const -> std::unique_ptr<Error>
{
    return std::make_unique<Error>(*this);
}

auto RType::Errors::Error::what() const noexcept -> const char*
{
    static std::string fullMessage;

    // attempt fully qualified message if possible,
    // but fall back to plain message if not.
    try {
        fullMessage = generateMessage();
        return fullMessage.c_str();
    } catch (...) {
        return m_Message.c_str();
    }
}

#include <filesystem>

auto RType::Errors::Error::generateMessage(unsigned int depth) const -> std::string
{
    std::string message;

    message.clear();

    if (m_ParentError) {
        message
            .append(m_ParentError->generateMessage(depth + 1))
            .append("\n");
    }

    auto filepath = std::filesystem::relative(m_Location.file_name());

    if (depth == 0)
        message.append("\033[35;1mError\033[0m:\n  ");
    else
        message.append("\033[35;1mCaused by\033[0m:\n  ");

    message
        .append(filepath.c_str()[0] == '\0' ? "<?>" : filepath.c_str())
        .append(":")
        .append(std::to_string(m_Location.line()))
        .append(":")
        .append(std::to_string(m_Location.column()))
        .append(": \033[31;1m")
        .append(type())
        .append("\033[0m: ")
        .append(m_Message);

    return message;
}

RType::Errors::Error::ErrorWrapper::ErrorWrapper(const std::exception& err)
    : Error(err.what(), std::source_location())
{

}

auto RType::Errors::Error::ErrorWrapper::clone() const -> std::unique_ptr<Error>
{
    return std::make_unique<ErrorWrapper>(*this);
}
