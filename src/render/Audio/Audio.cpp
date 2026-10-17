/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Source file for
** Audio class
*/

// Render headers must come before raylib.h (color macros clash)
#include "Audio.hpp"
#include "../Error.hpp"
#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include "../Log.hpp"
#include <raylib.h>

namespace {

    template <typename T>
    struct Loaded {
        T handle;
        std::string path;
    };

    constexpr std::size_t CHANNELS = 3;

    // raylib has a single global audio device
    std::atomic<bool> s_AudioExists = false;

    float clampVolume(float volume) noexcept
    {
        return std::isnan(volume) ? 0.0F : std::clamp(volume, 0.0F, 1.0F);
    }
}

struct RType::Render::Audio::Impl {
    bool ready = false;
    std::uint32_t nextId = 1;   // Shared by sounds and musics
    std::unordered_map<std::uint32_t, Loaded<::Sound>> sounds;
    std::unordered_map<std::uint32_t, Loaded<::Music>> musics;
    std::unordered_map<std::string, SoundId> soundPaths;
    std::unordered_map<std::string, MusicId> musicPaths;
    MusicId current = MusicId::Invalid;
    std::array<float, CHANNELS> volumes = {1.0F, 1.0F, 1.0F};

    float channelVolume(Channel channel) const noexcept
    {
        return volumes[static_cast<std::size_t>(channel)];
    }

    ::Music* currentMusic() noexcept
    {
        auto it = musics.find(static_cast<std::uint32_t>(current));

        return it == musics.end() ? nullptr : &it->second.handle;
    }
};

RType::Render::Audio::Audio()
{
    if (s_AudioExists.exchange(true))
        throw Error("Audio: an audio device already exists");

    try {
        m_Impl = std::make_unique<Impl>();
        Log::installCallback();
        InitAudioDevice();
        m_Impl->ready = IsAudioDeviceReady();
    } catch (...) {
        s_AudioExists = false;
        throw;
    }
}

RType::Render::Audio::~Audio()
{
    for (auto& [id, sound] : m_Impl->sounds)
        UnloadSound(sound.handle);
    for (auto& [id, music] : m_Impl->musics)
        UnloadMusicStream(music.handle);
    if (m_Impl->ready)
        CloseAudioDevice();
    s_AudioExists = false;
}

bool RType::Render::Audio::isReady() const noexcept
{
    return m_Impl->ready;
}


/////////////////////////////////////////////////////////
//                                                     //
//                       LOADING                       //
//                                                     //
/////////////////////////////////////////////////////////

RType::Render::SoundId RType::Render::Audio::loadSound(const std::string& path)
{
    if (!m_Impl->ready)
        return SoundId::Invalid;
    if (auto it = m_Impl->soundPaths.find(path); it != m_Impl->soundPaths.end())
        return it->second;

    ::Sound sound = LoadSound(path.c_str());

    if (!IsSoundValid(sound))
        throw Error("Cannot load sound: " + path);

    std::uint32_t id = m_Impl->nextId;

    try {
        m_Impl->sounds.emplace(id, Loaded<::Sound>{sound, path});
        m_Impl->soundPaths.emplace(path, static_cast<SoundId>(id));
    } catch (...) {
        m_Impl->sounds.erase(id);
        UnloadSound(sound);
        throw;
    }
    ++m_Impl->nextId;
    return static_cast<SoundId>(id);
}

RType::Render::MusicId RType::Render::Audio::loadMusic(const std::string& path)
{
    if (!m_Impl->ready)
        return MusicId::Invalid;
    if (auto it = m_Impl->musicPaths.find(path); it != m_Impl->musicPaths.end())
        return it->second;

    ::Music music = LoadMusicStream(path.c_str());

    if (!IsMusicValid(music))
        throw Error("Cannot load music: " + path);

    std::uint32_t id = m_Impl->nextId;

    try {
        m_Impl->musics.emplace(id, Loaded<::Music>{music, path});
        m_Impl->musicPaths.emplace(path, static_cast<MusicId>(id));
    } catch (...) {
        m_Impl->musics.erase(id);
        UnloadMusicStream(music);
        throw;
    }
    ++m_Impl->nextId;
    return static_cast<MusicId>(id);
}

