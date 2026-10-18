/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Source file for
** asset path helper
*/

#include "Assets.hpp"
#include "raylib.h"
#include <filesystem>

namespace {

    // Parents searched above the executable directory:
    // build/ and build/<Config>/ (multi-config generators)
    constexpr int MAX_PARENT_DEPTH = 2;

    std::filesystem::path findAssetsDirectory()
    {
        const std::filesystem::path executableDirectory = std::filesystem::path(GetApplicationDirectory()).parent_path();
        std::filesystem::path directory = executableDirectory;
        std::error_code error;

        for (int depth = 0; depth <= MAX_PARENT_DEPTH; ++depth) {
            if (std::filesystem::is_directory(directory / "assets", error))
                return directory / "assets";
            directory = directory.parent_path();
        }
        // Not found: load errors will name the expected location
        return executableDirectory / "assets";
    }
}

std::string RType::Render::assetPath(std::string_view name)
{
    static const std::filesystem::path assetsDirectory = findAssetsDirectory();

    return (assetsDirectory / name).string();
}
