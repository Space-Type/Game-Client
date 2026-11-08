#pragma once


namespace RType::Components {
    /**
     * The ship of this client: the only one whose PlayerInput
     * comes from the keyboard or gamepad (Systems::PlayerInput).
     * The other ships keep the inputs the server shares.
     */
    struct Controllable {
        float acceleration = 4000.0f;  // virtual pixels per second, per second
    };
}
