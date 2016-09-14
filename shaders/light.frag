// For the lights, only the color really needs to be affected, so the cube vertex shader is used
#version 330 core

out vec4 color;

void main()
{
    color = vec4(1, 1, 1, 1);
}
