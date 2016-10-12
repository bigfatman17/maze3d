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
