#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "shader.h"
#include "maze.h"

constexpr GLuint WIDTH = 800, HEIGHT = 600;

//camera stuff
glm::vec3 camPos(0.0f, 5.0f, 0.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);
glm::vec3 camUp(0.0f, 1.0f, 0.0f);

//mouse stuff
GLfloat lastX = WIDTH / 2;
GLfloat lastY = HEIGHT / 2;
GLfloat pitch = 0.0f;
GLfloat yaw = -90.0f;

//delta time stuff, to make sure things happen at the same rates
GLfloat delta = 0.0f;
GLfloat lastFrame = 0.0f;

bool keys[1024];

struct Cube {
    glm::vec3 position;
    glm::vec3 size;
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    camFront = glm::normalize(front);
}

void handle_movement()
{
    GLfloat camSpeed = 5.0f * delta;
    if (keys[GLFW_KEY_W])
        camPos += camSpeed * camFront;
    if (keys[GLFW_KEY_S])
        camPos -= camSpeed * camFront;
    if (keys[GLFW_KEY_A])
        camPos -= glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
    if (keys[GLFW_KEY_D])
        camPos += glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
    if (keys[GLFW_KEY_SPACE])
        camPos += camSpeed * camUp;
    if (keys[GLFW_KEY_LEFT_SHIFT])
        camPos -= camSpeed * camUp;
}

std::vector<Cube> convertMazeToWorld(std::vector<glm::vec3> maze)
{
    std::vector<Cube> result;

    for (glm::vec3 i : maze) {
        switch ((unsigned)i.z) {
            case 0:
                result.push_back({ glm::vec3(i.x - 0.5f + wallSIZE / 2, 1, i.y), glm::vec3(wallSIZE, 1, 1) });
                break;
            case 1:
                result.push_back({ glm::vec3(i.x, 1, i.y - 0.5f + wallSIZE / 2), glm::vec3(1, 1, wallSIZE) });
                break;
            case 2:
                result.push_back({ glm::vec3(i.x + 0.5f - wallSIZE / 2, 1, i.y), glm::vec3(wallSIZE, 1, 1) });
                break;
            case 3:
                result.push_back({ glm::vec3(i.x, 1, i.y + 0.5f - wallSIZE / 2), glm::vec3(1, 1, wallSIZE) });
                break;
        }
    }

    return result;
}

int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Random Generation of 3D Mazes", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    //the actual vertices for the cubes
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    //import the cube shader file
    Shader cube("shaders/cube.vert", "shaders/cube.frag");
    Shader lightShader("shaders/cube.vert", "shaders/light.frag");

    //create the vbo and vao for the cubes
    GLuint vbo, vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //point the shader to the actual positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //point the shader to the normal values
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    std::vector<Cube> level;
    //level.push_back({ glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) });
    for (unsigned x = 0; x < mWIDTH; x++)
        for (unsigned y = 0; y < mHEIGHT; y++)
            level.push_back({ glm::vec3(x, 0, y), glm::vec3(1) });

    //create a light for our maze
    Cube light = { glm::vec3(mWIDTH / 2, 3, mHEIGHT / 2), glm::vec3(0.2f) };

    std::srand(std::time(nullptr));

    std::vector<glm::vec3> initMaze = generateMaze();
    initMaze.erase(std::remove(initMaze.begin(), initMaze.end(), glm::vec3(0, 0, 1)), initMaze.end());
    initMaze.erase(std::remove(initMaze.begin(), initMaze.end(), glm::vec3(mWIDTH - 1, mHEIGHT - 1, 3)), initMaze.end());
    std::vector<Cube> maze = convertMazeToWorld(initMaze);
    level.reserve(level.size() + maze.size());
    level.insert(level.end(), maze.begin(), maze.end());

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        //calculate delta time
        GLfloat currentFrame = glfwGetTime();
        delta = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        handle_movement();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //update the camera
        glm::mat4 view = glm::lookAt(camPos, camPos + camFront, camUp);

        cube.use();
        glBindVertexArray(vao);

        //pass in light color
        glUniform3f(glGetUniformLocation(cube.program, "light"), 1.0f, 1.0f, 1.0f);

        glUniformMatrix4fv(glGetUniformLocation(cube.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(cube.program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        for (unsigned i = 0; i < level.size(); i++) {
            glm::mat4 model;
            model = glm::translate(model, level[i].position);
            model = glm::scale(model, level[i].size);
            glUniformMatrix4fv(glGetUniformLocation(cube.program, "model"), 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glUniform3f(glGetUniformLocation(cube.program, "lightPos"), light.position.x, light.position.y, light.position.z);

        lightShader.use();

        glUniformMatrix4fv(glGetUniformLocation(lightShader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glm::mat4 model;
        model = glm::translate(model, light.position);
        model = glm::scale(model, light.size);
        glUniformMatrix4fv(glGetUniformLocation(lightShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glUseProgram(0);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();

    return 0;
}
