//
//  main.cpp
//  maze.cpp
//
//  Created by Nathan Tjoar on 2/7/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Using recursion...
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
    if(sr == er && sc == ec) // Are we there yet?
        return true;
    if(sr >= nRows || er >= nRows || sc < 0 || ec < 0) // Out of bounds
        return false;
    maze[sr][sc] = '*'; //Mark position
    
    if(sr != nRows - 1 && maze[sr + 1][sc] != '*' && maze[sr + 1][sc] != 'X') // SOUTH
        if(pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
            return true;
    
    if(sr != 0 && maze[sr][sc - 1] != '*' && maze[sr][sc - 1] != 'X') // WEST
        if(pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
            return true;
    
    if(sr != 0 && maze[sr - 1][sc] != '*' && maze[sr - 1][sc] != 'X') // NORTH
        if(pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
            return true;
    
    if(sc != nCols - 1 && maze[sr][sc + 1] != '*' && maze[sr][sc + 1] != 'X') // EAST
        if(pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
            return true;
    
    return false;
}

int main() {
    string maze[10] = {
        "XXXXXXXXXX",
        "X....X...X",
        "X.XX.XX..X",
        "XXX....X.X",
        "X.XXX.XXXX",
        "X.X...X..X",
        "X...X.X..X",
        "XXXXX.X.XX",
        "X........X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 3,5, 8,8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
