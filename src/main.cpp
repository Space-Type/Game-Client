/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Program Entry
*/

#include "wrecks/common.hpp"
#include <wrecks.hpp>
#include <iostream>



//////////////////////////////////////////////////////////////////////
//                                                                  //
//                            Components                            //
//                                                                  //
//////////////////////////////////////////////////////////////////////

struct NameComponent {
    std::string name;
};

struct MessageComponent {
    std::string message;
};

using Components = Wrecks::ECS::ComponentManager<NameComponent,
                                                 MessageComponent>;


//////////////////////////////////////////////////////////////////////
//                                                                  //
//                              Systems                             //
//                                                                  //
//////////////////////////////////////////////////////////////////////


class LogSystem {
    void* m_Engine;

    public:
        constexpr LogSystem() noexcept
            : m_Engine(nullptr)
        {
        }

        static constexpr Wrecks::Signature signature() noexcept
        {
            return Wrecks::Signature()
                .set(Components::index<NameComponent>())
                .set(Components::index<MessageComponent>());
        }

        constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }

        void operator()(Wrecks::ECS::Entity entity);
};

using Systems = std::tuple<LogSystem>;


//////////////////////////////////////////////////////////////////////
//                                                                  //
//                           Engine Setup                           //
//                                                                  //
//////////////////////////////////////////////////////////////////////

using Engine = Wrecks::ECS::Engine<Systems, Components>;


//////////////////////////////////////////////////////////////////////
//                                                                  //
//                          Implementation                          //
//                                                                  //
//////////////////////////////////////////////////////////////////////

void LogSystem::operator()(Wrecks::ECS::Entity entity)
{
    Engine& engine = *reinterpret_cast<Engine*>(m_Engine);

    auto& names = engine.get<NameComponent>();
    auto& messages = engine.get<MessageComponent>();

    std::cout << "<" << names[entity].name << "> " << messages[entity].message << std::endl;
}

int main(int argc, char *argv[])
{
    Engine engine;

    Wrecks::ECS::Entity jean = engine.createEntity();
    engine.addComponent(jean, NameComponent{"Jean-Eudes"});
    engine.addComponent(jean, MessageComponent{"pipicaca"});

    Wrecks::ECS::Entity michel = engine.createEntity();
    engine.addComponent(michel, NameComponent{"Michel"});

    engine.update();

    engine.deleteEntity(jean);
    engine.deleteEntity(michel);
}
