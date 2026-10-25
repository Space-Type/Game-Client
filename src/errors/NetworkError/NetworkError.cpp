/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** NetworkError class
*/

#include "NetworkError.hpp"


auto RType::Errors::NetworkError::clone() const -> std::unique_ptr<Error>
{
    return std::make_unique<NetworkError>(*this);
}
