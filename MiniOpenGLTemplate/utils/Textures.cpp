#include "textures.hpp"
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>
#include <string>

Texture::Texture(const char* texturePath, unsigned int textureUnit) {
    unsigned char* data;
    this->textureUnit = textureUnit;
    //stbi_set_flip_vertically_on_load(true);
    data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data && nrChannels >= 0 && nrChannels <= 4) {
        switch (nrChannels) {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            break;
        }

        glGenTextures(1, &Id);
        glBindTexture(GL_TEXTURE_2D, Id);
        //Apply default parameter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); //generate all the required mipmap for the currently bounded Texture
    }
    else {
        throw std::invalid_argument(std::string("Error on loading texturePath ") + texturePath + std::string(" file not valid or nrChannels not supported: ") + std::to_string(nrChannels));
    }

    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &Id);
}

GLenum Texture::getTextureUnit() const {
    return GL_TEXTURE0 + textureUnit;
}
