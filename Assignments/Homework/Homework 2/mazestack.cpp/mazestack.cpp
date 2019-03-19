//
//  main.cpp
//  CS32_HW2
//
//  Created by Nathan Tjoar on 1/31/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#include <iostream>
#include <stack>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise
    stack<Coord> cStack;
    Coord start(sr, sc);
    cStack.push(start);
    maze[sr][sc] = '*';
    
    int row = sr;
    int col = sc;
    
    while(!cStack.empty())
    {
        start = cStack.top();
        row = start.r();
        col = start.c();
        cStack.pop();
        
        if(row == er && col == ec)
            return true;
        
        if(row != nRows - 1 &&
           maze[row + 1][col] != 'X' &&
           maze[row + 1][col] != '*') // Move South
        {
            Coord mv(row + 1, col);
            cStack.push(mv);
            maze[row + 1][col] = '*';
        }
        
        if(col != 0 &&
           maze[row][col - 1] != 'X' &&
           maze[row][col - 1] != '*') // Move West
        {
            Coord mv(row, col - 1);
            cStack.push(mv);
            maze[row][col - 1] = '*';
        }
        
        if(row != 0 &&
           maze[row - 1][col] != 'X' &&
           maze[row - 1][col] != '*') // Move North
        {
            Coord mv(row - 1, col);
            cStack.push(mv);
            maze[row - 1][col] = '*';
        }
        
        if(col != nCols - 1 &&
           maze[row][col + 1] != 'X' &&
           maze[row][col + 1] != '*') // Move East
        {
            Coord mv(row, col + 1);
            cStack.push(mv);
            maze[row][col + 1] = '*';
        }
    }
    return false;
}

int main()
{
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
