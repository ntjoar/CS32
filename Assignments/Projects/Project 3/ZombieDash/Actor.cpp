#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include "Level.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
/* Root Level Constructor */
Actor::Actor(int imgID, int stX, int stY, Direction stDir, int dpth,
             StudentWorld* sWorld) : GraphObject(imgID, stX, stY, stDir, dpth),
m_world(sWorld), m_alive(true), m_infection(0) // Constructor
{
    m_tick = m_world->getTick(); // "Birthtick is current tick obj was constructed" -Smallberg
}

/*Level 2 Implementations*/
// Inherits Actor
void Exit::doSomething() // Exit doSomething
{
    if(!getStatus()) // If status is dead
        return;
    
    if(this->hasOverlap()) // If this has overlap
        w->goToNextLevel(); // Set go to next level possible
}

bool Exit::hasOverlap() // Exit has overlap or not
{
    for(int i = 0; w->ActorInt(i) != INVALID; i++) // Repeatedly
        if(w->ActorInt(i) == IS_SECONDARY_C || w->ActorInt(i) == IS_MAIN) // If actor is main or citizen
        {
            if(w->distance(this->getX(), this->getY(), w->ActorX(i), w->ActorY(i)) <= 10 &&
               w->ActorInt(i) == IS_SECONDARY_C) // Overlap with character
            {
                w->exiter(i);  // Citizen is exiting
                w->killObj(i); // Save citizen
                return true;  // There is overlap
            }
            else if(w->distance(this->getX(), this->getY(), w->ActorX(i), w->ActorY(i)) <= 10 &&
                    w->ActorInt(i) == IS_MAIN) // Overlap with character
            {
                w->jobFinished(); // Job done yet?
                return true;  // There is overlap
            }
        }
    return false; // No overlap
}

// Inherits Danger
void Landmine::doSomething() // Landmine doSomething
{
    if(!getStatus()) // If object is dead
        return; // Do nothing
    
    int k = w->canMove(VOLATILE_OBJ, this->getX(), this->getY(), this->getX(), this->getY()); // What is at this position?
    if(k == TAKE_DAMAGE) // If flame overlaps before armed
        this->objDeath(); // Blow up
    if(this->isArmed()) // Check if armed
    {
        for(int i = 0; w->ActorInt(i) != INVALID; i++)
        {
            if((w->ActorInt(i) == IS_SECONDARY_C || w->ActorInt(i) == IS_MAIN ||
                w->ActorInt(i) == IS_SECONDARY_Z) &&
               w->distance(w->ActorX(i), w->ActorY(i), this->getX(), this->getY()) <= 10)
                this->objDeath();
        }
    }
}

bool Landmine::isArmed() // Is landmine armed
{
    if(getBirthtick() + 30 <= w->getTick()) // If 30 ticks passed from place
        return true; // Landmine armed
    return false; // Landmine not armed
}

void Pit::doSomething() // Pit doSomething
{
    if(!getStatus()) // If dead
        return; // Do nothing
    
    StudentWorld* w = getWorld();
    for(int i = 0; w->ActorInt(i) != INVALID; i++) // Repeatedly
        if(w->distance(w->ActorX(i), w->ActorY(i), this->getX(), this->getY()) <= 10 &&
           (w->ActorInt(i) == IS_MAIN || w->ActorInt(i) == IS_SECONDARY_Z
            || w->ActorInt(i) == IS_SECONDARY_C)) // Searching for any overlap
        {
            w->cureObj(i); // They become not infected
            w->killObj(i); // Kill them
        }
}

// Inherits Goodies
bool Goodie::needToDo() // If there is overlap
{
    if(!getStatus()) // If object is dead
        return false; // Don't do anything
    
    StudentWorld* w = getWorld();
    if(w->distance(w->penelopeX(), w->penelopeY(), this->getX(), this->getY()) <= 10) // Penelope overlaps
    {
        m_pickup = true;
        return true; // Pickup allowed
    }
    return false; // Pickup not allowed
}

void GasCan::doSomething() // Gas can overlap
{
    if(!getStatus()) // If object is dead
        return; // Don't do anything
    if(needToDo()) // If overlap
    {
        StudentWorld* w = getWorld();
        w->addFlame(); // Pick up and add necessary
        this->objDeath(); // Object dies
    }
    return; // End
}

