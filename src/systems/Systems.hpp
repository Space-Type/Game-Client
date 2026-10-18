/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Systems type
*/

#pragma once

#include <rtype/systems/types.hpp>
#include <tuple>


namespace RType::Systems {
    // Render systems draw in this order: stars behind the sprites
    using Systems = std::tuple<Controllable,
                               Physics,
                               RandomMovement,
                               Starfield,
                               StarRender,
                               Render>;
}
