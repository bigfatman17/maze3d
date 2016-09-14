#ifndef SHADER_H

#define SHADER_H

#include <string>
#include <fstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
    GLuint program;

    void use();

    Shader(const GLchar* vert, const GLchar* frag);
    ~Shader();
};

#endif /* end of include guard: SHADER_H */
