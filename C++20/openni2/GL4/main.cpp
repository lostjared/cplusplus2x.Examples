#include <OpenNI.h>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#include <algorithm>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/signal.h>
#include <unistd.h>
#include <vector>

#include "cmx_gl.h"

#define APP_WIDTH 1440
#define APP_HEIGHT 1080

#define DEPTH_WIDTH 640
#define DEPTH_HEIGHT 480

#define COLOR_WIDTH 640
#define COLOR_HEIGHT 480



#define COLOR_CAM_INDEX 3



static const float vertices[] = {
    
    1.0f,
    1.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0f, 
    1.0f,
    -1.0f,
    0.0f,
    0.0f,
    1.0f,
    0.0f, 
    -1.0f,
    -1.0f,
    0.0f,
    0.0f,
    0.0f,
    1.0f, 
    -1.0f,
    1.0f,
    0.0f,
    1.0f,
    1.0f,
    0.0f, 
};
static const uint32_t indices[] = {
    0,
    1,
    3,
    1,
    2,
    3,
};

class DepthDevice {
  public:
    openni::Device device;
    openni::VideoStream depth;
    openni::VideoFrameRef frame;
    int frame_width = 0;
    int frame_height = 0;
    int pixel_count = 0;
    DepthDevice() = default;
};

bool init_depth_sensor(DepthDevice *dep) {
    if (openni::OpenNI::initialize() != openni::STATUS_OK) {
        return false;
    }
    if (dep->device.open(openni::ANY_DEVICE) != openni::STATUS_OK) {
        return false;
    }
    if (dep->device.isImageRegistrationModeSupported(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR)) {
        dep->device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);
        std::cout << "Depth-to-Color Registration Enabled." << std::endl;
    }
    dep->depth.create(dep->device, openni::SENSOR_DEPTH);
    dep->depth.setMirroringEnabled(false);

    const openni::SensorInfo &info = dep->depth.getSensorInfo();
    const openni::Array<openni::VideoMode> &modes = info.getSupportedVideoModes(); 

    bool modeFound = false;
    for (int i = 0; i < modes.getSize(); ++i) {
        if (modes[i].getResolutionX() == DEPTH_WIDTH && modes[i].getResolutionY() == DEPTH_HEIGHT) {
            std::cout << "Mode: " << modes[i].getResolutionX() << "x" << modes[i].getResolutionY() << "\n";
            if (dep->depth.setVideoMode(modes[i]) == openni::STATUS_OK) {
                modeFound = true;
                break;
            }
        }
    }

    if (dep->depth.start() != openni::STATUS_OK) {
        return false;
    }
    return true;
}
const uint16_t *pull_depth_frame(DepthDevice *dep) {
    if (dep->depth.readFrame(&dep->frame) == openni::STATUS_OK) {
        const uint16_t *d = (const uint16_t *)dep->frame.getData();
        dep->frame_width = dep->frame.getWidth();
        dep->frame_height = dep->frame.getHeight();
        dep->pixel_count = dep->frame_width * dep->frame_height;
        return d;
    }
    return nullptr;
}

void close_depth_sensor(DepthDevice *dep) {
    dep->device.close();
    _exit(0);
}

DepthDevice dep;

void sig_action(int) {
    close_depth_sensor(&dep);
}

