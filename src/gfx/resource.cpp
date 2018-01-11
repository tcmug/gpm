
#include "resource.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
///  #include <SDL2_Mixer/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <map>
#include <iostream>
#include <string>

std::map <std::string, GLuint> textures;
std::map <std::string, shader*> shaders;

void resource_flush() {

    for (auto &entry : textures) {
        std::cout << "Texture deleted: " << entry.first << std::endl;
        glDeleteTextures(1, &entry.second);
    }

    textures.clear();

    for (auto &entry : shaders) {
        std::cout << "Shader deleted: " << entry.first << std::endl;
        delete entry.second;
    }

    shaders.clear();
}




shader *resource_load_shader(const char *vertex_name, const char *geometry_name, const char *fragment_name) {

    std::string id;

    if (vertex_name) {
        id += vertex_name;
        id += "#";
    }
    if (geometry_name) {
        id += geometry_name;
        id += "#";
    }
    if (fragment_name) {
        id += fragment_name;
        id += "#";
    }

    auto item = shaders.find(id);

    if (item != shaders.end()) {
        std::cout << "Shader loaded from cache: " << id << std::endl;
        return item->second;
    }

    shader *shd = new shader(vertex_name, geometry_name, fragment_name);

    shaders[id] = shd;

    std::cout << "Shader added to cache: " << id << std::endl;

    return shd;
}



GLuint resource_load_texture(const char *file) {

    auto item = textures.find(file);

    if (item != textures.end()) {
        std::cout << "Texture loaded from cache: " << file << std::endl;
        return item->second;
    }

    SDL_Surface *surface;
    GLuint texture = 0;

    surface = IMG_Load(file);

    if (surface) {

        int Mode = GL_RGBA;
        int Colors = surface->format->BytesPerPixel;

        if (Colors == 4) {
            if (surface->format->Rmask == 0x000000ff)
                Mode = GL_RGBA;
            else
                Mode = GL_BGRA;
            } else if (Colors == 3) {
            if (surface->format->Rmask == 0x000000ff)
                Mode = GL_RGB;
            else
            Mode = GL_BGR;
        } else {
            Mode = GL_COLOR_INDEX;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        float pixels[] = {
            1.0f, 0.5f, 0.0f,   1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.5f
        };

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, Mode, GL_UNSIGNED_BYTE, surface->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        engine_gl_check();

        SDL_FreeSurface(surface);

    } else {
        std::cout << "Failed to load: " << file << std::endl;
    }

    std::cout << "Texture added to cache: " << file << std::endl;
    textures[file] = texture;

    return texture;
}
