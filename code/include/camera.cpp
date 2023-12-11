#ifndef CAMERA_CPP
#define CAMERA_CPP

#include <camera.h>

void InitializeCamera() {    
    Camera.CameraPosition = {0.0, 0.0, 10.0};
    Camera.CameraFront    = {0.0, 0.0, -1.0};
    Camera.CameraUp       = {0.0, 1.0, 0.0};
    
    Camera.FieldOfView = 45.0;
    Camera.AspectRatio = (f32)WindowDimensions.Width / (f32)WindowDimensions.Height;
    
    Camera.NearPlane = 0.1;
    Camera.FarPlane  = 100.0;
    
    Camera.Yaw   = -90.0;
    Camera.Pitch = 0.0;
    
    return;
}

#endif