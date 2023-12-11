#ifndef SHADERS_H
#define SHADERS_H

#define MAX_SHADER_NUMBER 16
#define MAX_ENTITY_NUMBER 256

global_variable char const *VertexShaderSourceConst;
global_variable char const *FragmentShaderSourceConst;
global_variable char VertexShaderSource[2000];
global_variable char FragmentShaderSource[2000];

//NOTE: Here we initialize enum values because the value of the first
//element depends on the compiler.
enum uniform {
    VIEW_UNIFORM                  = 0,
    PROJECTION_UNIFORM            = 1,
    MODEL_UNIFORM                 = 2,
    NORMAL_UNIFORM                = 3,
    PROJECTION_VIEW_MODEL_UNIFORM = 4,
    OBJECT_COLORS_UNIFORM         = 5,
    LIGHT_COLORS_UNIFORM          = 6,
    LIGHT_POSITION_UNIFORM        = 7,
    VIEW_POSITION_UNIFORM         = 8,
    LIGHT_DIRECTION_UNIFORM       = 9
};

global_variable const u32 NumberOfUniformElements = 10;

struct shader {
    u32 ShaderID;
    s32 Uniforms[NumberOfUniformElements] = {};
};

global_variable u32 ShaderCounter = 0;
global_variable shader ShaderArray[MAX_SHADER_NUMBER];

#endif SHADERS_H