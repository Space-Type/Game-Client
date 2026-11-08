/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Radar game (local, my_radar rules)
*/

#include "Radar.hpp"
#include <rtype/render.hpp>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace RType::Render;

const float SCREEN_WIDTH = 1920.0F;
const float SCREEN_HEIGHT = 1080.0F;

const float PLANE_SIZE = 20.0F;

struct Plane {
    float x;
    float y;
    float speedX;
    float speedY;
    float distanceLeft;
    float delay;
    bool alive;
};

struct Tower {
    float x;
    float y;
    float radius;
};

static float distance(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    return std::sqrt(dx * dx + dy * dy);
}

static bool readPlane(std::istringstream& line, std::vector<Plane>& planes)
{
    float startX, startY, endX, endY, speed, delay;

    if (!(line >> startX >> startY >> endX >> endY >> speed >> delay))
        return false;
    if (speed <= 0 || delay < 0)
        return false;

    Plane plane;
    float totalDistance = distance(startX, startY, endX, endY);

    plane.x = startX;
    plane.y = startY;
    plane.speedX = 0;
    plane.speedY = 0;
    if (totalDistance > 0) {
        plane.speedX = (endX - startX) / totalDistance * speed;
        plane.speedY = (endY - startY) / totalDistance * speed;
    }
    plane.distanceLeft = totalDistance;
    plane.delay = delay;
    plane.alive = true;
    planes.push_back(plane);
    return true;
}

static bool readTower(std::istringstream& line, std::vector<Tower>& towers)
{
    float x, y, radius;

    if (!(line >> x >> y >> radius))
        return false;
    if (radius < 0)
        return false;

    Tower tower;

    tower.x = x;
    tower.y = y;
    tower.radius = radius;
    towers.push_back(tower);
    return true;
}

static bool readFile(const char* path, std::vector<Plane>& planes, std::vector<Tower>& towers)
{
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "radar: cannot open script: " << path << std::endl;
        return false;
    }

    std::string text;
    int lineNumber = 0;

    while (std::getline(file, text)) {
        lineNumber++;

        std::istringstream line(text);
        char type;

        if (!(line >> type))
            continue;

        bool ok = false;

        std::string extra;

        if (type == 'A')
            ok = readPlane(line, planes);
        else if (type == 'T')
            ok = readTower(line, towers);
        // Rien ne doit rester apres les valeurs attendues
        if (ok && line >> extra)
            ok = false;
        if (!ok) {
            std::cerr << "radar: " << path << ":" << lineNumber << ": invalid line: " << text << std::endl;
            return false;
        }
    }
    if (planes.empty()) {
        std::cerr << "radar: " << path << ": no aircraft in the script" << std::endl;
        return false;
    }
    return true;
}

static bool isFlying(const Plane& plane, float time)
{
    return plane.alive && time >= plane.delay;
}

static void movePlanes(std::vector<Plane>& planes, float time, float dt)
{
    for (size_t i = 0; i < planes.size(); i++) {
        Plane& plane = planes[i];

        if (!isFlying(plane, time))
            continue;

        float speed = std::sqrt(plane.speedX * plane.speedX + plane.speedY * plane.speedY);

        plane.x = plane.x + plane.speedX * dt;
        plane.y = plane.y + plane.speedY * dt;
        plane.distanceLeft = plane.distanceLeft - speed * dt;
        if (plane.distanceLeft <= 0)
            plane.alive = false;
    }
}

static bool isInsideTower(const Plane& plane, const std::vector<Tower>& towers)
{
    for (size_t i = 0; i < towers.size(); i++) {
        if (distance(plane.x, plane.y, towers[i].x, towers[i].y) <= towers[i].radius)
            return true;
    }
    return false;
}

static void checkCollisions(std::vector<Plane>& planes, const std::vector<Tower>& towers, float time)
{
    for (size_t i = 0; i < planes.size(); i++) {
        for (size_t j = i + 1; j < planes.size(); j++) {
            Plane& a = planes[i];
            Plane& b = planes[j];

            if (!isFlying(a, time) || !isFlying(b, time))
                continue;

            bool touching = std::abs(a.x - b.x) < PLANE_SIZE && std::abs(a.y - b.y) < PLANE_SIZE;

            if (!touching)
                continue;
            if (isInsideTower(a, towers) || isInsideTower(b, towers))
                continue;
            a.alive = false;
            b.alive = false;
        }
    }
}

