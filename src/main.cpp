/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Program Entry
*/

#include <rtype.hpp>


int main(int argc, char *argv[])
{
    RType::Game::Game game;

    game.setFPS(120);
    game.run();
}
