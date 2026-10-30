/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** scene Manager class
*/

#pragma once

#include "../SceneId.hpp"
#include <wrecks/common.hpp>
#include <concepts>
#include <cstddef>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>


namespace RType::Scenes {
    /**
     * A scene of the game. `update` returns the
     * scene to switch to, if any. Nothrow default
     * construction: a scene change never leaves
     * the manager without a scene.
     */
    template <typename S, typename Ctx>
    concept Scene = std::default_initializable<S>
        && std::is_nothrow_default_constructible_v<S>
        && requires (S scene, const S& constScene, Ctx& ctx, Wrecks::DeltaTime dt) {
            { scene.update(ctx, dt) } -> std::same_as<std::optional<SceneId>>;
            { constScene.render(ctx) } -> std::same_as<void>;
        };

    /**
     * Holds the active scene, one of `Scenes`,
     * listed in SceneId order (Quit excluded).
     *
     * Scene changes are deferred: `request()` (or a
     * value returned by the scene's update) is applied
     * by `applyPending()`, at the end of the frame.
     * The last request of a frame wins.
     */
    template <typename Ctx, Scene<Ctx>... Scenes>
        requires (sizeof...(Scenes) == static_cast<std::size_t>(SceneId::Quit))
    class Manager {
        std::variant<Scenes...> m_Scene;
        std::optional<SceneId> m_Pending;
        bool m_QuitRequested;

        public:
            Manager()
                : m_Scene(std::in_place_index<0>)
                , m_Pending()
                , m_QuitRequested(false)
            {}

            inline void request(SceneId scene) noexcept { m_Pending = scene; }

            std::optional<SceneId> update(Ctx& ctx, Wrecks::DeltaTime dt)
            {
                std::optional<SceneId> next = std::visit([&](auto& scene){
                    return scene.update(ctx, dt);
                }, m_Scene);

                if (next)
                    request(*next);
                return next;
            }

            void render(Ctx& ctx) const
            {
                std::visit([&](const auto& scene){ scene.render(ctx); }, m_Scene);
            }

            /**
             * Applies the pending request: the requested
             * scene is constructed anew (even if it is the
             * current one), Quit only sets `quitRequested()`.
             * Returns false if there was nothing to apply.
             */
            bool applyPending()
            {
                if (!m_Pending)
                    return false;

                auto scene = static_cast<std::size_t>(*m_Pending);
                m_Pending.reset();

                if (scene == static_cast<std::size_t>(SceneId::Quit)) {
                    m_QuitRequested = true;
                    return true;
                }
                return emplace(scene, std::index_sequence_for<Scenes...>{});
            }

            [[ nodiscard ]] inline SceneId current() const noexcept { return static_cast<SceneId>(m_Scene.index()); }
            [[ nodiscard ]] inline bool quitRequested() const noexcept { return m_QuitRequested; }

        private:
            template <std::size_t... I>
            bool emplace(std::size_t scene, std::index_sequence<I...>)
            {
                return ((scene == I && (m_Scene.template emplace<I>(), true)) || ...);
            }
    };
}
