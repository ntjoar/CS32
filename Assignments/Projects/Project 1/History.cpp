//
//  History.cpp
//  CS32Proj1
//
//  Created by Nathan Tjoar on 1/12/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#include "History.h"

///////////////////////////////////////////////////////////////////////////
//  History implementations
///////////////////////////////////////////////////////////////////////////

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    for(int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            deadZ[r - 1][c - 1] = '.';
}

bool History::record(int r, int c)
{
    if (r > m_rows || r < 1 || c > m_cols || c < 1)
        return false;
    else
    {
        if (deadZ[r - 1][c - 1] == '.')
            deadZ[r - 1][c - 1] = 'A';
        else if (deadZ[r - 1][c - 1] < 'Z')
            deadZ[r - 1][c - 1]++;
        return true;
    }
}

void History::display() const
{
    // Draw the grid
    clearScreen();
    for (int r = 1; r <= m_rows; r++)
    {
        for (int c = 1; c <= m_cols; c++)
            cout << deadZ[r - 1][c - 1];
        cout << endl;
    }
    cout << endl;
}
