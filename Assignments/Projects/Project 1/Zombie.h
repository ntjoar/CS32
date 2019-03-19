//
//  Zombies.hpp
//  CS32Proj1
//
//  Created by Nathan Tjoar on 1/12/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#ifndef Zombies_hpp
#define Zombies_hpp

class Arena;

class Zombie
{
public:
    // Constructor
    Zombie(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    
    // Mutators
    void move();
    bool getAttacked(int dir);
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
};

#endif /* Zombies_hpp */
