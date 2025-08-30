#pragma once
#include <glad/glad.h>


class Texture {
public:

    Texture(const char* texturePath, unsigned int textureUnit);
    ~Texture();
    void use(void) const { glActiveTexture(getTextureUnit()); glBindTexture(GL_TEXTURE_2D, Id); };
    void setParameter(GLenum pname, GLint param) const { glBindTexture(GL_TEXTURE_2D, Id); glTexParameteri(GL_TEXTURE_2D, pname, param); };
    unsigned int getId(void) const { return Id; };
    unsigned int textureUnit;
    unsigned int format;

private:

    // Texture coordinates s,t,r (don't know why are different name from x,y,z but s=x, t=y, r=z) are float between 0, 1
    // (0,1)                            (1,1)
    // 
    // 
    // 
    // (0,0)                            (1,0)
    // 
    unsigned int Id;          
    int          width;       //width in pixels of the image loaded
    int          height;      //height in pixels of the image loaded
    int          nrChannels;  //nr of channels of the image loaded
    
    GLenum getTextureUnit() const;
};
