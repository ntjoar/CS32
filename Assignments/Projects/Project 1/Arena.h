//
//  Arena.hpp
//  CS32Proj1
//
//  Created by Nathan Tjoar on 1/12/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#ifndef Arena_hpp
#define Arena_hpp

#include "History.h"
#include "globals.h"

class Zombie;
class Player;

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     zombieCount() const;
    int     numZombiesAt(int r, int c) const;
    bool    determineNewPosition(int& r, int& c, int dir) const;
    void    display() const;
    History& history();
    
    // Mutators
    bool   addZombie(int r, int c);
    bool   addPlayer(int r, int c);
    bool   attackZombieAt(int r, int c, int dir);
    bool   moveZombies();
    
private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Zombie* m_zombies[MAXZOMBIES];
    int     m_nZombies;
    History m_history;
};

#endif /* Arena_hpp */
