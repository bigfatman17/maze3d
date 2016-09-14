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
