/* Maze 3D - A simplistic random maze generator.
 * Copyright (C) 2016 Armend Murtishi

 * This file is part of Maze 3D.

 * Maze 3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Maze 3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Maze 3D.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "shader.h"

void Shader::use() {
    glUseProgram(this->program);
}

std::string readfile(const GLchar* path)
{
    std::string result;
    std::ifstream stream(path, std::ios::in);

    if (!stream.is_open()) {
        std::cout << "ERROR::SHADER::FILE_DOES_NOT_EXIST" << std::endl;
        return "";
    }

    std::string line = "";
    while (!stream.eof()) {
        std::getline(stream, line);
        result.append(line + "\n");
    }

    stream.close();
    return result;
}

Shader::Shader(const GLchar* vert, const GLchar* frag) {
    //loading the shaders
    std::string vertexC = readfile(vert);
    std::string fragmentC = readfile(frag);

    const GLchar* vertexCode = vertexC.c_str();
    const GLchar* fragmentCode = fragmentC.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    //compiling the vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);
    //get the errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
    glCompileShader(fragment);
    //get the errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //create the shader program finally
    this->program = glCreateProgram();
    glAttachShader(this->program, vertex);
    glAttachShader(this->program, fragment);
    glLinkProgram(this->program);
    //get linking errors
    glGetShaderiv(this->program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(this->program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //delete our shaders so they don't take up memory
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(this->program);
}
