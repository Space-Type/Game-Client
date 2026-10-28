/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** EntityMap class
*/

#include "EntityMap.hpp"
#include <rtype/errors.hpp>
#include <protecs/common.hpp>
#include <stdexcept>


RType::Utils::EntityMap::EntityMap() noexcept
    : m_NetworkToLocal()
    , m_LocalToNetwork()
{

}

/**
 * Converts a network entity
 * id into a local entity id.
 */
auto RType::Utils::EntityMap::toLocal(Protecs::NetworkEntity entity) const -> Wrecks::ECS::Entity
{
    try {
        return m_NetworkToLocal.at(entity);
    } catch (const std::out_of_range& err) {
        throw Errors::Error("failed to find network entity in entity map", err);
    }
}

/**
 * Converts a local entity id
 * into a network entity id.
 */
auto RType::Utils::EntityMap::toNetwork(Wrecks::ECS::Entity entity) const -> Protecs::NetworkEntity
{
    try {
        return m_LocalToNetwork.at(entity);
    } catch (const std::out_of_range& err) {
        throw Errors::Error("failed to find local entity in entity map", err);
    }
}

/**
 * Determines of the map contains
 * the given local entity.
 */
bool RType::Utils::EntityMap::containsLocal(Wrecks::ECS::Entity local) const noexcept
{
    return m_LocalToNetwork.contains(local);
}

/**
 * Determines of the map contains
 * the given network entity.
 */
bool RType::Utils::EntityMap::containsNetwork(Protecs::NetworkEntity network) const noexcept
{
    return m_NetworkToLocal.contains(network);
}

/**
 * Binds a network id to
 * a local entity id. On
 * success, returns true.
 * Returns false otherwise.
 */
bool RType::Utils::EntityMap::bind(Protecs::NetworkEntity network, Wrecks::ECS::Entity local)
{
    if (m_LocalToNetwork.contains(local) || m_NetworkToLocal.contains(network))
        return false;

    m_LocalToNetwork[local] = network;
    m_NetworkToLocal[network] = local;

    return true;
}

/**
 * Removes an entity binding
 * using the local value.
 *
 * Note:
 * The whole binding will be
 * removed.
 */
void RType::Utils::EntityMap::removeLocal(Wrecks::ECS::Entity entity)
{
    auto localIt = m_LocalToNetwork.find(entity);
    if (localIt == m_LocalToNetwork.end())
        return;

    m_NetworkToLocal.erase(localIt->second);
    m_LocalToNetwork.erase(localIt);
}

/**
 * Removes an entity binding
 * using the network value.
 *
 * Note:
 * The whole binding will be
 * removed.
 */
void RType::Utils::EntityMap::removeNetwork(Protecs::NetworkEntity entity)
{
    auto networkIt = m_NetworkToLocal.find(entity);
    if (networkIt == m_NetworkToLocal.end())
        return;

    m_LocalToNetwork.erase(networkIt->second);
    m_NetworkToLocal.erase(networkIt);
}
