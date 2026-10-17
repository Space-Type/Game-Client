/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Private header for
** raylib log redirection
*/

#pragma once

// Include after the render headers: raylib.h defines
// color macros that clash with RType::Render::Colors
#include <cstdarg>
#include <cstdio>
#include <raylib.h>

namespace RType::Render::Log {

    /**
     * Prints a raylib message to stderr. raylib only calls it
     * for messages at or above the current trace log level.
     * Installing a callback also stops raylib from calling
     * exit() on LOG_FATAL: failures are reported by the caller
     */
    inline void raylibCallback(int logLevel, const char* text, va_list args)
    {
        (void)logLevel;
        std::fputs("raylib: ", stderr);
        std::vfprintf(stderr, text, args);
        std::fputc('\n', stderr);
    }

    /** Redirects raylib logs (global, safe to call more than once) */
    inline void installCallback() noexcept
    {
        SetTraceLogLevel(LOG_WARNING);
        SetTraceLogCallback(raylibCallback);
    }
}
