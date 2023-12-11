#ifndef PLATFORM_CPP
#define PLATFORM_CPP

#include <platform.h>

void Win32PixelFormatSetup(PIXELFORMATDESCRIPTOR *pfd, int *PixelFormat) {
    *pfd = {
                sizeof(PIXELFORMATDESCRIPTOR),
                1,
                PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
                PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
                24,                   // Colordepth of the framebuffer.
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                32,                   // Number of bits for the depthbuffer
                0,                    // Number of bits for the stencilbuffer
                0,                    // Number of Aux buffers in the framebuffer.
                PFD_MAIN_PLANE,
                0, 0, 0, 0};
    *PixelFormat = ChoosePixelFormat(WindowDeviceContext, pfd);
    SetPixelFormat(WindowDeviceContext, *PixelFormat, pfd);
}

void Win32ReadShaderFile(char *DestinationBuffer, char *ShaderFileName) {
    HANDLE hFile = CreateFileA((LPCSTR)ShaderFileName, // name of the write
                              GENERIC_READ,                              // open for reading
                              NULL,                                      // do not share
                              NULL,                    // default security
                              OPEN_EXISTING,           // create new file only
                              FILE_ATTRIBUTE_READONLY, // normal file
                              NULL);                   // no attr. template
    DWORD BytesToRead = 1999;
    DWORD BytesRead = 0;
    ReadFile(hFile, DestinationBuffer, BytesToRead, &BytesRead, 0);
    DestinationBuffer[BytesRead] = '\0';    
    CloseHandle(hFile);
}

#endif PLATFORM_CPP