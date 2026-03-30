#include <SDL3/SDL.h>
#include <cstring>
#include <fstream>
#include <glad/glad.h>
#include <print>
#include <vector>

struct PushConstants {
    float screenSize[2];
    float spritePos[2];
    float spriteSize[2];
    float pad[2];
    float params[4];
};

struct SpriteExtended {
    float mouse[4];
    float u0[4];
    float u1[4];
    float u2[4];
    float u3[4];
};

static GLuint loadTextureBMP(const char *path) {
    SDL_Surface *raw = SDL_LoadBMP(path);
    if (!raw) {
        std::println("Failed to load BMP: {}", SDL_GetError());
        return 0;
    }

    SDL_Surface *surface = SDL_ConvertSurface(raw, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(raw);
    if (!surface) {
        std::println("Failed to convert surface: {}", SDL_GetError());
        return 0;
    }

    int pitch = surface->pitch;
    int h = surface->h;
    std::vector<uint8_t> rowBuf(pitch);
    uint8_t *pixels = static_cast<uint8_t *>(surface->pixels);

    for (int i = 0; i < h / 2; ++i) {
        uint8_t *row1 = pixels + i * pitch;
        uint8_t *row2 = pixels + (h - i - 1) * pitch;
        std::memcpy(rowBuf.data(), row1, pitch);
        std::memcpy(row1, row2, pitch);
        std::memcpy(row2, rowBuf.data(), pitch);
    }

    GLuint tex = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &tex);
    glTextureStorage2D(tex, 1, GL_RGBA8, surface->w, h);
    glTextureSubImage2D(tex, 0, 0, 0, surface->w, h, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    SDL_DestroySurface(surface);
    return tex;
}

static std::vector<char> readFile(const char *path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::println("Failed to open: {}", path);
        return {};
    }
    size_t size = static_cast<size_t>(file.tellg());
    if (size == 0) {
        std::println("Empty file: {}", path);
        return {};
    }
    std::vector<char> buffer(size);
    file.seekg(0);
    file.read(buffer.data(), static_cast<std::streamsize>(size));
    return buffer;
}

static GLuint loadSPV(GLenum type, const char *path) {
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
    if (!ok) {
        char log[4096];
        glGetShaderInfoLog(shader, 4096, nullptr, log);
        std::println("Shader error ({}): {}", path, log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

struct FractalShader {
    GLuint program = 0;
};

template <typename T>
static T initShader() {
    T s;
    GLuint vs = loadSPV(GL_VERTEX_SHADER, "shaders/vertex.spv");
    GLuint fs = loadSPV(GL_FRAGMENT_SHADER, "shaders/frac.spv");
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
    if (!linked) {
        char log[4096];
        glGetProgramInfoLog(s.program, 4096, nullptr, log);
        std::println("Program link error: {}", log);
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return s;
}

int main(int argc, char **argv) {
    if (!SDL_Init(SDL_INIT_VIDEO))
        return EXIT_FAILURE;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window = SDL_CreateWindow("OpenGL SPV Fractal", 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext ctx = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, ctx);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::println("Failed to load GL");
        return EXIT_FAILURE;
    }

    SDL_GL_SetSwapInterval(0);

    std::println("GL_VERSION: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));
    if (!SDL_GL_ExtensionSupported("GL_ARB_gl_spirv")) {
        std::println("Missing GL_ARB_gl_spirv");
    }

    GLuint vao = 0;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint tex = 0;
    if (argc > 1) {
        tex = loadTextureBMP(argv[1]);
    }

    if (!tex) {
        glCreateTextures(GL_TEXTURE_2D, 1, &tex);
        glTextureStorage2D(tex, 1, GL_RGBA8, 1, 1);
        unsigned char color[] = {255, 255, 255, 255};
        glTextureSubImage2D(tex, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    }

    glBindTextureUnit(0, tex);

    GLuint ubos[2];
    glCreateBuffers(2, ubos);
    glNamedBufferStorage(ubos[0], sizeof(PushConstants), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(ubos[1], sizeof(SpriteExtended), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubos[0]);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubos[1]);

    FractalShader shader = initShader<FractalShader>();

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT ||
                (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE))
                running = false;
        }

        int w, h;
        SDL_GetWindowSizeInPixels(window, &w, &h);

        float mx, my;
        Uint32 mouseState = SDL_GetMouseState(&mx, &my);
        bool mDown = mouseState & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);
        float t = SDL_GetTicks() / 1000.0f;

        PushConstants pc = {};
        pc.screenSize[0] = static_cast<float>(w);
        pc.screenSize[1] = static_cast<float>(h);
        pc.spriteSize[0] = static_cast<float>(w);
        pc.spriteSize[1] = static_cast<float>(h);
        pc.params[3] = t;

        SpriteExtended ext = {};
        ext.mouse[0] = mx;
        ext.mouse[1] = static_cast<float>(h) - my;
        ext.mouse[2] = mDown ? 1.0f : 0.0f;

        glNamedBufferSubData(ubos[0], 0, sizeof(PushConstants), &pc);
        glNamedBufferSubData(ubos[1], 0, sizeof(SpriteExtended), &ext);

        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader.program);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    }

    glDeleteTextures(1, &tex);
    glDeleteBuffers(2, ubos);
    SDL_Quit();
    return EXIT_SUCCESS;
}
