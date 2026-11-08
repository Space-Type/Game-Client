/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Program Entry
*/

#include <rtype.hpp>
#include "radar/Radar.hpp"
#include <iostream>
#include <string_view>


void unsafeMain(int argc, char *argv[])
{
    RType::Game::Game game(RType::Game::parseArguments(argc, argv));

    game.setFPS(120);
    game.run();
}

int main(int argc, char *argv[])
{
    if (argc >= 2 && std::string_view(argv[1]) == "--radar") {
        if (argc != 3) {
            std::cerr << argv[0] << " --radar: bad arguments: " << argc - 2
                      << " given but 1 is required" << std::endl << "retry with -h" << std::endl;
            return 84;
        }
        try {
            return RType::Radar::run(argv[2]);
        } catch (const std::exception& err) {
            std::cerr << err.what() << std::endl;
            return 84;
        }
    }
    try {
        unsafeMain(argc, argv);
    } catch (const RType::Errors::Error& err) {
        std::cerr << err.what() << std::endl;
        return err.status();
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 1;
    }
}
