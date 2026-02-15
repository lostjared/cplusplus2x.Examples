/* 
	SDL3/OpenGL 4.6/C++23
*/

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <vector>
#include <fstream>
#include <print>

static std::vector<char> readFile(const char* path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::println("Failed to open: {}", path);
        return {};
    }
    size_t size = (size_t)file.tellg();
    if (size == 0) {
        std::println("Empty file: {}", path);
        return {};
    }
    std::vector<char> buffer(size);
    file.seekg(0);
    file.read(buffer.data(), (std::streamsize)size);
    return buffer;
}

static GLuint loadSPV(GLenum type, const char* path)
{
    auto data = readFile(path);
    if (data.empty()) {
        std::println("SPV load failed: {}", path);
        return 0;
    }

    GLuint shader = glCreateShader(type);
    glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V,
                   data.data(), data.size());

    glSpecializeShader(shader, "main", 0, nullptr, nullptr);

    GLint ok = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if(!ok)
    {
        char log[4096];
        glGetShaderInfoLog(shader, 4096, nullptr, log);
        std::println("Shader error ({}): {}", path, log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

struct StaticShader
{
    GLuint program = 0;
};

static StaticShader initStaticShader()
{
    StaticShader s;

    GLuint vs = loadSPV(GL_VERTEX_SHADER,   "shaders/static.vert.spv");
    GLuint fs = loadSPV(GL_FRAGMENT_SHADER, "shaders/static.frag.spv");

    if (!vs || !fs) {
        std::println("Shader creation failed");
        return s;
    }

    s.program = glCreateProgram();
    glAttachShader(s.program, vs);
    glAttachShader(s.program, fs);
    glLinkProgram(s.program);

    GLint linked = 0;
    glGetProgramiv(s.program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        char log[4096];
        glGetProgramInfoLog(s.program, 4096, nullptr, log);
        std::println("Program link error: {}", log);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return s;
}

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) return 1;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow("OpenGL SPV Static", 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext ctx = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, ctx);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::println("Failed to load GL");
        return 1;
    }

    SDL_GL_SetSwapInterval(0);

    std::println("GL_VERSION: {}", (const char*)glGetString(GL_VERSION));
    if (!SDL_GL_ExtensionSupported("GL_ARB_gl_spirv")) {
        std::println("Missing GL_ARB_gl_spirv");
    }

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    StaticShader shader = initStaticShader();

    bool running = true;
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT ||
               (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE))
                running = false;
        }

        int w,h;
        SDL_GetWindowSizeInPixels(window,&w,&h);

        glViewport(0,0,w,h);
        glClearColor(0.1f,0.0f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader.program);
        float t = SDL_GetTicks()/1000.0f;
        glUniform1f(0, t);
        glUniform2f(1, (float)w, (float)h);

        glDrawArrays(GL_TRIANGLES,0,3);

        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();
    return 0;
}

