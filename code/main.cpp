#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <GL\glew.h>
#include <GL\wglew.h>
//#include <GL\wglext.h>
#include <types.h>
#include <stdio.h>
#include <math.h>
#include <ourmath.h>
#include <auxiliary.cpp>
#include <platform.cpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <shaders.cpp>
#include <camera.cpp>
#include <opengl.cpp>
#include <scene.cpp>
#include <input.cpp>
#include <objectloader.cpp>

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "Gdi32.lib")
#pragma comment (lib, "user32.lib")
#pragma comment (lib, "glu32.lib")

v3 LightCubePosition = {0.0, 0.0, 0.0};

v3 LightColor = {1.0, 1.0, 1.0};
v3 ToyColor   = {1.0, 0.5, 0.31};
v3 Result     = LightColor * ToyColor;

void InstantiateEntity(entity *Entity) {
    glUseProgram(Entity->Shader->ShaderID);
    //Setting up the various buffers
    glGenVertexArrays(1, &Entity->Buffers.VertexArrayObject);
    glGenBuffers(1, &Entity->Buffers.VertexPositionBuffer);
    glGenBuffers(1, &Entity->Buffers.TexturePositionBuffer);
    glGenBuffers(1, &Entity->Buffers.NormalPositionBuffer);
    glGenBuffers(1, &Entity->Buffers.ElementBuffer);
    //-------------------------------------------
    //Binding the buffers and sending the vertex data
    glBindVertexArray(Entity->Buffers.VertexArrayObject);
    //Vertex Positions, Normals and Texture Coordinates
    glBindBuffer(GL_ARRAY_BUFFER, Entity->Buffers.VertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, Entity->Mesh->NumberOfVertices * sizeof(vertex), Entity->Mesh->VertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, Entity->Buffers.NormalPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, Entity->Mesh->NumberOfVertices * sizeof(vertex), Entity->Mesh->VertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) (sizeof(v3) + sizeof(v2)));
    glEnableVertexAttribArray(1);
   
    glBindBuffer(GL_ARRAY_BUFFER, Entity->Buffers.TexturePositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, Entity->Mesh->NumberOfVertices * sizeof(vertex), Entity->Mesh->VertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) sizeof(v3));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Entity->Buffers.ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Entity->Mesh->NumberOfIndices * sizeof(u32), Entity->Mesh->IndexData, GL_STATIC_DRAW);
}

void InstantiateLightSource(directional_light_source *LightSource) {
    glUseProgram(LightSource->Shader->ShaderID);
    //Setting up the various buffers
    glGenVertexArrays(1, &LightSource->Buffers.VertexArrayObject);
    glGenBuffers(1, &LightSource->Buffers.VertexPositionBuffer);
    glGenBuffers(1, &LightSource->Buffers.TexturePositionBuffer);
    glGenBuffers(1, &LightSource->Buffers.NormalPositionBuffer);
    glGenBuffers(1, &LightSource->Buffers.ElementBuffer);
    //-------------------------------------------
    //Binding the buffers and sending the vertex data
    glBindVertexArray(LightSource->Buffers.VertexArrayObject);
    //Vertex Positions, Normals and Texture Coordinates
    glBindBuffer(GL_ARRAY_BUFFER, LightSource->Buffers.VertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, LightSource->Mesh->NumberOfVertices * sizeof(vertex), LightSource->Mesh->VertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, LightSource->Buffers.NormalPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, LightSource->Mesh->NumberOfVertices * sizeof(vertex), LightSource->Mesh->VertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) (sizeof(v3) + sizeof(v2)));
    glEnableVertexAttribArray(1);
   
    glBindBuffer(GL_ARRAY_BUFFER, LightSource->Buffers.TexturePositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, LightSource->Mesh->NumberOfVertices * sizeof(vertex), LightSource->Mesh->VertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) sizeof(v3));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LightSource->Buffers.ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, LightSource->Mesh->NumberOfIndices * sizeof(u32), LightSource->Mesh->IndexData, GL_STATIC_DRAW);
}

void LoadScene(scene *InputScene) {
    for(int i = 0; i < InputScene->EntityArray.Length; i++) {
        InstantiateEntity(InputScene->EntityArray.Data[i]);
    }
    InstantiateLightSource(InputScene->LightArray.Data[0]);
}

void DrawEntity(entity *InputEntity,
                m4x4 *PerspectiveProjectionMatrix,
                m4x4 *ViewMatrix) {
    
    m4x4 ModelMatrix  = InputEntity->Transforms.Translation * (InputEntity->Transforms.Rotation * InputEntity->Transforms.Scaling);
    m4x4 PVMMatrix    = *PerspectiveProjectionMatrix * (*ViewMatrix * ModelMatrix);
    m3x3 NormalMatrix = Transpose(Inverse(GetUpperLeftMatrix(ModelMatrix)));
    m4x4SetUniform(InputEntity->Shader, MODEL_UNIFORM, &ModelMatrix);
    m4x4SetUniform(InputEntity->Shader, PROJECTION_VIEW_MODEL_UNIFORM, &PVMMatrix);
    m3x3SetUniform(InputEntity->Shader, NORMAL_UNIFORM, &NormalMatrix);
    
    glBindVertexArray(InputEntity->Buffers.VertexArrayObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, InputEntity->Buffers.ElementBuffer);
    glDrawElements(GL_TRIANGLES, InputEntity->Mesh->NumberOfIndices, GL_UNSIGNED_INT, 0);
}

