/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Header file for
** backend Error class
*/

#pragma once

#include <wrecks/errors.hpp>

namespace Backend {

    /**
     * Thrown on unrecoverable backend failures:
     * window creation, second Window instance,
     * asset that cannot be loaded.
     */
    class Error : public Wrecks::Error {
        public:
            using Wrecks::Error::Error;
    };
}
