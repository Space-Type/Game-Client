/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** NetworkError class
*/

#pragma once

#include "../Error/Error.hpp"


namespace RType::Errors {
    class NetworkError : public Error {
        public:
            using Error::Error;

            std::string_view type() const noexcept override { return "NetworkError"; }
            std::unique_ptr<Error> clone() const override;
    };
}
