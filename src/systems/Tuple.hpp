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
    // Order matters: systems run in this order every frame.
    // Render systems draw in this order: stars behind the sprites
    using Tuple = std::tuple<Controllable,
                             RandomMovement,
                             Shoot,
                             Projectile,
                             Collision,
                             Starfield,
                             StarRender,
                             Render,
                             Dead>;
}
