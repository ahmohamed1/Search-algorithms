// SearchAlgorithms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<string>
#include<vector>
#include <algorithm>  // for sort
#include <fstream>
#include <sstream>
#include <queue> 
#include <stack>          // std::stack

#include <tuple>
#include <map>
#include <vector>
using namespace std;
#include"helper.h"
#include"algorithms.h"
#include"astart.h"
#include"Grid.h"


int main()
{
    auto board = ReadBoard("1board.txt");
    auto grid = CreateGrid(board);
    int start[2]{7, 15};
    int goal[2]{7, 10};
    cout << "[INFO] Breadth First...\n";
    cout << BreadthFirst(grid, start, goal) << "\n";
    PrintGrid(grid);
    cout << "[INFO] Depth First...\n";
    grid = CreateGrid(board);
    cout << DepthFirst(grid, start, goal) << "\n";
    PrintGrid(grid);
    cout << "[INFO] A star...\n";
    grid = CreateGrid(board);
    Astar astar;
    Grid gridMap;
    auto map = gridMap.ConvertToMat(grid);
    auto path = astar.Search(grid, start, goal);
    PrintGrid(grid);
   
    return 0;
}