static bool hasPlanesLeft(const std::vector<Plane>& planes)
{
    for (size_t i = 0; i < planes.size(); i++) {
        if (planes[i].alive)
            return true;
    }
    return false;
}

// Dessine l'image centree sur (x, y), redimensionnee en un carre de `size` pixels
static void drawCentered(Renderer& renderer, const Resources& resources,
    TextureId texture, float x, float y, float size)
{
    Vec2D textureSize = resources.textureSize(texture);
    SpriteParams params;

    params.position = {x, y};
    params.origin = {0.5F, 0.5F};
    params.scale = {size / textureSize.x, size / textureSize.y};
    renderer.drawSprite(texture, params);
}

static void draw(Renderer& renderer, const Resources& resources,
    TextureId planeSprite, TextureId towerSprite, const std::vector<Plane>& planes,
    const std::vector<Tower>& towers, float time, bool showHitboxes, bool showSprites)
{
    for (size_t i = 0; i < towers.size(); i++) {
        const Tower& tower = towers[i];

        if (showHitboxes)
            renderer.drawCircle({tower.x, tower.y}, tower.radius, {0, 228, 48, 60});
        if (showSprites)
            drawCentered(renderer, resources, towerSprite, tower.x, tower.y, 40);
    }

    for (size_t i = 0; i < planes.size(); i++) {
        const Plane& plane = planes[i];

        if (!isFlying(plane, time))
            continue;

        Rect box = {plane.x - PLANE_SIZE / 2, plane.y - PLANE_SIZE / 2, PLANE_SIZE, PLANE_SIZE};

        if (showSprites)
            drawCentered(renderer, resources, planeSprite, plane.x, plane.y, PLANE_SIZE);
        if (showHitboxes)
            renderer.drawRectLines(box, 2, Colors::RED);
    }

    std::string seconds = std::to_string((int)time);
    renderer.drawText(seconds.c_str(), {SCREEN_WIDTH - 120, 20}, 40, Colors::WHITE);
}

static void printUsage()
{
    std::cout << "Air traffic simulation panel\n"
                 "USAGE\n"
                 "  ./r-type_client --radar [OPTIONS] path_to_script\n"
                 "  path_to_script    The path to the script file.\n"
                 "OPTIONS\n"
                 "  -h                print the usage and quit.\n"
                 "USER INTERACTIONS\n"
                 "  `L' key           enable/disable hitboxes and areas.\n"
                 "  `S' key           enable/disable sprites.\n";
}

int RType::Radar::run(const char* path)
{
    if (std::string(path) == "-h") {
        printUsage();
        return 0;
    }

    std::vector<Plane> planes;
    std::vector<Tower> towers;

    if (!readFile(path, planes, towers))
        return 84;

    WindowConfig config;
    config.title = "Radar";
    // La fenetre s'ouvre en 1920x1080 et peut etre redimensionnee:
    // le dessin reste en 1920x1080 et s'adapte (bandes noires si besoin)
    config.width = (int)SCREEN_WIDTH;
    config.height = (int)SCREEN_HEIGHT;
    config.resizable = true;
    config.virtualWidth = (int)SCREEN_WIDTH;
    config.virtualHeight = (int)SCREEN_HEIGHT;

    Window window(config);
    Resources resources(window);
    Renderer renderer(resources);
    Input input(window);
    TextureId planeSprite = resources.loadTexture(assetPath("radar/plane.png"));
    TextureId towerSprite = resources.loadTexture(assetPath("radar/tower.png"));

    bool showHitboxes = true;   // touche L
    bool showSprites = true;    // touche S
    float time = 0;             // secondes depuis le debut de la simulation

    // Sert a mesurer le temps entre deux images
    std::chrono::steady_clock::time_point lastFrame = std::chrono::steady_clock::now();

    while (!window.shouldClose()) {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - lastFrame;
        float dt = elapsed.count();

        lastFrame = now;
        time = time + dt;

        if (input.isPressed(Key::L))
            showHitboxes = !showHitboxes;
        if (input.isPressed(Key::S))
            showSprites = !showSprites;

        movePlanes(planes, time, dt);
        checkCollisions(planes, towers, time);

        window.beginFrame(Colors::BLACK);
        draw(renderer, resources, planeSprite, towerSprite, planes, towers, time, showHitboxes, showSprites);
        window.endFrame();

        // Plus aucun avion: la simulation est finie
        if (!hasPlanesLeft(planes))
            window.close();
    }
    return 0;
}
