# Backend

`rtype_backend` is the client's platform layer: window, 2D rendering, input and
audio, implemented with [raylib](https://www.raylib.com/) but exposed through our
own API. Game code includes `<backend.hpp>` and never `raylib.h` (raylib is linked
`PRIVATE`, so its headers are not even reachable).

The library knows nothing about R-Type or the ECS, so it can later move into the
engine (Wrecks) as an optional module.

## Layout

```
backend/
  include/backend.hpp        aggregate header (the only include game code needs)
  include/backend/*.hpp      public API: the contract
  src/<Class>/<Class>.cpp    raylib implementation (the only place raylib.h appears)
  tests/                     headless unit tests (GoogleTest, DebugTests build)
  manual-test/main.cpp       manual test program (needs a display)
```

## Quick start

```cpp
#include <backend.hpp>
#include <chrono>

int main()
{
    Backend::Window window({.title = "R-Type", .virtualWidth = 1920, .virtualHeight = 1080});
    Backend::Resources resources(window);   // after Window, destroyed before it
    Backend::Renderer renderer(resources);
    Backend::Input input(window);
    Backend::Audio audio;                   // independent from Window

    Backend::TextureId ship = resources.loadTexture("assets/ship.png");
    Backend::Vec2 position = {200.0F, 540.0F};

    auto previous = std::chrono::steady_clock::now();
    while (!window.shouldClose()) {
        auto now = std::chrono::steady_clock::now();
        float delta = std::chrono::duration<float>(now - previous).count();
        previous = now;

        if (input.isDown(Backend::Key::Right))
            position.x += 500.0F * delta;     // speed in virtual pixels per second
        if (input.isPressed(Backend::Key::Escape))
            window.close();
        audio.update();

        window.beginFrame(Backend::Colors::BLACK);
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
struct Sprite {                       // an ECS component
    Backend::TextureId texture;
    Backend::Rect frame;              // sprite-sheet frame
    Backend::Color tint;              // e.g. one color per player
};

// In a render system:
renderer.drawSprite(sprite.texture, {.position = {transform.x, transform.y},
                                     .source = sprite.frame, .tint = sprite.tint});
```

Never put a raylib object (or anything that unloads itself in a destructor) in a
component: components are moved and copied around by their containers.

### Timing

The backend never paces time (there is no FPS cap). The game loop measures time
itself with `std::chrono::steady_clock` and moves things by `speed * delta`, so
gameplay does not depend on the frame rate (a subject requirement).

### Controls and remapping

`ActionMap<A>` binds game actions (any enum) to keys, mouse buttons, gamepad
buttons and gamepad axes. Bindings can be changed at runtime, for a remapping menu
(accessibility). `Input::nextPressed()` and `keyName()` help build that menu.

```cpp
enum class Action { Up, Shoot };

Backend::ActionMap<Action> controls;
controls.bind(Action::Up, Backend::Key::Up);
controls.bind(Action::Up, Backend::AxisBinding{Backend::GamepadAxis::LeftY, -0.5F});
controls.bind(Action::Shoot, Backend::Key::Space);
controls.bind(Action::Shoot, Backend::GamepadButton::A);

if (controls.isPressed(Action::Shoot, input))
    audio.play(shootSound);
```

`ActionMap` only needs something satisfying the `InputBackend` concept, so it can
be unit-tested with a fake input. `RendererBackend` and `AudioBackend` do the same
for systems that draw or play sounds.

## Rules to know

- **One `Window` and one `Audio` at a time.** Creating a second one throws
  `Backend::Error`.
- **Handles:** valid ids start at 1, and `0` is `Invalid`. `FontId::Default` (0) is
  the built-in font. Unknown or unloaded handles are ignored: nothing is drawn and
  nothing is played.
- **Loading** a missing or unsupported file throws `Backend::Error`. Loading the
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

## Working on the backend itself

- raylib.h must only be included in `src/` files, **after** every backend header.
  raylib `#define`s `WHITE`, `RED`... which clash with `Backend::Colors`. Inside
  `namespace Backend`, write raylib types as `::Color`, `::Vector2`.
- Avoid names that are Windows macros in public headers (`TRANSPARENT`,
  `DrawText`, `ERROR`...). `windows.h` gets included through Wrecks' networking
  headers on Windows.
- raylib logs go through a callback (`src/Log.hpp`). It also stops raylib from
  calling `exit()` on fatal errors, so failures surface as `Backend::Error`.

## Build and test

```sh
# Library only
cmake -S . -B build && cmake --build build --parallel

# Unit tests (headless: no window, no audio device)
cmake -DCMAKE_BUILD_TYPE=DebugTests -S . -B build-tests
cmake --build build-tests --parallel
ctest --test-dir build-tests -R '^backend_' --output-on-failure

# Manual test program (window, rendering, input, audio)
cmake --build build --target backend_manual_test
./build/backend/manual-test/backend_manual_test <texture> [sound] [music] [font]
```

The manual test shows a scrolling background, a sprite moved with the arrows or a
gamepad, the letterbox frame and the mouse position. Space plays the sound, M
toggles the music, +/- changes the volume, F11 toggles fullscreen and Escape
quits.
