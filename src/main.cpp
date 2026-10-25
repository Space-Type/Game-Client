/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Program Entry
*/

#include <rtype.hpp>
#include <iostream>


void unsafeMain(int argc, char *argv[])
{
    RType::Game::Game game;

    game.setFPS(120);
    game.run();
}

int main(int argc, char *argv[])
{
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
