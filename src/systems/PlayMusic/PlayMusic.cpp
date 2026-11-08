/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** PlayMusic system
*/

#include "PlayMusic.hpp"
#include <rtype/errors.hpp>
#include <rtype/game.hpp>
#include <iostream>


RType::Systems::PlayMusic::PlayMusic() noexcept
    : m_Engine(nullptr)
    , m_Audio(nullptr)
{

}

void RType::Systems::PlayMusic::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const std::string& track = engine.get<Components::Music>()[entity].track;

    if (!m_Audio)
        return;
    if (track != m_Playing) {
        m_Playing = track;
        m_Audio->stopMusic();
        if (!track.empty()) {
            try {
                m_Audio->playMusic(m_Audio->loadMusic(Render::assetPath(track)), true);
            } catch (const std::exception& error) {
                std::cerr << "music " << track << ": " << error.what() << std::endl;
            }
        }
    }
    m_Audio->update();
}
