#ifndef MAZE_H
#define MAZE_H

#include <stack>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <cstdlib>
#include <iostream>

const unsigned mWIDTH = 20, mHEIGHT = 20;
const float wallSIZE = 0.25f;
std::vector<glm::vec3> generateMaze();

#endif /* end of include guard: MAZE_H */
