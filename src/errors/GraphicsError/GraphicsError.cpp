/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** GraphicsError class
*/

#include "GraphicsError.hpp"


auto RType::Errors::GraphicsError::clone() const -> std::unique_ptr<Error>
{
    return std::make_unique<GraphicsError>(*this);
}
