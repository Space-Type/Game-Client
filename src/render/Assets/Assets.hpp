/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** asset path helper
*/

#pragma once

#include <string>
#include <string_view>

namespace RType::Render {

    /**
     * Full path of a file of the assets/ folder, searched
     * next to the executable, then in its parent and
     * grandparent directories (build/, build/<Config>/).
     * Loading does not depend on the working directory.
     *
     * Example: assetPath("spaceship.png")
     */
    std::string assetPath(std::string_view name);
}