int main(int argc, char **argv) {
    
    signal(SIGINT, sig_action);

    
    
    enum class Mode {
        Glitch,
        Depth,
        Custom
    };
    Mode mode = Mode::Glitch;
    std::string compute_path = "compute.spv";
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "glitch") {
            mode = Mode::Glitch;
            compute_path = "compute.spv";
        } else if (arg == "depth" || arg == "depth_demo") {
            mode = Mode::Depth;
            compute_path = "depth_demo.spv";
        } else if (arg == "aura") {
            mode = Mode::Glitch; 
            compute_path = "aura.spv";
        } else if (arg == "aura2") {
            mode = Mode::Glitch;
            compute_path = "aura2.spv";
        } else if (arg == "lidar") {
            mode = Mode::Glitch;
            compute_path = "lidar.spv";
        } else {
            
            
            mode = (arg.find("depth") != std::string::npos) ? Mode::Depth
                                                            : Mode::Custom;
            compute_path = arg;
        }
    }
    std::cout << "Compute shader: " << compute_path
              << " (mode=" << (mode == Mode::Depth ? "depth" : mode == Mode::Glitch ? "glitch"
                                                                                    : "custom")
              << ")" << std::endl;

    if (!init_depth_sensor(&dep)) {
        return EXIT_FAILURE;
    }
    std::cout << "Successfully opened depth device!" << std::endl;

    
    cv::VideoCapture color_cam(COLOR_CAM_INDEX, cv::CAP_V4L2);
    if (!color_cam.isOpened()) {
        std::cerr << "Could not open color camera at index "
                  << COLOR_CAM_INDEX << std::endl;
        close_depth_sensor(&dep);
        return EXIT_FAILURE;
    }
    color_cam.set(cv::CAP_PROP_FRAME_WIDTH, COLOR_WIDTH);
    color_cam.set(cv::CAP_PROP_FRAME_HEIGHT, COLOR_HEIGHT);
    color_cam.set(cv::CAP_PROP_FPS, 30.0);
    color_cam.set(cv::CAP_PROP_BUFFERSIZE, 1);
    std::cout << "Color camera open: "
              << (int)color_cam.get(cv::CAP_PROP_FRAME_WIDTH) << "x"
              << (int)color_cam.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
    std::cout << "Press ESC to exit..." << std::endl;

    struct mx_app_info app = {};
    if (!mx_init_sdl(&app, "Astra Pro Glitch (depth + color)", APP_WIDTH, APP_HEIGHT)) {
        close_depth_sensor(&dep);
        return EXIT_FAILURE;
    }

    SDL_Event e;
    bool active = true;
    GLuint vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    
    
    
    GLuint color_texture;
    glGenTextures(1, &color_texture);
    glBindTexture(GL_TEXTURE_2D, color_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, COLOR_WIDTH, COLOR_HEIGHT);
    glBindTexture(GL_TEXTURE_2D, 0);

    
    GLuint depth_texture;
    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R16UI, DEPTH_WIDTH, DEPTH_HEIGHT);
    glBindTexture(GL_TEXTURE_2D, 0);

    
    
    GLuint output_texture;
    glGenTextures(1, &output_texture);
    glBindTexture(GL_TEXTURE_2D, output_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, COLOR_WIDTH, COLOR_HEIGHT);
    glBindTexture(GL_TEXTURE_2D, 0);

    
    
    GLuint trail_texture[2];
    glGenTextures(2, trail_texture);
    for (int i = 0; i < 2; ++i) {
        glBindTexture(GL_TEXTURE_2D, trail_texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, COLOR_WIDTH, COLOR_HEIGHT);
        
        std::vector<uint8_t> zeros(COLOR_WIDTH * COLOR_HEIGHT * 4, 0);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                        COLOR_WIDTH, COLOR_HEIGHT,
                        GL_RGBA, GL_UNSIGNED_BYTE, zeros.data());
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    int trail_read = 0, trail_write = 1;
    MX_CHECK_GL_ERROR();

    GLuint program = mx_create_shader_program("vert.spv", "frag.spv");
    GLuint compute_program = mx_create_compute_program(compute_path.c_str());
    GLuint trails_program = mx_create_compute_program("trails.spv");
    if (program == 0 || compute_program == 0 || trails_program == 0) {
        glDeleteTextures(1, &color_texture);
        glDeleteTextures(1, &depth_texture);
        glDeleteTextures(1, &output_texture);
        glDeleteTextures(2, trail_texture);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
        mx_close_sdl(&app);
        printf("Shader program creation failed.\n");
        close_depth_sensor(&dep);
        return EXIT_FAILURE;
    }

    GLint u_time_loc = glGetUniformLocation(compute_program, "u_time");
    Uint64 start_ticks = SDL_GetTicks();

    cv::Mat bgr;

    while (active) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_EVENT_QUIT:
                active = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch (e.key.key) {
                case SDLK_ESCAPE:
                    active = false;
                    break;
                }
                break;
            }
        }

        
        if (color_cam.read(bgr) && !bgr.empty()) {
            if (bgr.cols != COLOR_WIDTH || bgr.rows != COLOR_HEIGHT) {
                cv::resize(bgr, bgr, cv::Size(COLOR_WIDTH, COLOR_HEIGHT));
            }
            glBindTexture(GL_TEXTURE_2D, color_texture);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, (GLint)(bgr.step / bgr.elemSize()));
            
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            COLOR_WIDTH, COLOR_HEIGHT,
                            GL_BGR, GL_UNSIGNED_BYTE, bgr.data);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        
        const uint16_t *p_data = pull_depth_frame(&dep);
        if (p_data) {
            glBindTexture(GL_TEXTURE_2D, depth_texture);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            dep.frame_width, dep.frame_height,
                            GL_RED_INTEGER, GL_UNSIGNED_SHORT, p_data);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        
        glUseProgram(compute_program);
        float t = (float)(SDL_GetTicks() - start_ticks) / 1000.0f;
        if (u_time_loc >= 0)
            glUniform1f(u_time_loc, t);
        if (mode == Mode::Depth) {
            
            glBindImageTexture(0, depth_texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16UI);
            glBindImageTexture(1, output_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
        } else {
            
            glBindImageTexture(0, color_texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
            glBindImageTexture(1, output_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
            glBindImageTexture(2, depth_texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16UI);
        }
        glDispatchCompute((COLOR_WIDTH + 15) / 16, (COLOR_HEIGHT + 15) / 16, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
        MX_CHECK_GL_ERROR();

        
        glUseProgram(trails_program);
        glBindImageTexture(0, output_texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
        glBindImageTexture(1, trail_texture[trail_write], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
        glBindImageTexture(2, trail_texture[trail_read], 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
        glBindImageTexture(3, depth_texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16UI);
        glDispatchCompute((COLOR_WIDTH + 15) / 16, (COLOR_HEIGHT + 15) / 16, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
        MX_CHECK_GL_ERROR();

        
        glViewport(0, 0, app.w, app.h);
        glClearColor(0.1f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, trail_texture[trail_write]);
        glUniform1i(glGetUniformLocation(program, "screenTex"), 0);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        SDL_GL_SwapWindow(app.window);

        
        std::swap(trail_read, trail_write);
    }

    glDeleteProgram(program);
    glDeleteProgram(compute_program);
    glDeleteProgram(trails_program);
    glDeleteTextures(1, &color_texture);
    glDeleteTextures(1, &depth_texture);
    glDeleteTextures(1, &output_texture);
    glDeleteTextures(2, trail_texture);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
    color_cam.release();
    mx_close_sdl(&app);

    std::cout << "Force exiting to bypass driver bug..." << std::endl;
    close_depth_sensor(&dep);
    return EXIT_SUCCESS;
}
