//
//  History.hpp
//  CS32Proj1
//
//  Created by Nathan Tjoar on 1/12/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#ifndef History_hpp
#define History_hpp

#include "globals.h"

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
private:
    int m_rows;
    int m_cols;
    char deadZ[MAXROWS][MAXCOLS];
};

#endif /* History_hpp */
