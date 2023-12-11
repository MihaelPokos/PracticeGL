#ifndef CAMERA_H
#define CAMERA_H

struct camera {
    v3 CameraPosition;
    v3 CameraFront;
    v3 CameraUp;
    
    f32 FieldOfView;
    f32 AspectRatio;
    
    f32 NearPlane;
    f32 FarPlane;
    
    f32 Yaw; 
    f32 Pitch;
};

global_variable camera Camera;

#endif  CAMERA_H