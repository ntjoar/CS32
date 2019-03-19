//
//  Game.hpp
//  CS32Proj1
//
//  Created by Nathan Tjoar on 1/12/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

class Arena;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nZombies);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
};

#endif /* Game_hpp */
