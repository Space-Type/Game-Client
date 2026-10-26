/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** EntityMap class
*/

#include "EntityMap.hpp"
#include "protecs/common.hpp"


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
    return m_NetworkToLocal.at(entity);
}

/**
 * Converts a local entity id
 * into a network entity id.
 */
auto RType::Utils::EntityMap::toNetwork(Wrecks::ECS::Entity entity) const -> Protecs::NetworkEntity
{
    return m_LocalToNetwork.at(entity);
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
