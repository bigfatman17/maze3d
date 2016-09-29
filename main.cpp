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

constexpr unsigned WindowWidth = 800, WindowHeight = 600;

struct Cube {
    glm::vec3 position;
    glm::vec3 size;
};

class Keyboard {
    private:
        GLFWwindow* window;
    public:
        bool getKey(int key) {
            return glfwGetKey(window, key) == GLFW_PRESS;
        }

        Keyboard(GLFWwindow* win) { window = win; }
};

class Camera {
    private:
        //we need the window pointer so we can calculate the mouse coordinates
        GLFWwindow* window;
        //mouse variables
        float lastX = WindowWidth / 2;
        float lastY = WindowHeight / 2;
        float pitch = 0.0f;
        float yaw = -90.0f;
        //camera stuff
        glm::vec3 DirFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 DirUp = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::mat4 Matrix;
    public:
        glm::vec3 pos = glm::vec3(0.0f, 5.0f, 0.0f);
        glm::vec3 front() const { return DirFront; }
        glm::vec3 up() const { return DirUp; }

        glm::mat4 matrix() const { return Matrix; }

        void update() {
            Matrix = glm::lookAt(pos, pos + DirFront, DirUp);
            static double mX, mY;
            glfwGetCursorPos(window, &mX, &mY);
            if (mX != lastX || mY != lastY)
                rotate(mX, mY);
        }

        float sensitivity = 0.1f;

        void rotate(double mouseX, double mouseY)
        {
            GLfloat xoffset = mouseX - lastX;
            GLfloat yoffset = lastY - mouseY;
            lastX = mouseX;
            lastY = mouseY;

            xoffset *= sensitivity;
            yoffset *= sensitivity;

            yaw += xoffset;
            pitch += yoffset;

            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;

            glm::vec3 newFront;
            newFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
            newFront.y = sin(glm::radians(pitch));
            newFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
            DirFront = glm::normalize(newFront);
        }

        Camera(GLFWwindow* win) { window = win; }
};

static void MovePlayer(Camera& camera, Keyboard keyboard, float delta)
{
    float camSpeed = 5.0f * delta;
    if (keyboard.getKey(GLFW_KEY_W))
        camera.pos += camSpeed * camera.front();
    if (keyboard.getKey(GLFW_KEY_S))
        camera.pos -= camSpeed * camera.front();
    if (keyboard.getKey(GLFW_KEY_A))
        camera.pos -= glm::normalize(glm::cross(camera.front(), camera.up())) * camSpeed;
    if (keyboard.getKey(GLFW_KEY_D))
        camera.pos += glm::normalize(glm::cross(camera.front(), camera.up())) * camSpeed;
    if (keyboard.getKey(GLFW_KEY_SPACE))
        camera.pos += camSpeed * camera.up();
    if (keyboard.getKey(GLFW_KEY_LEFT_SHIFT))
        camera.pos -= camSpeed * camera.up();
}

static std::vector<Cube> ConvertMazeToWorld(std::vector<glm::vec3> maze)
{
    std::vector<Cube> result;

    for (glm::vec3 i : maze) {
        switch ((unsigned)i.z) {
            case 0:
                result.push_back({ glm::vec3(i.x - 0.5f + WallSize / 2, 1, i.y), glm::vec3(WallSize, 1, 1) });
                break;
            case 1:
                result.push_back({ glm::vec3(i.x, 1, i.y - 0.5f + WallSize / 2), glm::vec3(1, 1, WallSize) });
                break;
            case 2:
                result.push_back({ glm::vec3(i.x + 0.5f - WallSize / 2, 1, i.y), glm::vec3(WallSize, 1, 1) });
                break;
            case 3:
                result.push_back({ glm::vec3(i.x, 1, i.y + 0.5f - WallSize / 2), glm::vec3(1, 1, WallSize) });
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
    GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "Random Generation of 3D Mazes", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glViewport(0, 0, WindowWidth, WindowHeight);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Camera camera(window);
    Keyboard keyboard(window);

    //the actual vertices for the cubes
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

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
    for (unsigned x = 0; x < MazeWidth; x++)
        for (unsigned y = 0; y < MazeHeight; y++)
            level.push_back({ glm::vec3(x, 0, y), glm::vec3(1) });

    //create a light for our maze
    Cube light = { glm::vec3(MazeWidth / 2, 3, MazeHeight / 2), glm::vec3(0.2f) };

    std::srand(std::time(nullptr));

    std::vector<glm::vec3> initMaze = GenerateMaze();
    initMaze.erase(std::remove(initMaze.begin(), initMaze.end(), glm::vec3(0, 0, 1)), initMaze.end());
    initMaze.erase(std::remove(initMaze.begin(), initMaze.end(), glm::vec3(MazeWidth - 1, MazeHeight - 1, 3)), initMaze.end());
    std::vector<Cube> maze = ConvertMazeToWorld(initMaze);
    level.reserve(level.size() + maze.size());
    level.insert(level.end(), maze.begin(), maze.end());

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

    //delta time stuff, to make sure things happen at the same rates
    float delta = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        //calculate delta time
        float currentFrame = glfwGetTime();
        delta = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        //keyboard events
        //check for escape to quit game
        if (keyboard.getKey(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);
        MovePlayer(camera, keyboard, delta);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.update();

        cube.use();
        glBindVertexArray(vao);

        //pass in light color
        glUniform3f(glGetUniformLocation(cube.program, "light"), 1.0f, 1.0f, 1.0f);

        glUniformMatrix4fv(glGetUniformLocation(cube.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(cube.program, "view"), 1, GL_FALSE, glm::value_ptr(camera.matrix()));

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
        glUniformMatrix4fv(glGetUniformLocation(lightShader.program, "view"), 1, GL_FALSE, glm::value_ptr(camera.matrix()));
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
