/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Program Entry
*/

#include <rtype.hpp>
#include <raylib.h>


int main(int argc, char *argv[])
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
    RType::Game::Game game;

    game.run();
}
