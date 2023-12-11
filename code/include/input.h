#ifndef INPUT_H
#define INPUT_H

struct cursor_position {
    v2 Current;
    v2 Previous;
};

struct mouse_cursor {
    cursor_position Position;
    f32 Sensitivity = 0.15;
    v2 Delta;
};

struct key_state {
    u32 KeyValue;
    b32 IsDown = 0;
};

struct key_input {
    key_state MoveCameraForward;
    key_state MoveCameraBackward;
    key_state MoveCameraLeft;
    key_state MoveCameraRight;
    key_state EnableCameraRotate;
};

global_variable key_input KeyInput;
global_variable mouse_cursor MouseCursor;

#endif  INPUT_H