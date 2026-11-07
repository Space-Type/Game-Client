/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** EntityKind component
*/

#pragma once

#include <protecs/networking.hpp>


namespace RType::Components {
    // What the server created the entity as (EntityCreate packet)
    struct EntityKind {
        Protecs::Networking::Packets::EntityCreate::EntityType type = Protecs::Networking::Packets::EntityCreate::GENERIC;
    };
}
