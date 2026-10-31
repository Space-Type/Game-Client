/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Error class
*/

#pragma once

#include <source_location>
#include <string_view>
#include <cstdint>
#include <string>
#include <memory>


namespace RType::Errors {
    class Error : public std::exception {
        protected:
            std::unique_ptr<Error> m_ParentError;
            std::source_location m_Location;
            std::string m_Message;
            std::uint8_t m_Status;

        public:
            Error(std::string_view msg, const Error& parent, const std::source_location loc = std::source_location::current());
            Error(std::string_view msg, const std::exception& parent, const std::source_location loc = std::source_location::current());
            Error(std::string_view msg, const std::source_location loc = std::source_location::current());
            Error(const Error& cpy);

            virtual std::string_view type() const noexcept { return "Error"; }
            virtual std::unique_ptr<Error> clone() const;

            const char* what() const noexcept override;
            inline std::string_view message() const noexcept { return m_Message; }  // what(), for the screen: no location nor colors
            inline std::uint8_t status() const noexcept { return m_Status; }

        private:
            std::string generateMessage(unsigned int depth = 0) const;

        private:
            class ErrorWrapper;
    };

    class Error::ErrorWrapper : public Error {
        public:
            ErrorWrapper(const std::exception& err);

            std::unique_ptr<Error> clone() const override;
    };
}
