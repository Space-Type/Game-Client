/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Render Error class
*/

#pragma once

#include <wrecks/errors.hpp>

namespace RType::Render {

    /**
     * Thrown on unrecoverable render failures:
     * window creation, second Window instance,
     * asset that cannot be loaded.
     */
    class Error : public Wrecks::Error {
        public:
            using Wrecks::Error::Error;
    };
}
