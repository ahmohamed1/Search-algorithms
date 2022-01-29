#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include<queue>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
// #include <unistd.h>

struct CompareDis {
    bool operator()(Node const& node1, Node const& node2) {
        // f = g + h
        int f_1 = node1.G + node1.H;
        int f_2 = node2.G + node2.H;
        return f_1 > f_2;
    }
};

class Astar {
public:
    // directional deltas
    const int delta[4][2]{ {-1, 0}, {0, -1}, {1, 0}, {0, 1} };
    // const int delta[8][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {1,1},{-1,1}, {-1,-1}, {1,-1}};

    std::vector<Node> Search(std::vector<std::vector<Node>> &grid, int init[2], int goal[2]) {

        std::priority_queue<Node, std::vector<Node>, CompareDis> openList;
        std::vector<int> path;
        int x = init[0];
        int y = init[1];
        int h = Heuristic(x, y, goal[0], goal[1]);
        AddToOpen(x, y, 0, h, 0, 0, openList, grid);
        int threshold = 0;
        //while open vector is non empty {
        while (!openList.empty()) {
            //Get the x and y values from the current node,
            auto currentNode = openList.top();
            openList.pop();
            x = currentNode.x;
            y = currentNode.y;
            grid[x][y].state = State::kVisited;

            // Check if you've reached the goal. If so, return grid.
            if (x == goal[0] && y == goal[1])
            {
                int _goal[2]{ x,y };
                auto path = RetracePath(init, _goal, grid);

                grid[init[0]][init[1]].state = State::kStart;
                grid[goal[0]][goal[1]].state = State::kFinish;
                std::cout << "Path found!\n";
                // PrintPath(path);
                return path;
            }
            // If we're not done, expand search to current node's neighbors.
            ExpandNeighbors(currentNode, openList, grid, goal);
        }
        std::cout << " No path found!" << "\n";
        return std::vector<Node>{};
    }

private:
    int Heuristic(int x1, int y1, int x2, int y2, int cost = 0) {

        return abs(x2 - x1) + abs(y2 - y1) + cost;
        // return std::sqrt(std::pow(x2 - x1,2) + std::pow(y2 - y1,2));
    }

    /**
     * Add a node to the open list and mark it as open.
     */
    void AddToOpen(int x, int y, int g, int h, int p_x, int p_y,
        std::priority_queue<Node, std::vector<Node>, CompareDis>& openList,
        std::vector<std::vector<Node>>& grid) {
        Node currentPoint{ x, y, State::kClose, p_x, p_y, g, h };
        openList.push(currentPoint);
        grid[x][y].parent_x = p_x;
        grid[x][y].parent_y = p_y;
    }


    /**
     * Check that a cell is valid: on the grid, not an obstacle, and clear.
     */
    bool CheckValidCell(int x, int y, std::vector<std::vector<Node>>& grid) {
        bool on_grid_x = (x >= 0 && x < grid.size());
        bool on_grid_y = (y >= 0 && y < grid[0].size());
        if (on_grid_x && on_grid_y) {
            State nodeState = grid[x][y].state;
            if (nodeState != State::kObstecal && grid[x][y].visited == false)
            {
                return grid[x][y].state == State::kEmpty;
            }
        }
        return false;
    }

    /**
     * Expand current nodes's neighbors and add them to the open list.
     */
    void ExpandNeighbors(Node& currentNode,
        std::priority_queue<Node, std::vector<Node>, CompareDis>& openList,
        std::vector<std::vector<Node>>& grid, int goal[2])
    {
        // Get current node's data.
        int x = currentNode.x;
        int y = currentNode.y;
        int g = currentNode.G;
        // Loop through current node's potential neighbors.
        for (auto neighnor : delta)
        {
            // Check that the potential neighbor's x2 and y2 values are on the grid and not closed.
            int x2 = x + neighnor[0];
            int y2 = y + neighnor[1];
            // Increment g value, compute h value, and add neighbor to open list.
            if (CheckValidCell(x2, y2, grid) == 1 && grid[x2][y2].visited == false)
            {
                // std::cout<< x2 << "," <<y2 << " ==> (" << x <<"," <<y <<")\n";
                int cost = grid[x2][y2].cost;
                int g2 = g + 1;
                int h2 = Heuristic(x2, y2, goal[0], goal[1], cost);
                AddToOpen(x2, y2, g2, h2, x, y, openList, grid);
                grid[x2][y2].visited = true;
            }
        }

    }


    std::vector<Node> RetracePath(int startNode[2], int goal[2], std::vector<std::vector<Node>>& grid)
    {
        std::vector<Node> path;
        Node currentNode = grid[goal[0]][goal[1]];
        int x = currentNode.x;
        int y = currentNode.y;
        while (x != startNode[0] || y != startNode[1])
        {
            // std::cout <<"currentNode x : " << currentNode.x << "," << currentNode.y <<" // ";
            // std::cout <<currentNode.parent_x << "," << currentNode.parent_y <<"\n";
            path.push_back(currentNode);
            grid[currentNode.x][currentNode.y].state = State::kPath;
            currentNode = grid[currentNode.parent_x][currentNode.parent_y];
            x = currentNode.x;
            y = currentNode.y;
        }

        std::reverse(path.begin(), path.end());
        std::cout << "Path lendth: " << path.size() << "\n";
        return path;
    }
};



