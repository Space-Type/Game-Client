/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** EntityMap class
*/

#pragma once

#include <protecs/common.hpp>
#include <wrecks/ecs.hpp>
#include <unordered_map>


namespace RType::Utils {
    class EntityMap {
        std::unordered_map<Protecs::NetworkEntity, Wrecks::ECS::Entity> m_NetworkToLocal;
        std::unordered_map<Wrecks::ECS::Entity, Protecs::NetworkEntity> m_LocalToNetwork;

        public:
            EntityMap() noexcept;

            Wrecks::ECS::Entity toLocal(Protecs::NetworkEntity entity) const;
            Wrecks::ECS::Entity toLocal(Wrecks::ECS::Entity entity) const = delete;

            Protecs::NetworkEntity toNetwork(Wrecks::ECS::Entity entity) const;
            Protecs::NetworkEntity toNetwork(Protecs::NetworkEntity entity) const = delete;

            bool containsLocal(Wrecks::ECS::Entity local) const noexcept;
            bool containsLocal(Protecs::NetworkEntity local) const noexcept = delete;

            bool containsNetwork(Protecs::NetworkEntity network) const noexcept;
            bool containsNetwork(Wrecks::ECS::Entity network) const noexcept = delete;

            bool bind(Protecs::NetworkEntity network, Wrecks::ECS::Entity local);
            bool bind(Wrecks::ECS::Entity a, Protecs::NetworkEntity b) = delete;

            void removeLocal(Wrecks::ECS::Entity entity);
            void removeNetwork(Protecs::NetworkEntity entity);
    };
}
