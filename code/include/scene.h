#ifndef SCENE_H
#define SCENE_H

#define MAX_MODEL_NUMBER 256
#define MAX_LIGHT_NUMBER 256
#define MAX_ENTITY_NUMBER 256

struct vertex {
    v3 Position;
    v2 Texture;
    v3 Normal;
};

struct mesh {
    u32 MeshID;
    vertex *VertexData;
    u32    NumberOfVertices;
    u32    *IndexData;
    u32    NumberOfIndices;
};

struct transforms {
    m4x4 Translation;
    m4x4 Scaling;
    m4x4 Rotation;
};

struct buffers {
    u32 VertexArrayObject     = 0;
    u32 VertexPositionBuffer  = 0;
    u32 TexturePositionBuffer = 0;
    u32 NormalPositionBuffer  = 0;
    u32 ElementBuffer         = 0;
};

struct directional_light_source {
    mesh *Mesh;
    shader *Shader;
    v3 Position;
    v3 Target;   //Must be normalized
    v3 Irradiance;
    buffers Buffers;
};

struct directional_light_array {
    directional_light_source *Data[MAX_LIGHT_NUMBER];
    u32 Length = 0;
};

struct entity {
    u32     EntityID;
    mesh    *Mesh;
    shader  *Shader;
    transforms Transforms;
    buffers Buffers;
};

struct entity_array {
    entity *Data[MAX_ENTITY_NUMBER];
    u32 Length = 0;
};

struct scene {
    entity_array             EntityArray;
    directional_light_array  LightArray;
};

global_variable directional_light_source LightSource = {};
global_variable entity Entity = {};
global_variable entity Entity2 = {};
global_variable scene Scene = {};

#endif