void DrawLightSource(directional_light_source *InputLightSource,
                     m4x4 *PerspectiveProjectionMatrix,
                     m4x4 *ViewMatrix) {
    glUseProgram(InputLightSource->Shader->ShaderID);
    
    m4x4 ModelMatrix  = Translation4x4(InputLightSource->Position);
    m4x4 PVMMatrix    = *PerspectiveProjectionMatrix * (*ViewMatrix * ModelMatrix);
    m4x4SetUniform(InputLightSource->Shader, PROJECTION_VIEW_MODEL_UNIFORM, &PVMMatrix);
    
    glBindVertexArray(InputLightSource->Buffers.VertexArrayObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, InputLightSource->Buffers.ElementBuffer);
    glDrawElements(GL_TRIANGLES, InputLightSource->Mesh->NumberOfIndices, GL_UNSIGNED_INT, 0);
}

void DrawScene(scene *InputScene) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m4x4 ViewMatrix = GenerateViewMatrix(Camera.CameraPosition,
                                         Camera.CameraFront,
                                         Camera.CameraUp);
    m4x4 PerspectiveProjectionMatrix = GeneratePerspectiveMatrix(Camera.FieldOfView,
                                                                 Camera.AspectRatio,
                                                                 Camera.NearPlane,
                                                                 Camera.FarPlane);
                                                                 
    //-------------------------------------------
    //Setting up the object cube uniforms  
    
    
    //-------------------------------------------
    for(int i = 0; i < InputScene->LightArray.Length; i++) {
        DrawLightSource(InputScene->LightArray.Data[i], 
                        &PerspectiveProjectionMatrix,
                        &ViewMatrix);
    }
    
    for(int i = 0; i < InputScene->EntityArray.Length; i++) {
        glUseProgram(InputScene->EntityArray.Data[i]->Shader->ShaderID);
        v3 ColorsToSend = {0.5, 0.5, 0.3};
        v3 DirLightSourceDirection = -Normalize(InputScene->LightArray.Data[0]->Target -
                                                InputScene->LightArray.Data[0]->Position);                                            
        v3SetUniform(&ShaderArray[0], OBJECT_COLORS_UNIFORM, &ColorsToSend);
        v3SetUniform(&ShaderArray[0], LIGHT_COLORS_UNIFORM, &InputScene->LightArray.Data[0]->Irradiance);
        v3SetUniform(&ShaderArray[0], LIGHT_POSITION_UNIFORM, &InputScene->LightArray.Data[0]->Position);
        v3SetUniform(&ShaderArray[0], VIEW_POSITION_UNIFORM, &Camera.CameraPosition);
        v3SetUniform(&ShaderArray[0], LIGHT_DIRECTION_UNIFORM, &DirLightSourceDirection);
        DrawEntity(InputScene->EntityArray.Data[i],
                   &PerspectiveProjectionMatrix,
                   &ViewMatrix);
    }
        
    return;
}

