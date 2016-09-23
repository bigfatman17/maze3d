#include "maze.h"

static void initWalls(std::vector<glm::vec3>& array)
{
    for (unsigned x = 0; x < mWIDTH; x++)
        for (unsigned y = 0; y < mHEIGHT; y++)
            for (unsigned orientation = 0; orientation < 4; orientation++)
                array.push_back(glm::vec3(x, y, orientation));
}

static bool getCell(int x, int y, bool cells[][mHEIGHT])
{
    return x >= 0 && x < (int)mWIDTH && y >= 0 && y < (int)mHEIGHT ? cells[y][x] : true;
}

static std::vector<glm::vec2> getUnvisitedNeighbors(glm::vec2 pos, bool cells[][mHEIGHT])
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

std::vector<glm::vec3> generateMaze() {
    std::vector<glm::vec3> walls;
    initWalls(walls);

    glm::vec2 currentCell(0, 0);
    bool cells[mWIDTH][mHEIGHT] = { { 0 } };
    cells[(int)currentCell.y][(int)currentCell.x] = true;
    unsigned visitedCells = 1;

    std::stack<glm::vec2> used;

    while (visitedCells < mWIDTH * mHEIGHT) {
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
