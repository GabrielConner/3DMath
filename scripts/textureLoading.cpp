#include "textureLoading.h"

bool loadTexture(std::string name, unsigned int& textureOut, int& width, int& height) {
    glDeleteTextures(1, &textureOut);
    glGenTextures(1, &textureOut);

    glBindTexture(GL_TEXTURE_2D, textureOut);

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    unsigned char* imageData = nullptr;
    int channels = 0;

    imageData = stbi_load(("textures/" + name).c_str(), &width, &height, &channels, 0);

    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(imageData);
        return true;
    }
    else {
        stbi_image_free(imageData);
        return false;
    }
}