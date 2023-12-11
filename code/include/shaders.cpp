#ifndef SHADERS_CPP
#define SHADERS_CPP

#include <shaders.h>

void GetUniform(shader *Shader, uniform UniformType, char *UniformName) {
    Shader->Uniforms[UniformType] = glGetUniformLocation(Shader->ShaderID, UniformName);
}

void m3x3SetUniform(shader *Shader, uniform UniformType, m3x3 *Target) {
    glUniformMatrix3fv(Shader->Uniforms[UniformType], 1, GL_TRUE, (f32 *)Target);
}

void m4x4SetUniform(shader *Shader, uniform UniformType, m4x4 *Target) {
    glUniformMatrix4fv(Shader->Uniforms[UniformType], 1, GL_TRUE, (f32 *)Target);
}

void v3SetUniform(shader *Shader, uniform UniformType, v3 *Target) {
    glUniform3f(Shader->Uniforms[UniformType], Target->x, Target->y, Target->z);
}

void GetUniform(shader *Shader, uniform UniformType) {
    switch (UniformType) {
        case VIEW_UNIFORM:
            GetUniform(Shader, UniformType, "ViewMatrix");
            break;
        case PROJECTION_UNIFORM:
            GetUniform(Shader, UniformType, "ProjectionMatrix");
            break;
        case MODEL_UNIFORM:
            GetUniform(Shader, UniformType, "ModelMatrix");
            break;
        case PROJECTION_VIEW_MODEL_UNIFORM:
            GetUniform(Shader, UniformType, "ProjectionViewModelMatrix");
            break;
        case NORMAL_UNIFORM:
            GetUniform(Shader, UniformType, "NormalMatrix");
            break;
        case OBJECT_COLORS_UNIFORM:
            GetUniform(Shader, UniformType, "ObjectColors");
            break;
        case LIGHT_COLORS_UNIFORM:
            GetUniform(Shader, UniformType, "LightColors");
            break;
        case LIGHT_POSITION_UNIFORM:
            GetUniform(Shader, UniformType, "LightPos");
            break;
        case VIEW_POSITION_UNIFORM:
            GetUniform(Shader, UniformType, "ViewPos");
            break;
        case LIGHT_DIRECTION_UNIFORM:
            GetUniform(Shader, UniformType, "LightDirection");
            break;
        default:
            break;
    }
}

void LoadAndCompileShader(shader *Shader,
                          char *VertexShaderLoc,
                          char *FragmentShaderLoc) {
    Win32ReadShaderFile(VertexShaderSource,   VertexShaderLoc);
    Win32ReadShaderFile(FragmentShaderSource, FragmentShaderLoc);
    VertexShaderSourceConst   = VertexShaderSource;
    FragmentShaderSourceConst = FragmentShaderSource;
    
    u32 VertexShader;
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VertexShaderSourceConst, NULL);
    glCompileShader(VertexShader);
    
    s32 VShaderCompilationStatus;
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &VShaderCompilationStatus);
    if(!VShaderCompilationStatus)
    {
        Win32WriteToLogFile("Error: Vertex shader (in shader %d) compilation returned an error.\n", &Shader->ShaderID);
        glGetShaderInfoLog(VertexShader, MAX_LOGBUFFER_STRING_LENGTH,
                           NULL, DebugInfo.GlobalLogBuffer);
        Win32WriteToLogFile();
    }
    
    u32 FragmentShader;
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSourceConst, NULL);
    glCompileShader(FragmentShader);
    
    s32 FShaderCompilationStatus;
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &FShaderCompilationStatus);
    if(!FShaderCompilationStatus)
    {
        Win32WriteToLogFile("Error: Fragment shader (in shader %d)  compilation returned an error.\n", &Shader->ShaderID);
        glGetShaderInfoLog(FragmentShader, MAX_LOGBUFFER_STRING_LENGTH,
                           NULL, DebugInfo.GlobalLogBuffer);
        Win32WriteToLogFile();
    }
    
    Shader->ShaderID = glCreateProgram();
    glAttachShader(Shader->ShaderID, VertexShader);
    glAttachShader(Shader->ShaderID, FragmentShader);
    glLinkProgram(Shader->ShaderID);
    
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    
    for (int UniformType = 0; UniformType < NumberOfUniformElements; UniformType++) {
        GetUniform(Shader, (uniform) UniformType);
    }
    return;
}


#endif  SHADERS_CPP