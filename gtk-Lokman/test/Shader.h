#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gtk/gtk.h>
#include <epoxy/gl.h>

typedef struct
{
    float Position[3];
} VS_Input;

typedef struct
{
    char* VertexShader;
    char* PixelShader;

    VS_Input* Vertices;
    int VerticesCount;

    int* Indices;
    int IndicesCount;

    float UpdateSpeed;

    // No touching v v v

    GtkWindow* Window;

    GLuint Program;
    GLuint VertexBuffer, IndexBuffer;
    GLuint VertexArray;

    float Time;
    int Frame;
} ShaderData;

void ShaderData_Init(ShaderData* Data);

void ShaderData_Default(ShaderData* Data);

GtkWidget* Shader_Create(ShaderData* Data, GtkWindow* Window);
