#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Level.h"
#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    // Basic
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init(); // Initialize the level
    virtual int move(); // Every tick, do...
    virtual void cleanUp(); // Destroy everything
    
    // Getter Functions
    int getTick() const {return m_tick;} // What is the current tick
    bool goToNewLevel(); // Go to a new level
    bool finished() {return m_finishedJob;} // Is the job done?
    double penelopeX() {return m_mainChar->getX();} // Penelope's X
    double penelopeY() {return m_mainChar->getY();} // Penelope's Y
    double ActorX(int i); // Get m_actor[i]->getX()
    double ActorY(int i); // Get m_actor[i]->getY()
    int ActorInt(int i); // Get m_actor[i]->interacting()
    void killObj(int i); // Do m_actor[i]->objDeath()
    void cureObj(int i); // Do m_actor[i]->cure()
    void infectObj(int i); // Do m_actor[i]->infect()
    void exiter(int i); // Do m_actor[i]->exiting()
    
    // Mutators
    void goToNextLevel() {m_nextLevel = true;} // Change value
    void fireFlame(); // Penelope fires flames for 2 ticks
    void plantMine(double x, double y); // Plant a mine
    void goBoom(double x, double y); // Blow up a mine
    void vomit(double x, double y, Direction d); // Zombie interactivity
    void jobFinished() {m_finishedJob = true;} // Job is finished
    void jobNotDone() {m_finishedJob = false;} // Job not done yet
    void pickUp(); // Pick up an object
    void addFlame() {m_mainChar->incFlame();} // Increase flame
    void addMine() {m_mainChar->incMineAmmo();} // Increase mine
    void addVax() {m_mainChar->incVax();} // Increase mine
    void citizenHit() {m_inf = true;}
    void zVom() {m_zVom = true;}
    
    // Calculations
    int canMove(int ignoreValue, double x, double y, double currentX, double currentY); // Can you move
    double distance(int x1, int y1, int x2, int y2) const; // Check distance
private:
    std::vector<Actor*> m_actor; // Actors only, mainly focused on Objects
    std::vector<Exit*> m_exit; // Exits
    std::vector<Pit*> m_pit; // Pits
    
    Penelope* m_mainChar; // Penelope
    
    int m_curLevel = 0; // Current level
    int m_tick; // Tick counter
    bool m_nextLevel; // next level
    int m_nCit; // Citizen
    bool m_finishedJob; // Job is done
    bool m_inf;
    bool m_zVom;
    void setText(); // Game text to set
};

#endif // STUDENTWORLD_H_
