#ifndef INPUT_CPP
#define INPUT_CPP

#include <input.h>

void SetKeyboardInputValues() {
    KeyInput.MoveCameraForward.KeyValue  = 'W';
    KeyInput.MoveCameraBackward.KeyValue = 'S';
    KeyInput.MoveCameraLeft.KeyValue     = 'A';
    KeyInput.MoveCameraRight.KeyValue    = 'D';
    KeyInput.EnableCameraRotate.KeyValue = MK_MBUTTON;
    return;
};

void Win32GetKeyboardInput(u32 InputType, LPARAM KeyValue) {
    if(InputType == WM_KEYDOWN) {
        if(KeyValue == KeyInput.MoveCameraForward.KeyValue)
        KeyInput.MoveCameraForward.IsDown  = 1;
        if(KeyValue == KeyInput.MoveCameraBackward.KeyValue)
        KeyInput.MoveCameraBackward.IsDown = 1;
        if(KeyValue == KeyInput.MoveCameraLeft.KeyValue)
        KeyInput.MoveCameraLeft.IsDown     = 1;
        if(KeyValue == KeyInput.MoveCameraRight.KeyValue)
        KeyInput.MoveCameraRight.IsDown    = 1;
    }
    else if(InputType == WM_KEYUP) {
        if(KeyValue == KeyInput.MoveCameraForward.KeyValue)
        KeyInput.MoveCameraForward.IsDown  = 0;
        if(KeyValue == KeyInput.MoveCameraBackward.KeyValue)
        KeyInput.MoveCameraBackward.IsDown = 0;
        if(KeyValue == KeyInput.MoveCameraLeft.KeyValue)
        KeyInput.MoveCameraLeft.IsDown     = 0;
        if(KeyValue == KeyInput.MoveCameraRight.KeyValue)
        KeyInput.MoveCameraRight.IsDown    = 0;
    }
}

void Win32ProcessKeyboardInput() {
    if(KeyInput.MoveCameraForward.IsDown == 1)
        Camera.CameraPosition = Camera.CameraPosition + 0.1 * Camera.CameraFront;
    if(KeyInput.MoveCameraBackward.IsDown == 1)
        Camera.CameraPosition = Camera.CameraPosition - 0.1 * Camera.CameraFront;
    if(KeyInput.MoveCameraLeft.IsDown == 1)
        Camera.CameraPosition = Camera.CameraPosition - 0.1 * Normalize(CrossProduct(Camera.CameraFront, Camera.CameraUp));
    if(KeyInput.MoveCameraRight.IsDown == 1)
        Camera.CameraPosition = Camera.CameraPosition + 0.1 * Normalize(CrossProduct(Camera.CameraFront, Camera.CameraUp));
    return;
}

void Win32GetMouseInput(u32 ButtonPress, LPARAM Message) {
    if((ButtonPress == WM_MBUTTONDOWN) && (Message = MK_MBUTTON))
        KeyInput.EnableCameraRotate.IsDown = 1;
    else if((ButtonPress == WM_MBUTTONUP) && (Message = MK_MBUTTON))
        KeyInput.EnableCameraRotate.IsDown = 0;
    else {
        MouseCursor.Position.Current.x = (f32)MAKEPOINTS(Message).x;
        MouseCursor.Position.Current.y = -(f32)MAKEPOINTS(Message).y;
        MouseCursor.Delta = MouseCursor.Position.Previous - MouseCursor.Position.Current;
        // Win32OutputToDebugConsole("Current X: %f, Current Y: %f\nPrevious X: %f, Previous Y: %f\nDelta X: %f, Delta Y: %f\n-----\n",
        //                            MouseCursor.Position.Current.x,  MouseCursor.Position.Current.y,
        //                            MouseCursor.Position.Previous.x, MouseCursor.Position.Previous.y,
        //                            MouseCursor.Delta.x,             MouseCursor.Delta.y);
        MouseCursor.Position.Previous = MouseCursor.Position.Current;
    }
    return;
}

void Win32ProcessMouseInput() {
    if(KeyInput.EnableCameraRotate.IsDown) {
        Camera.Yaw += MouseCursor.Sensitivity * MouseCursor.Delta.x;
        Camera.Pitch += MouseCursor.Sensitivity * MouseCursor.Delta.y;
        MouseCursor.Delta = {0, 0};
        if(Camera.Pitch > 89.0) Camera.Pitch = 89.0;
        if(Camera.Pitch < -89.0) Camera.Pitch = -89.0;
        Camera.CameraFront = Normalize({cosf(Camera.Yaw * DEGTORAD) * cosf(Camera.Pitch * DEGTORAD),
                                        sinf(Camera.Pitch * DEGTORAD),
                                        sinf(Camera.Yaw * DEGTORAD) * cosf(Camera.Pitch * DEGTORAD)});
    }                                
    return;
}

#endif  INPUT_CPP