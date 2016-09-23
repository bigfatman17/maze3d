#ifndef MAZE_H
#define MAZE_H

#include <stack>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <cstdlib>
#include <iostream>

constexpr unsigned MazeWidth = 20, MazeHeight = 20;
constexpr float WallSize = 0.25f;
std::vector<glm::vec3> GenerateMaze();

#endif /* end of include guard: MAZE_H */