void RType::Render::Audio::unload(SoundId sound) noexcept
{
    auto it = m_Impl->sounds.find(static_cast<std::uint32_t>(sound));

    if (it == m_Impl->sounds.end())
        return;
    UnloadSound(it->second.handle);
    m_Impl->soundPaths.erase(it->second.path);
    m_Impl->sounds.erase(it);
}

void RType::Render::Audio::unload(MusicId music) noexcept
{
    auto it = m_Impl->musics.find(static_cast<std::uint32_t>(music));

    if (it == m_Impl->musics.end())
        return;
    if (m_Impl->current == music) {
        StopMusicStream(it->second.handle);
        m_Impl->current = MusicId::Invalid;
    }
    UnloadMusicStream(it->second.handle);
    m_Impl->musicPaths.erase(it->second.path);
    m_Impl->musics.erase(it);
}


/////////////////////////////////////////////////////////
//                                                     //
//                       PLAYBACK                      //
//                                                     //
/////////////////////////////////////////////////////////

void RType::Render::Audio::play(SoundId sound) noexcept
{
    auto it = m_Impl->sounds.find(static_cast<std::uint32_t>(sound));

    if (it == m_Impl->sounds.end())
        return;
    SetSoundVolume(it->second.handle, m_Impl->channelVolume(Channel::Effects));
    PlaySound(it->second.handle);
}

void RType::Render::Audio::stop(SoundId sound) noexcept
{
    auto it = m_Impl->sounds.find(static_cast<std::uint32_t>(sound));

    if (it != m_Impl->sounds.end())
        StopSound(it->second.handle);
}

void RType::Render::Audio::playMusic(MusicId music, bool loop) noexcept
{
    auto it = m_Impl->musics.find(static_cast<std::uint32_t>(music));

    if (it == m_Impl->musics.end())
        return;
    stopMusic();

    ::Music& handle = it->second.handle;

    handle.looping = loop;
    SetMusicVolume(handle, m_Impl->channelVolume(Channel::Music));
    PlayMusicStream(handle);
    m_Impl->current = music;
}

void RType::Render::Audio::stopMusic() noexcept
{
    if (::Music* music = m_Impl->currentMusic())
        StopMusicStream(*music);
    m_Impl->current = MusicId::Invalid;
}

void RType::Render::Audio::pauseMusic() noexcept
{
    if (::Music* music = m_Impl->currentMusic())
        PauseMusicStream(*music);
}

void RType::Render::Audio::resumeMusic() noexcept
{
    if (::Music* music = m_Impl->currentMusic())
        ResumeMusicStream(*music);
}

void RType::Render::Audio::update() noexcept
{
    if (::Music* music = m_Impl->currentMusic())
        UpdateMusicStream(*music);
}


/////////////////////////////////////////////////////////
//                                                     //
//                        VOLUME                       //
//                                                     //
/////////////////////////////////////////////////////////

void RType::Render::Audio::setVolume(Channel channel, float volume) noexcept
{
    auto index = static_cast<std::size_t>(channel);

    if (index >= CHANNELS)
        return;
    volume = clampVolume(volume);
    m_Impl->volumes[index] = volume;
    if (!m_Impl->ready)
        return;
    switch (channel) {
        case Channel::Master:
            SetMasterVolume(volume);
            break;
        case Channel::Music:
            if (::Music* music = m_Impl->currentMusic())
                SetMusicVolume(*music, volume);
            break;
        case Channel::Effects:
            for (auto& [id, sound] : m_Impl->sounds)
                SetSoundVolume(sound.handle, volume);
            break;
    }
}

float RType::Render::Audio::volume(Channel channel) const noexcept
{
    auto index = static_cast<std::size_t>(channel);

    return index < CHANNELS ? m_Impl->volumes[index] : 0.0F;
}
