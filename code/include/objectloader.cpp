#ifndef OBJECTLOADER_CPP
#define OBJECTLOADER_CPP

inline f32 GetVertexValue(char *Input) {
    char TempValue[20];
    u32 i = 0;
    while(*Input != ' ' && *Input != '\n' && *Input != '\0') {
        TempValue[i] = *Input++;
        i++;
    }
    TempValue[i] = '\0';
    return atof(TempValue);
}

inline u32 GetIndexValue(char *Input) {
    char TempValue[10];
    u32 i = 0;
    while(*Input != ' ' && *Input != '\n' && *Input != '\0') {
        TempValue[i] = *Input++;
        i++;
    }
    TempValue[i] = '\0';
    return strtol(TempValue, '\0', 10);
}

inline char* SkipToBlankSpace(char *Input) {
    while(*Input != ' ' && *Input != '\n' && *Input != '\0') Input++;
    return Input;
}

mesh* ReadRMF(char *InputFileName) {
    mesh *Output = (mesh *)malloc(sizeof(mesh));
    void *ReadData = Win32ReadEntireFile(InputFileName);
    char *CurrentChar = (char *)ReadData;
    u32 NumberOfVertices = 0;
    u32 NumberOfIndices  = 0;
    while(*CurrentChar != 'i') {
        if(*CurrentChar == 'v') NumberOfVertices++;
        CurrentChar++;
    }
    CurrentChar += 2;
    while(*CurrentChar != '\0') {
        if(*CurrentChar == ' ') NumberOfIndices++;
        CurrentChar++;
    }
    CurrentChar = (char *)ReadData;
    Output->VertexData = (vertex *)malloc(sizeof(vertex) * NumberOfVertices);
    Output->IndexData  = (u32 *)malloc(sizeof(u32) * NumberOfIndices);
    u32 VertexCount = 0;
    u32 IndexCount  = 0;
    while(1) {
        if(*CurrentChar == '\0')
            break;
        else if(CompareString(CurrentChar, 2, "v ", 2)) {
            vertex TempVertex = {};
            CurrentChar++;
            CurrentChar++;                 //Skip the blank spaces
            TempVertex.Position.x = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            CurrentChar++;
            TempVertex.Position.y = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            CurrentChar++;
            TempVertex.Position.z = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            CurrentChar += 3;
            TempVertex.Normal.x = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            CurrentChar++;
            TempVertex.Normal.y = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            CurrentChar++;
            TempVertex.Normal.z = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            CurrentChar += 3;
            TempVertex.Texture.x = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            CurrentChar++;
            TempVertex.Texture.y = GetVertexValue(CurrentChar);
            CurrentChar = SkipToBlankSpace(CurrentChar);
            Output->VertexData[VertexCount] = TempVertex;
            VertexCount++;
        }
        else if(CompareString(CurrentChar, 2, "i ", 2)) {
            CurrentChar += 2;
            while(*CurrentChar != '\0') {
                u32 TempIndex = GetIndexValue(CurrentChar);
                CurrentChar = SkipToBlankSpace(CurrentChar);
                Output->IndexData[IndexCount] = TempIndex;
                IndexCount++;
                CurrentChar++;
            }
        }
        else CurrentChar++;
    }
    Output->NumberOfVertices = VertexCount;
    Output->NumberOfIndices  = IndexCount;
    return Output;
}
#endif