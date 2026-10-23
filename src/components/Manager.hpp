/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Components type
*/

#pragma once

#include <rtype/components/types.hpp>
#include <protecs/components.hpp>
#include <wrecks/ecs.hpp>


namespace RType::Components {
    using Manager = Wrecks::Utils::concat_types_t<
        Protecs::Components::Manager,
        Wrecks::ECS::ComponentManager<Controllable,
                                      Dead,
                                      HasRandomMovement,
                                      Health,
                                      Hitbox,
                                      Projectile,
                                      Shooter,
                                      Sprite,
                                      Star>
    >;
}