void LandmineGoodie::doSomething()
{
    if(!getStatus()) // If object is dead
        return; // Don't do anything
    if(needToDo()) // If overlap
    {
        StudentWorld* w = getWorld();
        w->addMine();// Pick up and add necessary
        this->objDeath(); // Object dies
    }
    return; // End
}

void Vaccine::doSomething()
{
    StudentWorld* w = getWorld();
    if(!getStatus()) // If object is dead
        return; // Don't do anything
    if(needToDo()) // If overlap
    {
        w->addVax();
        this->objDeath(); // Object dies
    }
    return; // End
}

// Inherits People
void Citizen::doSomething() // Citizen does something
{
    if(isInfected()) // If citizen is infected
        incInfection(); // Increase infection count
    
    if(infectionCount() >= 500) // If infection is over 500
    {
        this->m_turn = true;
        this->objDeath(); // Citizen dies
        return;
    }
    
    if(!getStatus() || (w->getTick() - getBirthtick()) % 2 == 0) // Every other tick upon creation
        return; // This is paralyzed
    
    this->moveC(); // Character move
}

void Citizen::moveC() // Move character
{
    // 1 up
    // 2 down
    // 3 left
    // 4 right
    double closestZX = -1;
    double closestZY = -1;
    
    int mvTo = 0;
    double xPos = getX();
    double yPos = getY();
    
    double p_dist = w->distance(xPos, yPos, w->penelopeX(), w->penelopeY()); // Get distance to penelope
    double z_dist = -1;
    for(int i = 0; w->ActorInt(i) != INVALID; i++) // Repeatedly
    {
        double newDist = w->distance(xPos, yPos, w->ActorX(i), w->ActorY(i));
        if(z_dist == -1 && w->ActorInt(i) == IS_SECONDARY_Z) // In the first case
        {
            z_dist = newDist; // Copy distance
            closestZX = w->ActorX(i); // Allocate closest zombie
            closestZY = w->ActorY(i);
            continue;
        }
        if(z_dist >= newDist && w->ActorInt(i) == IS_SECONDARY_Z) // If object is a zombie
        {
            z_dist = newDist; // Copy distance
            closestZX = w->ActorX(i);
            closestZY = w->ActorY(i); // Allocate closest zombie
        }
    }
    
    if(p_dist <= 80 && p_dist < z_dist) // Determine where to move closer to P
    {
        if(xPos == w->penelopeX()) // If x are the same
        {
            if(yPos < w->penelopeY()) // If closer to move up
                mvTo = 1; // Move up
            else // Otherwise
                mvTo = 2; // Move down
        }
        else if(yPos == w->penelopeY()) // If y are the same
        {
            if(xPos > w->penelopeX()) //If closer to move left
                mvTo = 3; // Move left
            else // Otherwise
                mvTo = 4; // Move right
        }
        else
        {
            int i = randInt(1, 2);
            if(w->distance(xPos, yPos + CITIZEN_MOVE_SPEED, w->penelopeX(), w->penelopeY()) <
               w->distance(xPos, yPos - CITIZEN_MOVE_SPEED, w->penelopeX(), w->penelopeY())) // If closer to move up
            {
                if(w->distance(xPos + CITIZEN_MOVE_SPEED, yPos, w->penelopeX(), w->penelopeY()) <
                   w->distance(xPos - CITIZEN_MOVE_SPEED, yPos, w->penelopeX(), w->penelopeY())) // If closer to move right
                {
                    if(i == 1) // Random movement is up
                        mvTo = 1; // Move up
                    else // Otherwise
                        mvTo = 4; // Move right
                }
                else // Otherwise
                {
                    if(i == 1) // Random movement is up
                        mvTo = 1; // Move up
                    else // Otherwise
                        mvTo = 3; // Move left
                }
            }
            else // Otherwise
            {
                if(w->distance(xPos, yPos - CITIZEN_MOVE_SPEED, w->penelopeX(), w->penelopeY()) <
                   w->distance(xPos, yPos + CITIZEN_MOVE_SPEED, w->penelopeX(), w->penelopeY())) // If closer to move down
                {
                    if(w->distance(xPos - CITIZEN_MOVE_SPEED, yPos, w->penelopeX(), w->penelopeY()) <
                       w->distance(xPos + CITIZEN_MOVE_SPEED, yPos, w->penelopeX(), w->penelopeY())) // If closer to move left
                    {
                        if(i == 1) // Random choice is down
                            mvTo = 2; // Move right
                        else // Otherwise
                            mvTo = 3; // Move left
                    }
                    else // Otherwise
                    {
                        if(i == 1) // Random choice is down
                            mvTo = 2; // Move right
                        else // Otherwise
                            mvTo = 4; // Move left
                    }
                }
            }
        }
    }
    else if(z_dist <= 80) // If zombie within 80 pixels
    {
        if(xPos == closestZX) // Zombie x same as citizen x
        {
            if(yPos > closestZY) // Closer to move down
                mvTo = 1; // Move up
            else // Otherwise
                mvTo = 2;
        }
        else if(yPos == closestZY) // Zombie y same as citizen y
        {
            if(xPos < closestZX) // Closer to move right
                mvTo = 3; // Move left
            else // Otherwise
                mvTo = 4; // Move right
        }
        else // Otherwise
        {
            if(w->distance(xPos, yPos + CITIZEN_MOVE_SPEED, closestZX, closestZY) >
               w->distance(xPos, yPos - CITIZEN_MOVE_SPEED, closestZX, closestZY)) // Farther to move up
            {
                if(w->distance(xPos + CITIZEN_MOVE_SPEED, yPos, closestZX, closestZY) >
                   w->distance(xPos - CITIZEN_MOVE_SPEED, yPos, closestZX, closestZY)) // Farther to move right
                {
                    if(randInt(1, 2) == 1) // Randomly chosen decision is up
                        mvTo = 1; // Move up
                    else // Otherwise
                        mvTo = 4; // Move right
                }
                else // Otherwise
                {
                    if(randInt(1, 2) == 1) // Randomly chosen decision is up
                        mvTo = 1; // Move up
                    else // Otherwise
                        mvTo = 3; // Move left
                }
            }
            else // Otherwise
            {
                if(w->distance(xPos - CITIZEN_MOVE_SPEED, yPos, closestZX, closestZY) >
                   w->distance(xPos + CITIZEN_MOVE_SPEED, yPos, closestZX, closestZY)) // Farther to move left
                {
                    if(randInt(1, 2) == 1) // Randomly chosen decision is down
                        mvTo = 2; // Move down
                    else // Otherwise
                        mvTo = 3; // Move right
                }
                else // Otherwise
                {
                    if(randInt(1, 2) == 1)  // Randomly chosen decision is down
                        mvTo = 2; // Move down
                    else // Otherwise
                        mvTo = 4; // Move right
                }
            }
        }
    }
    switch (mvTo) // Switch move
    {
        case 1: // Move told to go up
        {
            this->setDirection(up); // Set Direction
            if(w->canMove(IS_SECONDARY_C, xPos, yPos + CITIZEN_MOVE_SPEED, xPos, yPos) != IS_MAIN &&
               w->canMove(IS_SECONDARY_C, xPos, yPos + CITIZEN_MOVE_SPEED, xPos, yPos) != IS_SECONDARY_C &&
               w->canMove(IS_SECONDARY_C, xPos, yPos + CITIZEN_MOVE_SPEED, xPos, yPos) != DO_NOTHING &&
               w->canMove(IS_SECONDARY_C, xPos, yPos + CITIZEN_MOVE_SPEED, xPos, yPos) != IS_SECONDARY_Z) // If nothing blocks
                this->moveTo(xPos, yPos + CITIZEN_MOVE_SPEED); // Move character
            break;
        }
        case 2: // Move told to go down
        {
            this->setDirection(down); // Set Direction
            if(w->canMove(IS_SECONDARY_C, xPos, yPos - CITIZEN_MOVE_SPEED, xPos, yPos) != IS_MAIN &&
               w->canMove(IS_SECONDARY_C, xPos, yPos - CITIZEN_MOVE_SPEED, xPos, yPos) != IS_SECONDARY_C &&
               w->canMove(IS_SECONDARY_C, xPos, yPos - CITIZEN_MOVE_SPEED, xPos, yPos) != DO_NOTHING &&
               w->canMove(IS_SECONDARY_C, xPos, yPos - CITIZEN_MOVE_SPEED, xPos, yPos) != IS_SECONDARY_Z) // If nothing blocks
                this->moveTo(xPos, yPos - CITIZEN_MOVE_SPEED); // Move character
            break; // Break
        }
        case 3: // Move told to go left
        {
            this->setDirection(left); // Set Direction
            if(w->canMove(IS_SECONDARY_C, xPos - CITIZEN_MOVE_SPEED, yPos, xPos, yPos) != IS_MAIN &&
               w->canMove(IS_SECONDARY_C, xPos - CITIZEN_MOVE_SPEED, yPos, xPos, yPos) != IS_SECONDARY_C &&
               w->canMove(IS_SECONDARY_C, xPos - CITIZEN_MOVE_SPEED, yPos, xPos, yPos) != DO_NOTHING &&
               w->canMove(IS_SECONDARY_C, xPos - CITIZEN_MOVE_SPEED, yPos, xPos, yPos) != IS_SECONDARY_Z) // If nothing blocks
                this->moveTo(xPos - CITIZEN_MOVE_SPEED, yPos); // Move character
            break; // Break
        }
        case 4: // Move told to go right
        {
            this->setDirection(right); // Set Direction
            if(w->canMove(IS_SECONDARY_C, xPos + CITIZEN_MOVE_SPEED, yPos, xPos, yPos) != IS_MAIN &&
               w->canMove(IS_SECONDARY_C, xPos + CITIZEN_MOVE_SPEED, yPos, xPos, yPos) != IS_SECONDARY_C &&
               w->canMove(IS_SECONDARY_C, xPos + CITIZEN_MOVE_SPEED, yPos, xPos, yPos) != DO_NOTHING &&
               w->canMove(IS_SECONDARY_C, xPos + CITIZEN_MOVE_SPEED, yPos, xPos, yPos) != IS_SECONDARY_Z) // If nothing blocks
                this->moveTo(xPos + CITIZEN_MOVE_SPEED, yPos); // Move character
            break; // Break
        }
        default: // Default
            break; // Do nothing
    }
}