LRESULT CALLBACK
Win32MainWindowCallbackProcedure(HWND Window,
                                 UINT uMsg,
                                 WPARAM wParam,
                                 LPARAM lParam) {
    u32 KeyInput;
    PAINTSTRUCT Paint;
    switch (uMsg) {
        case WM_CREATE:
            WindowDeviceContext = GetDC(Window);
            
            PIXELFORMATDESCRIPTOR pfd;
            int PixelFormat;
            Win32PixelFormatSetup(&pfd, &PixelFormat);
            
            if(InitializeOpenGL() == 0) return 0;
            
            break;
        
        case WM_DESTROY: 
            PostQuitMessage(0);
            return 0;
        
        case WM_SIZE:
            ResizeOpenGL(Window);
            break;

        case WM_PAINT:
            BeginPaint(Window, &Paint);
            EndPaint(Window, &Paint);
            break;

        case WM_CLOSE:
            wglDeleteContext(RenderingContextHandle);
            ReleaseDC(Window, WindowDeviceContext);
            break;
            
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_MOUSEMOVE:
            break;
    }
    return DefWindowProc(Window, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nShowCmd) {
    
    LARGE_INTEGER PerformanceCountFrequencyResult;
    QueryPerformanceFrequency(&PerformanceCountFrequencyResult);
    GlobalTime.PerfCountFreq = PerformanceCountFrequencyResult.QuadPart;
    
    GetLocalTime(&GlobalTime.LocalTime);
    Win32WriteToLogFile("Time: %02d:%02d:%02d\n", GlobalTime.LocalTime.wHour,
                                             GlobalTime.LocalTime.wMinute,
                                             GlobalTime.LocalTime.wSecond);    
    
    //Register window class
    WNDCLASS WindowClass = {};
    WindowClass.style         = CS_OWNDC;
    WindowClass.lpfnWndProc   = Win32MainWindowCallbackProcedure;
    WindowClass.hInstance     = hInstance;
    WindowClass.lpszClassName = L"OpenGL";
    RegisterClass(&WindowClass);
    
    WindowDimensions.Width  = 800;
    WindowDimensions.Height = 600;
    
        //Create the window
    HWND WindowHandle = CreateWindowEx(0,
                                       L"OpenGL",
                                       L"OpenGL",
                                       WS_OVERLAPPEDWINDOW,
                                       0, 100,
                                       WindowDimensions.Width,
                                       WindowDimensions.Height,
                                       NULL,
                                       NULL,
                                       hInstance,
                                       NULL);
    if(WindowHandle == NULL) return 0;
    
    ShowWindow(WindowHandle, nShowCmd);
    UpdateWindow(WindowHandle); 
    
    LoadAndCompileShader(&ShaderArray[0], "shaders/VertexShader1.vs", "shaders/FragmentShader1.fs");
    LoadAndCompileShader(&ShaderArray[1], "shaders/VertexShader2.vs", "shaders/FragmentShader2.fs");
        
    MSG Message = {};
    
    Win32WriteToLogFile("PCF: %d - Year: %d\n", GlobalTime.PerfCountFreq, GlobalTime.LocalTime.wYear, GlobalTime);
    
    LARGE_INTEGER LastCounter;
    QueryPerformanceCounter(&LastCounter);
    
    InitializeCamera();
    SetKeyboardInputValues();
    
    mesh *LoadedMesh = ReadRMF("cube.rmf");
    
    LightSource.Mesh = LoadedMesh;
    LightSource.Shader = &ShaderArray[1];
    LightSource.Position   = {-4.0, 5.0, 5.0};
    LightSource.Target     = {0.0, 0.0, 0.0};
    LightSource.Irradiance = {1.0, 1.0, 1.0};
    
    Scene.LightArray.Data[0] = &LightSource;
    Scene.LightArray.Length++;
    
    Entity.EntityID = 1;
    Entity.Mesh = LoadedMesh;
    Entity.Shader = &ShaderArray[0];
    Entity.Transforms.Translation = Translation4x4({1.0, -1.0, 5.0});
    Entity.Transforms.Scaling   = Scaling4x4(1);
    Entity.Transforms.Rotation  = Rotation4x4X(PI / 2);
    
    Entity2.EntityID = 2;
    Entity2.Mesh = LoadedMesh;
    Entity2.Shader = &ShaderArray[0];
    Entity2.Transforms.Translation = Translation4x4({5.0, -2.0, 8.0});
    Entity2.Transforms.Scaling   = Scaling4x4(0.5);
    Entity2.Transforms.Rotation  = Rotation4x4Y(PI / 4) * Rotation4x4X(PI / 4);
    
    Scene.EntityArray.Data[0] = &Entity;
    Scene.EntityArray.Length++;
    Scene.EntityArray.Data[1] = &Entity2;
    Scene.EntityArray.Length++;
    
    LoadScene(&Scene);
    
    while (1) { 
        /* 
         *  Process all pending messages 
         */ 
       while(PeekMessage(&Message, NULL, 0, 0, PM_NOREMOVE) == TRUE) {
            if((Message.message == WM_KEYDOWN) || (Message.message == WM_KEYUP)) {
                GetMessage(&Message, NULL, 0, 0);
                TranslateMessage(&Message);
                Win32GetKeyboardInput(Message.message, Message.wParam);
            }
            else if(((Message.message == WM_MOUSEMOVE) || (Message.message == WM_MBUTTONDOWN) || (Message.message == WM_MBUTTONUP))) {
                GetMessage(&Message, NULL, 0, 0);
                TranslateMessage(&Message);
                Win32GetMouseInput(Message.message, Message.lParam);
            }
            else if (GetMessage(&Message, NULL, 0, 0)) 
            { 
                TranslateMessage(&Message); 
                DispatchMessage(&Message); 
            } else { 
                return TRUE; 
            } 
        }
        
        Win32ProcessKeyboardInput();
        Win32ProcessMouseInput();
        
        //NOTE: Do the scene draw call here.
        DrawScene(&Scene);
        SwapBuffers(WindowDeviceContext);
        
        GetLocalTime(&GlobalTime.LocalTime);        
        LARGE_INTEGER EndCounter;
        QueryPerformanceCounter(&EndCounter);
        GlobalTime.CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;
        LastCounter = EndCounter;
    }    
    return 0;
}
