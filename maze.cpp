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

#include "maze.h"

static void initWalls(std::vector<glm::vec3>& array)
{
    for (unsigned x = 0; x < MazeWidth; x++)
        for (unsigned y = 0; y < MazeHeight; y++)
            for (unsigned orientation = 0; orientation < 4; orientation++)
                array.push_back(glm::vec3(x, y, orientation));
}

static bool getCell(int x, int y, bool cells[][MazeHeight])
{
    return x >= 0 && x < (int)MazeWidth && y >= 0 && y < (int)MazeHeight ? cells[y][x] : true;
}

static std::vector<glm::vec2> getUnvisitedNeighbors(glm::vec2 pos, bool cells[][MazeHeight])
{
    std::vector<glm::vec2> result;
    if (!getCell(pos.x - 1, pos.y, cells))
        result.push_back(glm::vec2(pos.x - 1, pos.y));
    if (!getCell(pos.x, pos.y - 1, cells))
        result.push_back(glm::vec2(pos.x, pos.y - 1));
    if (!getCell(pos.x + 1, pos.y, cells))
        result.push_back(glm::vec2(pos.x + 1, pos.y));
    if (!getCell(pos.x, pos.y + 1, cells))
        result.push_back(glm::vec2(pos.x, pos.y + 1));
    return result;
}

static void removeWall(glm::vec2 currentCell, glm::vec2 newCell, std::vector<glm::vec3>& walls)
{
    auto remove = [&](unsigned wall, unsigned second)
    {
        walls.erase(std::remove(walls.begin(), walls.end(), glm::vec3(currentCell.x, currentCell.y, wall)), walls.end());
        walls.erase(std::remove(walls.begin(), walls.end(), glm::vec3(newCell.x, newCell.y, second)), walls.end());
    };

    if (newCell.x < currentCell.x)
        remove(0, 2);
    else if (newCell.x > currentCell.x)
        remove(2, 0);
    if (newCell.y < currentCell.y)
        remove(1, 3);
    else if (newCell.y > currentCell.y)
        remove(3, 1);
}

std::vector<glm::vec3> GenerateMaze()
{
    std::vector<glm::vec3> walls;
    initWalls(walls);

    glm::vec2 currentCell(0, 0);
    bool cells[MazeWidth][MazeHeight] = { { 0 } };
    cells[(int)currentCell.y][(int)currentCell.x] = true;
    unsigned visitedCells = 1;

    std::stack<glm::vec2> used;

    while (visitedCells < MazeWidth * MazeHeight) {
        std::vector<glm::vec2> neighbors = getUnvisitedNeighbors(currentCell, cells);
        if (neighbors.size() != 0) {
            glm::vec2 newCell = neighbors[std::rand() % neighbors.size()];
            used.push(currentCell);
            removeWall(currentCell, newCell, walls);
            currentCell = newCell;
            cells[(int)currentCell.y][(int)currentCell.x] = true;
            visitedCells++;
        }
        else if (used.size() != 0) {
            currentCell = used.top();
            used.pop();
        }
    }

    return walls;
}
