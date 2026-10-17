# Render

`rtype_render` (namespace `RType::Render`) is the client's platform layer: window, 2D rendering, input and
audio, implemented with [raylib](https://www.raylib.com/) but exposed through our
own API. Game code includes `<rtype/render.hpp>` and never `raylib.h` (raylib is linked
`PRIVATE`, so its headers are not even reachable).

The library knows nothing about R-Type or the ECS, so it can later move into the
engine (Wrecks) as an optional module.

## Layout

Same layout as the rest of the client and Wrecks: one class per folder, headers
next to their sources, and an aggregate header in `include/`.

```
include/rtype/render.hpp             aggregate header (the only include game code needs)
src/render/<Class>/<Class>.{hpp,cpp} one class per folder (Window, Renderer, Input...)
src/render/*.hpp                     header-only parts (Types, InputTypes, Concepts, ActionMap, Error)
src/render/Convert.hpp, Log.hpp,     private headers, they include raylib.h: never
  Input/KeyMap.hpp,                  include them from game code or from a public header
  Resources/ResourcesImpl.hpp
tests/render/                        headless unit tests (GoogleTest, DebugTests build)
tests/render/manual/main.cpp         manual test program (needs a display)
```

## Quick start

```cpp
#include <rtype/render.hpp>
#include <chrono>

int main()
{
    RType::Render::Window window({.title = "R-Type", .virtualWidth = 1920, .virtualHeight = 1080});
    RType::Render::Resources resources(window);   // after Window, destroyed before it
    RType::Render::Renderer renderer(resources);
    RType::Render::Input input(window);
    RType::Render::Audio audio;                   // independent from Window

    RType::Render::TextureId ship = resources.loadTexture("assets/ship.png");
    RType::Render::Vec2 position = {200.0F, 540.0F};

    auto previous = std::chrono::steady_clock::now();
    while (!window.shouldClose()) {
        auto now = std::chrono::steady_clock::now();
        float delta = std::chrono::duration<float>(now - previous).count();
        previous = now;

        if (input.isDown(RType::Render::Key::Right))
            position.x += 500.0F * delta;     // speed in virtual pixels per second
        if (input.isPressed(RType::Render::Key::Escape))
            window.close();
        audio.update();

        window.beginFrame(RType::Render::Colors::BLACK);
        renderer.drawSprite(ship, {.position = position, .origin = {0.5F, 0.5F}});
        window.endFrame();
    }
}
```

Declaration order matters: C++ destroys objects in reverse order, so declaring
`Window` first guarantees it outlives everything that depends on it.

## Core concepts

### Virtual resolution

All drawing and the mouse position use **virtual coordinates**
(`[0, virtualWidth] x [0, virtualHeight]`), whatever the real window size. The
frame is rendered off-screen, then scaled and letterboxed into the window. Server
and clients can therefore share a single world coordinate system.

### Handles, not library objects

`Resources` and `Audio` own the textures, fonts, sounds and musics, and return
plain handles (`TextureId`, `FontId`, `SoundId`, `MusicId`). Handles are trivially
copyable, so they are what ECS components store:

```cpp
struct Sprite {                     // an ECS component
    RType::Render::TextureId texture;
    RType::Render::Rect frame;        // sprite-sheet frame
    RType::Render::Color tint;        // e.g. one color per player
};

// In a render system:
renderer.drawSprite(sprite.texture, {.position = {transform.x, transform.y},
                                     .source = sprite.frame, .tint = sprite.tint});
```

Never put a raylib object (or anything that unloads itself in a destructor) in a
component: components are moved and copied around by their containers.

### Timing

The render module never paces time (there is no FPS cap). The game loop measures time
itself with `std::chrono::steady_clock` and moves things by `speed * delta`, so
gameplay does not depend on the frame rate (a subject requirement).

### Controls and remapping

`ActionMap<A>` binds game actions (any enum) to keys, mouse buttons, gamepad
buttons and gamepad axes. Bindings can be changed at runtime, for a remapping menu
(accessibility). `Input::nextPressed()` and `keyName()` help build that menu.

```cpp
enum class Action { Up, Shoot };

RType::Render::ActionMap<Action> controls;
controls.bind(Action::Up, RType::Render::Key::Up);
controls.bind(Action::Up, RType::Render::AxisBinding{RType::Render::GamepadAxis::LeftY, -0.5F});
controls.bind(Action::Shoot, RType::Render::Key::Space);
controls.bind(Action::Shoot, RType::Render::GamepadButton::A);

if (controls.isPressed(Action::Shoot, input))
    audio.play(shootSound);
```

`ActionMap` only needs something satisfying the `InputType` concept, so it can
be unit-tested with a fake input. `RendererType` and `AudioType` do the same
for systems that draw or play sounds.

## Rules to know

- **One `Window` and one `Audio` at a time.** Creating a second one throws
  `RType::Render::Error`.
- **Handles:** valid ids start at 1, and `0` is `Invalid`. `FontId::Default` (0) is
  the built-in font. Unknown or unloaded handles are ignored: nothing is drawn and
  nothing is played.
- **Loading** a missing or unsupported file throws `RType::Render::Error`. Loading the
  same path twice returns the same handle.
- **No audio device:** `Audio::isReady()` is false, loads return `Invalid`, and
  every call is a silent no-op. The game keeps running.
- **Volume:** the effective volume is `Master * Music` or `Master * Effects`, each
  clamped to [0, 1].
- **Draw calls** are only valid between `beginFrame()` and `endFrame()`.
  `endFrame()` also polls input events.
- **Escape does not close the window.** The game decides, by calling `close()`.
- **Keys** are physical positions on a US QWERTY layout (`Key::W` is `Z` on
  AZERTY). Show them to the player with `keyName()`.
- **Gamepad axes:** sticks are in [-1, 1], triggers in [0, 1] (0 when released).
  `AxisBinding` counts for `isDown()` only.
- **`SpriteParams::scale`** must be positive. Mirror a sprite with `flipX` /
  `flipY`.
- **Text** must be null-terminated (`Wrecks::cstring_view`): pass string literals or
  `std::string`, never a `substr()` view.
- **`ActionMap::bindings()`** returns a reference that `bind`, `unbind` and `clear`
  invalidate.

## Working on the render module itself

- raylib.h must only be included in `.cpp` files and private headers, **after**
  every render header.
  raylib `#define`s `WHITE`, `RED`... which clash with `RType::Render::Colors`. Inside
  `namespace RType::Render`, write raylib types as `::Color`, `::Vector2`.
- Avoid names that are Windows macros in public headers (`TRANSPARENT`,
  `DrawText`, `ERROR`...). `windows.h` gets included through Wrecks' networking
  headers on Windows.
- raylib logs go through a callback (`src/render/Log.hpp`). It also stops raylib from
  calling `exit()` on fatal errors, so failures surface as `RType::Render::Error`.

## Dependencies

| Dependency | Provided by |
|---|---|
| raylib, Wrecks | git submodules (`lib/raylib`, `lib/wrecks`) |
| GoogleTest | vcpkg (`lib/vcpkg` submodule, `tests` feature of `vcpkg.json`), DebugTests builds only |
| OpenGL, X11 | the system |

vcpkg needs no installation: the first `cmake` bootstraps it from the submodule and
installs the manifest dependencies (cached in `~/.cache/vcpkg` afterwards).

OpenGL and X11 stay system libraries, as the subject allows for low-level
platform libraries. OpenGL is the graphics driver itself, and raylib's GLFW loads
the system X11 libraries at runtime anyway, so vcpkg could only provide headers
(and building them needs autotools on the system). On Linux, install the
development headers once:

```sh
sudo dnf install mesa-libGL-devel libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel   # Fedora
sudo apt install libgl1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev              # Ubuntu
```

## Build and test

```sh
git submodule update --init

# Client and render library
cmake -S . -B build && cmake --build build --parallel

# Unit tests (headless: no window, no audio device)
cmake -DCMAKE_BUILD_TYPE=DebugTests -S . -B build-tests
cmake --build build-tests --parallel
ctest --test-dir build-tests --output-on-failure

# Manual test program (window, rendering, input, audio)
cmake --build build --target render_manual_test
./build/tests/render/manual/render_manual_test <texture> [sound] [music] [font]
```

The manual test shows a scrolling background, a sprite moved with the arrows or a
gamepad, the letterbox frame and the mouse position. Space plays the sound, M
toggles the music, +/- changes the volume, F11 toggles fullscreen and Escape
quits.
