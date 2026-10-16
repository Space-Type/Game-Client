/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Header file for
** Audio class
*/

#pragma once

#include "Types.hpp"
#include <memory>
#include <string>

namespace Backend {

    /**
     * Audio device, sounds and music.
     *
     * If no audio device is available, the game keeps
     * running silently: `isReady()` is false, loads
     * return Invalid handles and every call is a no-op.
     *
     * Effective volume = Master * channel volume
     * (Music or Effects), each clamped to [0, 1].
     * Loads are cached by path; everything is released
     * by the destructor. Independent from Window.
     *
     * Only one Audio may exist at a time (the
     * underlying library has a single global device).
     */
    class Audio {
        struct Impl;
        std::unique_ptr<Impl> m_Impl;

        public:
            // Throws Backend::Error if an Audio already exists
            Audio();
            ~Audio();

            Audio(const Audio&) = delete;
            Audio(Audio&&) = delete;
            Audio& operator=(const Audio&) = delete;
            Audio& operator=(Audio&&) = delete;

            bool isReady() const noexcept;

            // Throws Backend::Error if the device is ready but the file cannot be loaded
            SoundId loadSound(const std::string& path);
            MusicId loadMusic(const std::string& path);

            // No-op for unknown, already unloaded or Invalid handles
            void unload(SoundId sound) noexcept;
            void unload(MusicId music) noexcept;

            // Restarts the sound if already playing
            void play(SoundId sound) noexcept;
            void stop(SoundId sound) noexcept;

            // Replaces the current music, if any
            void playMusic(MusicId music, bool loop = true) noexcept;
            void stopMusic() noexcept;
            void pauseMusic() noexcept;
            void resumeMusic() noexcept;

            // Streams the current music: call once per frame
            void update() noexcept;

            void setVolume(Channel channel, float volume) noexcept;
            float volume(Channel channel) const noexcept;
    };
}