void Penelope::doSomething() // Penelope does something
{
    if(!getStatus()) // If dead
        return; // Do nothing
    StudentWorld* w = getWorld();
    
    if(isInfected()) // If infected
        incInfection(); // Increase infection count
    
    int keyPressed;
    if(w->getKey(keyPressed)) // If key pressed
    {
        w->getKey(keyPressed);
        int xPos = getX();
        int yPos = getY();
        switch (keyPressed)
        {
            case KEY_PRESS_UP: // If key up
            {
                this->setDirection(up); // Set direction
                if(w->canMove(IS_MAIN, xPos, yPos + MAIN_MOVE_SPEED, xPos, yPos) != DO_NOTHING &&
                   w->canMove(IS_MAIN, xPos, yPos + MAIN_MOVE_SPEED, xPos, yPos) != IS_SECONDARY_C &&
                   w->canMove(IS_MAIN, xPos, yPos + MAIN_MOVE_SPEED, xPos, yPos) != IS_SECONDARY_Z) // If nothing blocking
                    this->moveTo(xPos, yPos + MAIN_MOVE_SPEED); // Move character
                break; // Break
            }
            case KEY_PRESS_DOWN: // If key down
            {
                this->setDirection(down); // Set direction
                if(w->canMove(IS_MAIN, xPos, yPos - MAIN_MOVE_SPEED, xPos, yPos) != DO_NOTHING &&
                   w->canMove(IS_MAIN, xPos, yPos - MAIN_MOVE_SPEED, xPos, yPos) != IS_SECONDARY_C &&
                   w->canMove(IS_MAIN, xPos, yPos - MAIN_MOVE_SPEED, xPos, yPos) != IS_SECONDARY_Z) // If nothing blocking
                    this->moveTo(xPos, yPos - MAIN_MOVE_SPEED); // Move character
                break; // Break
            }
            case KEY_PRESS_LEFT: // If key left
            {
                this->setDirection(left); // Set direction
                if(w->canMove(IS_MAIN, xPos - MAIN_MOVE_SPEED, yPos, xPos, yPos) != DO_NOTHING &&
                   w->canMove(IS_MAIN, xPos - MAIN_MOVE_SPEED, yPos, xPos, yPos) != IS_SECONDARY_C &&
                   w->canMove(IS_MAIN, xPos - MAIN_MOVE_SPEED, yPos, xPos, yPos) != IS_SECONDARY_Z) // If nothing blocking
                    this->moveTo(xPos - MAIN_MOVE_SPEED, yPos); // Move character
                break; // Break
            }
            case KEY_PRESS_RIGHT: // If key right
            {
                this->setDirection(right); // Set direction
                if(w->canMove(IS_MAIN, xPos + MAIN_MOVE_SPEED, yPos, xPos, yPos) != DO_NOTHING &&
                   w->canMove(IS_MAIN, xPos + MAIN_MOVE_SPEED, yPos, xPos, yPos) != IS_SECONDARY_C &&
                   w->canMove(IS_MAIN, xPos + MAIN_MOVE_SPEED, yPos, xPos, yPos) != IS_SECONDARY_Z) // If nothing blocking
                    this->moveTo(xPos + MAIN_MOVE_SPEED, yPos); // Move character
                break; // Break
            }
            case KEY_PRESS_SPACE: // If key space
            {
                if(m_flameAmmo > 0) // If you have a flame
                {
                    w->fireFlame(); // Fire and decrease flame count by 1
                    decFlame();
                }
                break; // Break
            }
            case KEY_PRESS_TAB: // If key tab
            {
                if(m_mineAmmo > 0) // If you have mines
                {
                    w->plantMine(getX(), getY()); // Plant mine and decrease mine count by 1
                }
                break; // Break
            }
            case KEY_PRESS_ENTER: // If key Enter
            {
                if(getVaxCount() > 0) // If you have a vaccine
                {
                    decVax(); // Cure yourself and decrease vaccine count
                    this->cure();
                }
                break; // Break
            }
            default: // Default
                break; // Do nothing
        }
    }
}

