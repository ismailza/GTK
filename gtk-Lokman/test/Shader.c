#include "Shader.h"

void GLAPIENTRY MessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}

void OpenGlInitialiaze()
{
    static gboolean Initialized = FALSE;
    if (Initialized)
    {
        return;
    }
    Initialized = TRUE;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
}

static GLuint OpenGlCompileShader(const char* Source, GLuint ShaderType)
{
    FILE* File = fopen(Source, "rb");
    fseek(File, 0, SEEK_END);
    long Size = ftell(File);
    fseek(File, 0, SEEK_SET);

    GLchar* Text;

    if (ShaderType == GL_VERTEX_SHADER)
    {
        const char Predefine[] =
            "#version 410 core\n";

        Text = calloc(Size + sizeof(Predefine) + 1, sizeof(char));
        strcpy(Text, Predefine);

        fread(Text + sizeof(Predefine) - 1, sizeof(char), Size, File);
    }
    else if (ShaderType == GL_FRAGMENT_SHADER)
    {
        const char Predefine[] =
            "uniform float iTime;\n"
            "uniform int iFrame;\n"
            "uniform vec3 iResolution;\n";

        const char Postdefine[] =
            "void main()\n"
            "{\n"
            "    mainImage(gl_FragColor, gl_FragCoord.xy);\n"
            "}\n";

        Text = calloc(Size + sizeof(Predefine) + sizeof(Postdefine) + 1, sizeof(char));
        strcpy(Text, Predefine);

        fread(Text + sizeof(Predefine) - 1, sizeof(char), Size, File);

        // write Postdefine at the end
        strcpy(Text + sizeof(Predefine) - 1 + Size, Postdefine);
    }

    fclose(File);

    GLuint Shader = glCreateShader(ShaderType);
    glShaderSource(Shader, 1, &Text, NULL);
    glCompileShader(Shader);

    free(Text);
    return Shader;
}

//

void ShaderData_Init(ShaderData* Data)
{
    Data->VertexShader = NULL;
    Data->PixelShader = NULL;

    Data->Vertices = NULL;
    Data->VerticesCount = 0;

    Data->Indices = NULL;
    Data->IndicesCount = 0;

    Data->UpdateSpeed = 0.1f;
    Data->Frame = 0;
}

void ShaderData_Default(ShaderData* Data)
{
    static VS_Input Vertices[] = {
        { { -1.f, -1.f, 0.0f } },
        { { 1.f, -1.f, 0.0f } },
        { { 1.f, 1.f, 0.0f } },
        { { -1.f, 1.f, 0.0f } },
    };
    static int Indices[] = {
        0,
        1,
        2,
        0,
        2,
        3,
    };

    Data->VertexShader = NULL;
    Data->PixelShader = NULL;

    Data->Vertices = Vertices;
    Data->VerticesCount = 4;

    Data->Indices = Indices;
    Data->IndicesCount = 6;

    Data->UpdateSpeed = 0.1f;
    Data->Frame = 0;
}

static gboolean OnAreaRealise(GtkGLArea* Area, ShaderData* Data)
{
    gtk_gl_area_make_current(GTK_GL_AREA(Area));
    if (gtk_gl_area_get_error(GTK_GL_AREA(Area)) != NULL)
    {
        printf("Failed to initialiize buffers\n");
        return FALSE;
    }

    OpenGlInitialiaze();

    GLuint Vertex = OpenGlCompileShader(Data->VertexShader, GL_VERTEX_SHADER);
    GLuint Pixel = OpenGlCompileShader(Data->PixelShader, GL_FRAGMENT_SHADER);

    Data->Program = glCreateProgram();
    glAttachShader(Data->Program, Vertex);
    glAttachShader(Data->Program, Pixel);
    glLinkProgram(Data->Program);

    // Create vertex and index buffer for rendering

    glGenVertexArrays(1, &Data->VertexArray);
    glBindVertexArray(Data->VertexArray);

    glGenBuffers(1, &Data->VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, Data->VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VS_Input) * Data->VerticesCount, Data->Vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &Data->IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Data->IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * Data->IndicesCount, Data->Indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VS_Input), (void*)0);

    // glDeleteBuffers(1, &Data->VertexBuffer);

    return TRUE;
}

static gboolean OnAreaRender(GtkGLArea* area, GdkGLContext* Context, ShaderData* Data)
{
    glUseProgram(Data->Program);
    {
        GLint FrameLoc = glGetUniformLocation(Data->Program, "iFrame");
        Data->Frame++;
        glUniform1i(FrameLoc, Data->Frame);

        GLint TimeLoc = glGetUniformLocation(Data->Program, "iTime");
        Data->Time += Data->UpdateSpeed;
        glUniform1f(TimeLoc, Data->Time);

        GLint ResolutionLoc = glGetUniformLocation(Data->Program, "iResolution");

        int Width, Height;
        gtk_window_get_size(Data->Window, &Width, &Height);
        gtk_widget_set_size_request(GTK_WIDGET(area), Width, Height);
        glUniform3f(ResolutionLoc, (float)(Width), (float)(Height), 1.0f);
    }

    // Draw from index buffer after binding it

    glBindVertexArray(Data->VertexArray);
    glDrawElements(GL_TRIANGLES, Data->IndicesCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);

    glFlush();
    gtk_widget_queue_draw(GTK_WIDGET(area));

    return TRUE;
}

GtkWidget* Shader_Create(ShaderData* Data, GtkWindow* Window)
{
    GtkWidget* Area = gtk_gl_area_new();

    ShaderData* CopyData = malloc(sizeof(ShaderData));

    CopyData->VertexShader = malloc(strlen(Data->VertexShader) + 1);
    strcpy(CopyData->VertexShader, Data->VertexShader);

    CopyData->PixelShader = malloc(strlen(Data->PixelShader) + 1);
    strcpy(CopyData->PixelShader, Data->PixelShader);

    CopyData->Vertices = malloc(sizeof(VS_Input) * Data->VerticesCount);
    CopyData->VerticesCount = Data->VerticesCount;
    memcpy(CopyData->Vertices, Data->Vertices, sizeof(VS_Input) * Data->VerticesCount);

    CopyData->Indices = malloc(sizeof(int) * Data->IndicesCount);
    CopyData->IndicesCount = Data->IndicesCount;
    memcpy(CopyData->Indices, Data->Indices, sizeof(int) * Data->IndicesCount);

    CopyData->UpdateSpeed = Data->UpdateSpeed;
    CopyData->Time = 0;

    g_signal_connect(Area, "realize", G_CALLBACK(OnAreaRealise), CopyData);
    g_signal_connect(Area, "render", G_CALLBACK(OnAreaRender), CopyData);

    CopyData->Window = Window;
    CopyData->Frame = 0;

    gtk_widget_show(Area);

    return Area;
}
