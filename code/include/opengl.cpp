#ifndef OPENGL_CPP
#define OPENGL_CPP

b32 InitializeOpenGL() {
    RenderingContextHandle = wglCreateContext(WindowDeviceContext);
    wglMakeCurrent(WindowDeviceContext, RenderingContextHandle);
    GLenum err;
    err = glewInit();
    if (GLEW_OK != err) return 0;
    glViewport(0, 0, WindowDimensions.Width, WindowDimensions.Height);
    
    GLfloat AspectRatio = (GLfloat) (WindowDimensions.Width / WindowDimensions.Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(-AspectRatio, AspectRatio, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    return 1;
}

void ResizeOpenGL(HWND Window) {
    RECT ClientRect;
    ClientRect = {};
    GetClientRect(Window, &ClientRect);
    WindowDimensions.Width = (int)ClientRect.right - (int)ClientRect.left;
    WindowDimensions.Height = (int)ClientRect.bottom - (int)ClientRect.top;
    glViewport(0, 0, WindowDimensions.Width, WindowDimensions.Height);
    
    GLfloat AspectRatio;
    AspectRatio = (GLfloat) (WindowDimensions.Width / WindowDimensions.Height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, AspectRatio, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    
    return;
}


inline m4x4 GeneratePerspectiveMatrix(f32 FieldOfViewY,
                                      f32 AspectRatio,
                                      f32 NearPlane,
                                      f32 FarPlane) {
    f32 Tangent = tan(FieldOfViewY * 0.5 * DEGTORAD); 
    f32 Top     = NearPlane * Tangent;
    f32 Right   = Top * AspectRatio;
        
    m4x4 PerspectiveProjectionMatrix    = Identity4x4();
    PerspectiveProjectionMatrix.E[0][0] = NearPlane / Right;
    PerspectiveProjectionMatrix.E[1][1] = NearPlane / Top;
    PerspectiveProjectionMatrix.E[2][2] = (FarPlane + NearPlane) / (NearPlane - FarPlane);
    PerspectiveProjectionMatrix.E[2][3] = - 2.0 * FarPlane * NearPlane / (FarPlane - NearPlane);
    PerspectiveProjectionMatrix.E[3][3] = 0.0;
    PerspectiveProjectionMatrix.E[3][2] = -1.0;

    return PerspectiveProjectionMatrix;
}

inline m4x4 GenerateOrthographicMatrix(f32 AspectRatio,
                                       f32 NearPlane,
                                       f32 FarPlane) {
    f32 Top   = 1.0;
    f32 Right = AspectRatio * Top;
    
    m4x4 OrthographicProjectionMatrix = Identity4x4();
    OrthographicProjectionMatrix.E[0][0] = 1.0 / Right;
    OrthographicProjectionMatrix.E[1][1] = 1.0 / Top;
    OrthographicProjectionMatrix.E[2][2] = -2 / (FarPlane - NearPlane);
    OrthographicProjectionMatrix.E[2][3] = (FarPlane + NearPlane) / (NearPlane - FarPlane);
    
    return OrthographicProjectionMatrix;
}

// The following function genenerates the matrix of transition from the world space coordinates
// to camera space coordinates, combined with a translation matrix.
inline m4x4 GenerateViewMatrix(v3 CameraPosition, v3 CameraFront, v3 WorldUpVector) {
    v3 CameraDirection = Normalize(-CameraFront);
    v3 CameraRight     = Normalize(CrossProduct(WorldUpVector, CameraDirection));
    v3 CameraUp        = Normalize(CrossProduct(CameraDirection, CameraRight));
    
    Win32OutputToDebugConsole("CameraPosition: (%f, %f, %f)\nCameraDirection: (%f, %f, %f)\nCameraFront: (%f, %f, %f)\nCameraRight: (%f, %f, %f)\nCameraUp: (%f, %f, %f)\n-----\n",
                              CameraPosition.x, CameraPosition.y, CameraPosition.z,
                              CameraDirection.x, CameraDirection.y, CameraDirection.z,
                              CameraFront.x, CameraFront.y, CameraFront.z,
                              CameraRight.x, CameraRight.y, CameraRight.z,
                              CameraUp.x, CameraUp.y, CameraUp.z);
                                  
    m4x4 ViewMatrix = Identity4x4();
    ViewMatrix.E[0][0] = CameraRight.x;
    ViewMatrix.E[0][1] = CameraRight.y;
    ViewMatrix.E[0][2] = CameraRight.z;
    ViewMatrix.E[1][0] = CameraUp.x;
    ViewMatrix.E[1][1] = CameraUp.y;
    ViewMatrix.E[1][2] = CameraUp.z;
    ViewMatrix.E[2][0] = CameraDirection.x;
    ViewMatrix.E[2][1] = CameraDirection.y;
    ViewMatrix.E[2][2] = CameraDirection.z;
    ViewMatrix.E[0][3] = DotProduct(-CameraPosition, CameraRight);
    ViewMatrix.E[1][3] = DotProduct(-CameraPosition, CameraUp);
    ViewMatrix.E[2][3] = DotProduct(-CameraPosition, CameraDirection);
    
    return ViewMatrix;
};

#endif OPENGL_CPP