// Inherits Projectiles
void Flame::doSomething()
{
    StudentWorld* w = getWorld();
    
    if(!getStatus()) // If dead
        return; // Do nothing
    if(w->getTick() > getBirthtick()) // If more than 2 ticks
        this->objDeath(); // It dies
    
    for(int i = 0; w->ActorInt(i) != INVALID; i++) // Repeatedly
    {
        if(w->ActorInt(i) != IS_SECONDARY_C && w->ActorInt(i) != IS_SECONDARY_Z &&
           w->ActorInt(i) != IS_MAIN && w->ActorInt(i) != PICK_UP_OBJECT &&
           w->ActorInt(i) != VOLATILE_OBJ) // If overlap with invincibile
            continue; // Skip
        if(w->distance(w->ActorX(i), w->ActorY(i), this->getX(), this->getY()) <= 10) // If overlap
            w->killObj(i); // Kill the object
    }
}

void Vomit::doSomething() // Vomit do something
{
    StudentWorld* w = getWorld();
    if(!getStatus()) // If dead
        return; // Do nothing
    if(w->getTick() > getBirthtick()) // If more than 2 ticks
        this->objDeath(); // It dies
    
    for(int i = 0; w->ActorInt(i) != INVALID; i++) // Repeatedly
        if(w->ActorInt(i) == IS_SECONDARY_C || w->ActorInt(i) == IS_MAIN) // If actor is main or citizen
        {
            if(w->distance(getX(), getY(), w->ActorX(i), w->ActorY(i)) <= 10) // Overlap with character
            {
                if(w->ActorInt(i) == IS_SECONDARY_C)
                    w->citizenHit();
                w->infectObj(i);  // Person is infected
                break;
            }
        }
}

