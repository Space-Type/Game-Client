/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** PlayChargeSound system
*/

#include "PlayChargeSound.hpp"
#include <rtype/game.hpp>


RType::Systems::PlayChargeSound::PlayChargeSound() noexcept
    : m_Engine(nullptr)
    , m_Audio(nullptr)
{

}

void RType::Systems::PlayChargeSound::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& sound = engine.get<Components::ChargeSound>()[entity];
    float seconds = engine.get<Components::LocalCharge>()[entity].seconds;

    switch (Components::chargeSoundChange(seconds, sound.playing)) {
        case Components::SoundChange::Play:
            if (m_Audio)
                m_Audio->play(sound.sound);
            sound.playing = true;
            return;
        case Components::SoundChange::Stop:
            if (m_Audio)
                m_Audio->stop(sound.sound);
            sound.playing = false;
            return;
        case Components::SoundChange::None:
            return;
    }
}
