const int delta[4][2]{ {-1, 0}, {0, -1}, {1, 0}, {0, 1} };
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

// This function used to add node to the openList and update the node on the grid
template <class listT>
void AddToOpenList(int x, int y, int p_x, int p_y, vector<vector<Node>>& grid, listT& openList) {
    Node currentNode{ x, y, State::kClose, p_x, p_y };
    openList.push(currentNode);
    grid[x][y].parent_x = p_x;
    grid[x][y].parent_y = p_y;
    grid[x][y].state = State::kClose;
}

// this function used to expand the neightbor of the cuurent Node
template<class listTT>
void ExpandNeighbor(Node currentNode, listTT& openList, vector<vector<Node>>& grid) {
    int x = currentNode.x;
    int y = currentNode.y;

    for (auto neighbor : delta)
    {
        int x2 = x + neighbor[0];
        int y2 = y + neighbor[1];
        if (CheckValidCell(x2, y2, grid) == 1)
        {
            AddToOpenList(x2, y2, x, y, grid, openList);
            grid[x2][y2].state = State::kVisited;
        }
    }
}


vector<Node> RetracePath(int startNode[2], int goal[2], vector<vector<Node>>& grid)
{
    vector<Node> path;
    Node currentNode = grid[goal[0]][goal[1]];
    int x = currentNode.x;
    int y = currentNode.y;
    while (x != startNode[0] || y != startNode[1])
    {
        path.push_back(currentNode);
        grid[currentNode.x][currentNode.y].state = State::kPath;
        currentNode = grid[currentNode.parent_x][currentNode.parent_y];
        x = currentNode.x;
        y = currentNode.y;
    }

    reverse(path.begin(), path.end());
    cout << "Path length: " << path.size() << "\n";
    return path;
}

/// <summary>
/// Apply breadth first: looking wide first
/// </summary>
/// <param name="grid"></param>
/// <param name="start"></param>
/// <param name="goal"></param>
/// <returns></returns>
string BreadthFirst(vector < vector<Node>>& grid, int start[2], int goal[2]) {
    // step 1: initialize nodeList and add the start node to the list
    queue <Node> openList;
    int x = start[0];
    int y = start[1];
    AddToOpenList(x, y, 0, 0, grid, openList);
    // step 3: check if the list is empty
    while (!openList.empty())
    {
        // step 5: dequeue node as currentnode and remove it from the list
        Node currentNode = openList.front();
        // remove the first element
        openList.pop();

        x = currentNode.x;
        y = currentNode.y;
        //cout << x << "," << y << "\n";
        grid[x][y].visited = true;
        grid[x][y].state = State::kVisited;

        // step 6: check if the current node is the goal
        if (x == goal[0] && y == goal[1])
        {
            RetracePath(start, goal, grid);
            grid[start[0]][start[1]].state = State::kStart;
            grid[goal[0]][goal[1]].state = State::kFinish;
            return "Path found";
            break;
        }
        // step 7: get the neighbors of the current node
        ExpandNeighbor(currentNode, openList, grid);
    }
    return "No path Found";

}

/// <summary>
/// Apply the depth first: looking deep first
/// </summary>
/// <param name="grid"></param>
/// <param name="start"></param>
/// <param name="goal"></param>
/// <returns></returns>
string DepthFirst(vector<vector<Node>>& grid, int start[2], int goal[2]) {
    // step 1: initialize openList and add that start node
    stack <Node> openList;
    int x = start[0];
    int y = start[1];
    AddToOpenList(x, y, 0, 0, grid, openList);

    // step 2: check iof open list is not empty
    while (!openList.empty())
    {
        // step 3: get the currentNode from the stack 
        Node currentNode = openList.top();
        openList.pop();
        x = currentNode.x;
        y = currentNode.y;
        //cout << x << "," << y << "\n";
        // step 4: mark the node as visited
        grid[x][y].visited = true;
        grid[x][y].state = State::kVisited;
        // step 5: check if the node is the goal
        if (x == goal[0] && y == goal[1])
        {
            RetracePath(start, goal, grid);
            grid[start[0]][start[1]].state = State::kStart;
            grid[goal[0]][goal[1]].state = State::kFinish;
            return "[INFO] path was found...";
            break;
        }

        // step 6: check the neighbor of the node
        ExpandNeighbor(currentNode, openList, grid);
    }
    return " [INFO] No path was found...";

}