// Inherits Zombies
void Zombie::moveZombies(int type)
{
    StudentWorld* w = getWorld();
    bool vomited = false;
    for(int i = 0; w->ActorInt(i) != INVALID; i++)
    {
        if(w->ActorInt(i) != IS_MAIN && w->ActorInt(i) != IS_SECONDARY_C)
            continue;
        switch(m_dir) // Dist 26  bc of width and height taken into consideration
        {
            case 1:
            {
                if((w->canMove(IS_SECONDARY_Z, getX(), getY() + SPRITE_HEIGHT, getX(), getY()) == IS_SECONDARY_C ||
                    w->canMove(IS_SECONDARY_Z, getX(), getY() + SPRITE_HEIGHT, getX(), getY()) == IS_MAIN) &&
                   (w->ActorInt(i) == IS_SECONDARY_C || w->ActorInt(i) == IS_MAIN) &&
                   (w->distance(getX(), getY(), w->ActorX(i), w->ActorY(i)) <= 26)) // Zombie is by a character
                {
                    if(randInt(1, 3) == 1 && this->getDirection() == up) // Zombie is facing there
                    {
                        vomited = true; // Vomited
                        w->vomit(getX(), getY(), getDirection());
                    }
                }
                break;
            }
            case 2:
            {
                if((w->canMove(IS_SECONDARY_Z, getX(), getY() - SPRITE_HEIGHT, getX(), getY()) == IS_SECONDARY_C ||
                    w->canMove(IS_SECONDARY_Z, getX(), getY() - SPRITE_HEIGHT, getX(), getY()) == IS_MAIN) &&
                   (w->ActorInt(i) == IS_SECONDARY_C || w->ActorInt(i) == IS_MAIN) &&
                   (w->distance(getX(), getY(), w->ActorX(i), w->ActorY(i)) <= 26)) // Zombie is by a character
                {
                    if(randInt(1, 3) == 1 && this->getDirection() == down) // Zombie is facing there
                    {
                        vomited = true; // Vomited
                        w->vomit(getX(), getY(), getDirection());
                    }
                }
                break;
            }
            case 3:
            {
                if((w->canMove(IS_SECONDARY_Z, getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT, getX(), getY()) == IS_SECONDARY_C ||
                    w->canMove(IS_SECONDARY_Z, getX() - SPRITE_WIDTH, getY(), getX(), getY()) == IS_MAIN) &&
                   (w->ActorInt(i) == IS_SECONDARY_C || w->ActorInt(i) == IS_MAIN) &&
                   (w->distance(getX(), getY(), w->ActorX(i), w->ActorY(i)) <= 26)) // Zombie is by a character
                {
                    if(randInt(1, 3) == 1 && this->getDirection() == left) // Zombie is facing there
                    {
                        vomited = true; // Vomited
                        w->vomit(getX(), getY(), getDirection());
                    }
                }
                break;
            }
            case 4:
            {
                if((w->canMove(IS_SECONDARY_Z, getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT, getX(), getY()) == IS_SECONDARY_C ||
                    w->canMove(IS_SECONDARY_Z, getX() + SPRITE_WIDTH, getY(), getX(), getY()) == IS_MAIN) &&
                   (w->ActorInt(i) == IS_SECONDARY_C || w->ActorInt(i) == IS_MAIN) &&
                   (w->distance(getX(), getY(), w->ActorX(i), w->ActorY(i)) <= 26)) // Zombie is by a character
                {
                    if(randInt(1, 3) == 1 && this->getDirection() == right) // Zombie is facing there
                    {
                        vomited = true; // Vomited
                        w->vomit(getX(), getY(), getDirection());
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    
    if(vomited) // Do nothing after vomiting
    {
        return; // Return immediately
    }
    
    if(m_distance == 0) // If movement plan is zero
    {
        m_distance = randInt(3, 10); // Set random distance
        m_dir = randInt(1, 4); // U, D, L, R
    }
    
    double xPos = this->getX();
    double yPos = this->getY();
    
    if(type == 0)
    {
        double closestCX = -1;
        double closestCY = -1;
        
        double p_dist = w->distance(xPos, yPos, w->penelopeX(), w->penelopeY()); // Get distance to penelope
        double c_dist = 100000;
        for(int i = 0; w->ActorInt(i) != INVALID; i++) // Repeatedly
        {
            double newDist = w->distance(xPos, yPos, w->ActorX(i), w->ActorY(i));
            if(c_dist == -1 && w->ActorInt(i) == IS_SECONDARY_C) // In the first case
            {
                c_dist = newDist; // Copy distance
                closestCX = w->ActorX(i); // Allocate closest zombie
                closestCY = w->ActorY(i);
                continue;
            }
            if(c_dist >= newDist && w->ActorInt(i) == IS_SECONDARY_C) // If object is a zombie
            {
                c_dist = newDist; // Copy distance
                closestCX = w->ActorX(i);
                closestCY = w->ActorY(i); // Allocate closest zombie
            }
        }
        
        if(c_dist >= p_dist && p_dist <= 80)
        {
            if(xPos == w->penelopeX()) // If x are the same
            {
                if(yPos < w->penelopeY()) // If closer to move up
                    m_dir = 1; // Move up
                else // Otherwise
                    m_dir = 2; // Move down
            }
            else if(yPos == w->penelopeY()) // If y are the same
            {
                if(xPos > w->penelopeX()) //If closer to move left
                    m_dir = 3; // Move left
                else // Otherwise
                    m_dir = 4; // Move right
            }
            else
            {
                int i = randInt(1, 2);
                if(w->distance(xPos, yPos + ZOMBIE_MOVE_SPEED, w->penelopeX(), w->penelopeY()) <
                   w->distance(xPos, yPos - ZOMBIE_MOVE_SPEED, w->penelopeX(), w->penelopeY())) // If closer to move up
                {
                    if(w->distance(xPos + ZOMBIE_MOVE_SPEED, yPos, w->penelopeX(), w->penelopeY()) <
                       w->distance(xPos - ZOMBIE_MOVE_SPEED, yPos, w->penelopeX(), w->penelopeY())) // If closer to move right
                    {
                        if(i == 1) // Random movement is up
                            m_dir = 1; // Move up
                        else // Otherwise
                            m_dir = 4; // Move right
                    }
                    else // Otherwise
                    {
                        if(i == 1) // Random movement is up
                            m_dir = 1; // Move up
                        else // Otherwise
                            m_dir = 3; // Move left
                    }
                }
                else // Otherwise
                {
                    if(w->distance(xPos, yPos - ZOMBIE_MOVE_SPEED, w->penelopeX(), w->penelopeY()) <
                       w->distance(xPos, yPos + ZOMBIE_MOVE_SPEED, w->penelopeX(), w->penelopeY())) // If closer to move down
                    {
                        if(w->distance(xPos - ZOMBIE_MOVE_SPEED, yPos, w->penelopeX(), w->penelopeY()) <
                           w->distance(xPos + ZOMBIE_MOVE_SPEED, yPos, w->penelopeX(), w->penelopeY())) // If closer to move left
                        {
                            if(i == 1) // Random choice is down
                                m_dir = 2; // Move right
                            else // Otherwise
                                m_dir = 3; // Move left
                        }
                        else // Otherwise
                        {
                            if(i == 1) // Random choice is down
                                m_dir = 2; // Move right
                            else // Otherwise
                                m_dir = 4; // Move left
                        }
                    }
                }
            }
        }
        else if(c_dist <= 80)
        {
            if(xPos == closestCX) // If x are the same
            {
                if(yPos < closestCY) // If closer to move up
                    m_dir = 1; // Move up
                else // Otherwise
                    m_dir = 2; // Move down
            }
            else if(yPos == closestCY) // If y are the same
            {
                if(xPos > closestCX) //If closer to move left
                    m_dir = 3; // Move left
                else // Otherwise
                    m_dir = 4; // Move right
            }
            else
            {
                int i = randInt(1, 2);
                if(w->distance(xPos, yPos + ZOMBIE_MOVE_SPEED, closestCX, closestCY) <
                   w->distance(xPos, yPos - ZOMBIE_MOVE_SPEED, closestCX, closestCY)) // If closer to move up
                {
                    if(w->distance(xPos + ZOMBIE_MOVE_SPEED, yPos, closestCX, closestCY) <
                       w->distance(xPos - ZOMBIE_MOVE_SPEED, yPos, closestCX, closestCY)) // If closer to move right
                    {
                        if(i == 1) // Random movement is up
                            m_dir = 1; // Move up
                        else // Otherwise
                            m_dir = 4; // Move right
                    }
                    else // Otherwise
                    {
                        if(i == 1) // Random movement is up
                            m_dir = 1; // Move up
                        else // Otherwise
                            m_dir = 3; // Move left
                    }
                }
                else // Otherwise
                {
                    if(w->distance(xPos, yPos - ZOMBIE_MOVE_SPEED, closestCX, closestCY) <
                       w->distance(xPos, yPos + ZOMBIE_MOVE_SPEED, closestCX, closestCY)) // If closer to move down
                    {
                        if(w->distance(xPos - ZOMBIE_MOVE_SPEED, yPos, closestCX, closestCY) <
                           w->distance(xPos + ZOMBIE_MOVE_SPEED, yPos, closestCX, closestCY)) // If closer to move left
                        {
                            if(i == 1) // Random choice is down
                                m_dir = 2; // Move right
                            else // Otherwise
                                m_dir = 3; // Move left
                        }
                        else // Otherwise
                        {
                            if(i == 1) // Random choice is down
                                m_dir = 2; // Move right
                            else // Otherwise
                                m_dir = 4; // Move left
                        }
                    }
                }
            }
        }
    }
        
    switch(m_dir)
    {
        case 1: // Move told to go up
        {
            this->setDirection(up); // Set Direction
            if(w->canMove(IS_SECONDARY_Z, xPos, yPos + ZOMBIE_MOVE_SPEED, xPos, yPos) != IS_MAIN &&
               w->canMove(IS_SECONDARY_Z, xPos, yPos + ZOMBIE_MOVE_SPEED, xPos, yPos) != IS_SECONDARY_C &&
               w->canMove(IS_SECONDARY_Z, xPos, yPos + ZOMBIE_MOVE_SPEED, xPos, yPos) != DO_NOTHING &&
               w->canMove(IS_SECONDARY_Z, xPos, yPos + ZOMBIE_MOVE_SPEED, xPos, yPos) != IS_SECONDARY_Z) // If nothing blocks
            {
                this->moveTo(xPos, yPos + ZOMBIE_MOVE_SPEED); // Move character
                m_distance--;
            }
            else
                m_distance = 0;
            break;  // Break
        }
        case 2: // Move told to go down
        {
            this->setDirection(down); // Set Direction
            if(w->canMove(IS_SECONDARY_Z, xPos, yPos - ZOMBIE_MOVE_SPEED, xPos, yPos) != IS_MAIN &&
               w->canMove(IS_SECONDARY_Z, xPos, yPos - ZOMBIE_MOVE_SPEED, xPos, yPos) != IS_SECONDARY_C &&
               w->canMove(IS_SECONDARY_Z, xPos, yPos - ZOMBIE_MOVE_SPEED, xPos, yPos) != DO_NOTHING &&
               w->canMove(IS_SECONDARY_Z, xPos, yPos - ZOMBIE_MOVE_SPEED, xPos, yPos) != IS_SECONDARY_Z) // If nothing blocks
            {
                this->moveTo(xPos, yPos - CITIZEN_MOVE_SPEED); // Move character
                m_distance--;
            }
            else
                m_distance = 0;
            break; // Break
        }
        case 3: // Move told to go left
        {
            this->setDirection(left); // Set Direction
            if(w->canMove(IS_SECONDARY_Z, xPos - ZOMBIE_MOVE_SPEED, yPos, xPos, yPos) != IS_MAIN &&
               w->canMove(IS_SECONDARY_Z, xPos - ZOMBIE_MOVE_SPEED, yPos, xPos, yPos) != IS_SECONDARY_C &&
               w->canMove(IS_SECONDARY_Z, xPos - ZOMBIE_MOVE_SPEED, yPos, xPos, yPos) != DO_NOTHING &&
               w->canMove(IS_SECONDARY_Z, xPos - ZOMBIE_MOVE_SPEED, yPos, xPos, yPos) != IS_SECONDARY_Z) // If nothing blocks
            {
                this->moveTo(xPos - CITIZEN_MOVE_SPEED, yPos); // Move character
                m_distance--;
            }
            else
                m_distance = 0;
            break; // Break
        }
        case 4: // Move told to go right
        {
            this->setDirection(right); // Set Direction
            if(w->canMove(IS_SECONDARY_Z, xPos + ZOMBIE_MOVE_SPEED, yPos, xPos, yPos) != IS_MAIN &&
               w->canMove(IS_SECONDARY_Z, xPos + ZOMBIE_MOVE_SPEED, yPos, xPos, yPos) != IS_SECONDARY_C &&
               w->canMove(IS_SECONDARY_Z, xPos + ZOMBIE_MOVE_SPEED, yPos, xPos, yPos) != DO_NOTHING &&
               w->canMove(IS_SECONDARY_Z, xPos + ZOMBIE_MOVE_SPEED, yPos, xPos, yPos) != IS_SECONDARY_Z) // If nothing blocks
            {
                this->moveTo(xPos + CITIZEN_MOVE_SPEED, yPos); // Move character
                m_distance--;
            }
            else
                m_distance = 0;
            break; // Break
        }
    }
}

void SmartZombie::doSomething()
{
    StudentWorld* w = getWorld();
    if(!getStatus()) // If dead
        return; // Do nothing
    if((w->getTick() - getBirthtick()) % 2 != 0)   // Every odd tick a zombie was introduced
        this->moveZombies(0); // Move
}

void DumbZombie::doSomething()
{
    StudentWorld* w = getWorld();
    if(!getStatus()) // If dead
        return;
    
    if((w->getTick() - getBirthtick()) % 2 != 0) // Every odd tick a zombie was introduced
        this->moveZombies(1); // Move
}
