#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
/*General in the game*/
class StudentWorld;

/* Global constants */
const int INVALID = -255;
const int IS_SECONDARY_Z = -5; // Secondary character
const int IS_SECONDARY_C = -4; // Secondary character
const int IS_MAIN = -3; // Main character
const int KILL_CHARACTER = -2; // Can kill a character
const int DO_NOTHING = -1; // Wall
const int GO_TO_NEW_LEVEL = 0; // Exits
const int MOVE_TO_NEW_SPACE = 1; // Clear to move
const int PICK_UP_OBJECT = 2; // Goodie
const int TAKE_DAMAGE = 3; // Can damage ch
const int INFECTS = 4; // Can infect someone
const int VOLATILE_OBJ = 5; // Can explode
const int INVINCIBLE = 6; // Static
const int MAIN_MOVE_SPEED = 4; // Main char movement
const int ZOMBIE_MOVE_SPEED = 1; // Zombie movement
const int CITIZEN_MOVE_SPEED = 2; // Citizen movement
const int OBJECT_PERIMETER = 16; // Object perimeter

/* Root layer */
class Actor : public GraphObject // Actor
{
public:
    // Constructor, Destructor
    Actor(int imgID, int stX, int stY,
          Direction stDir, int dpth, StudentWorld* sWorld); // Constructor
    virtual ~Actor() {} // Destructor
    
    // Getter functions
    StudentWorld* getWorld() const {return m_world;} // Return world values
    virtual int interacting() const = 0; // Return interacting value
    virtual bool getStatus() {return m_alive;} // Return alive or dead
    virtual bool isInfected() {return m_infected;} // Return if infected
    virtual int infectionCount() {return m_infection;} // Return current infection count
    virtual int getBirthtick() {return m_tick;} // Return tick of birth
    virtual bool turned() {return false;} // Has player turned yet?
    virtual bool pickedUp() {return false;} // Was object picked up?
    virtual bool exited() {return false;} // Has citizen exited?
    virtual bool canDrop() {return false;} // Can this character drop?
    
    // Mutators
    virtual void doSomething() = 0; // doSomething
    virtual void infect() {m_infected = true;} // Set infected
    virtual void cure() {m_infected = false; m_infection = 0;} // Reset infected value
    virtual void objDeath() {m_alive = false;} // obj dies
    virtual void incInfection() {m_infection++;} // Increase infection count
    virtual void exiting() {} // Does nothing, overrided by Citizen
private:
    StudentWorld* m_world; // World
    bool m_infected; // Infection status
    bool m_alive; // Life status
    int  m_infection; // Infection
    int  m_tick; // Tick born
};

/* Layer 1 */
class Danger : public Actor // Dangers
{
public:
    // Constructor, Destructor
    Danger(int imgID, int stX, int stY, int depth, StudentWorld* sWorld) :
    Actor(imgID, stX, stY, right, depth, sWorld) {} // Except for pit
    virtual ~Danger() {} // Destructor
    
    // Getter functions
    virtual int interacting() const = 0; // Return interaction value
    
    // Mutators
    virtual void doSomething() = 0; // doSomething
};

class Goodie : public Actor // Goodies
{
public:
    // Constructor, Destructor
    Goodie(int imgID, int stX, int stY, StudentWorld* sWorld) :
    Actor(imgID, stX, stY, right, 1, sWorld), m_inter(PICK_UP_OBJECT),
    m_pickup(false) {} // Constructor
    virtual ~Goodie() {} // Destructor
    
    // Getter functions
    virtual int interacting() const {return m_inter;} // Return interaction value
    virtual bool pickedUp() {return m_pickup;}
    
    // Mutators
    virtual void doSomething() = 0; // doSomething
    bool needToDo(); // Trigger that object needs to do something
private:
    const int m_inter; // Interaction value
    bool m_pickup;
};

class People : public Actor // People
{
public:
    // Constructor, Destructor
    People(int imgID, int stX, int stY, StudentWorld* sWorld) :
    Actor(imgID, stX, stY, right, 0, sWorld) {} // Constructor
    virtual ~People() {} // Destructor
    
    // Mutators
    virtual void doSomething() = 0; // doSomething
    virtual int interacting() const = 0;
};

class Projectile : public Actor // Projectiles
{
public:
    // Constructor, Destructor
    Projectile(int imgID, int stX, int stY, Direction d, StudentWorld* sWorld) :
    Actor(imgID, stX, stY, d, 0, sWorld), m_inter(TAKE_DAMAGE) {} // Constructor
    virtual ~Projectile() {} // Destructor
    
    // Getter functions
    virtual int interacting() const {return m_inter;} // Return interaction value
    
    // Mutators
    virtual void doSomething() = 0; // doSomething
private:
    const int m_inter; // Interaction value
};

class Zombie  : public Actor // Zombie
{
public:
    // Constructor, Destructor
    Zombie(int stX, int stY, StudentWorld* sWorld) :
    Actor(IID_ZOMBIE, stX, stY, right, 0, sWorld),
    m_inter(IS_SECONDARY_Z), m_distance(0), m_dir(-1) {} // Constructor
    virtual ~Zombie() {} // Destructor
    
    // Getter functions
    virtual int interacting() const {return m_inter;} // Return interaction value
    
    // Mutators
    virtual void doSomething() = 0; // doSomething
    virtual void moveZombies(int type);
private:
    const int m_inter; // Interaction value
    int m_distance; // Movement Plan
    int m_dir; // Direction to move
};

/*Level 2 Declarations*/
// Inherits Actor
class Exit : public Actor // Exit
{
public:
    // Constructor Destructor
    Exit(int stX, int stY, StudentWorld* sWorld) :
    Actor(IID_EXIT, stX, stY, right, 1, sWorld),
    m_inter(GO_TO_NEW_LEVEL), w(getWorld()) {} // Constructor
    virtual ~Exit() {} // Destructor
    
    // Getter functions
    virtual int interacting() const {return m_inter;} // Return interaction value
    
    // Mutators
    virtual void doSomething(); // doSomething
private:
    const int m_inter; // Interaction value
    bool hasOverlap(); // Is there some overlap with a person?
    StudentWorld* w;
};

class Wall : public Actor // Walls
{
public:
    // Constructor, Destructor
    Wall(int stX, int stY, StudentWorld* sWorld) :
    Actor(IID_WALL, stX, stY, right, 0, sWorld),
    m_inter(DO_NOTHING) {} // Constructor
    virtual ~Wall() {} // Destructor
    
    // Getter functions
    virtual int interacting() const {return m_inter;} // Return interaction value
    
    // Mutators
    virtual void doSomething() {} // doSomething
private:
    const int m_inter; // Interaction value
};

// Inherits Danger
class Pit : public Danger // Pit
{
public:
    // Constructor, Destructor
    Pit(int stX, int stY, StudentWorld* w) :
    Danger(IID_PIT, stX, stY, 0, w), m_inter(INVINCIBLE) {} // Constructor
    virtual ~Pit() {} // Destructor
    
    // Getter functions
    virtual int interacting() const {return m_inter;} // Return interaction value
    
    // Mutators
    virtual void doSomething(); // doSomething
private:
    const int m_inter; // Interaction value
};

class Landmine : public Danger // Landmine
{
public:
    // Constructor, Destructor
    Landmine(int stX, int stY, StudentWorld* w) :
    Danger(IID_LANDMINE, stX, stY, 0, w), m_inter(VOLATILE_OBJ),
    w(getWorld()) {} // Constructor
    virtual ~Landmine() {} // Destructor
    
    // Getter functions
    virtual int interacting() const {return m_inter;} // Return interaction value
    
    // Mutators
    virtual void doSomething(); // doSomething
private:
    bool isArmed(); // Is landmine armed?
    const int m_inter; // Interaction value
    StudentWorld* w;
};

// Inherits Goodie
class GasCan : public Goodie
{
public:
    // Constructor, Destructor
    GasCan(int stX, int stY, StudentWorld* sWorld) :
    Goodie(IID_GAS_CAN_GOODIE, stX, stY, sWorld) {} // Constructor
    virtual ~GasCan() {} // Destructor
    
    // Mutator
    virtual void doSomething(); // doSomething
};

class LandmineGoodie : public Goodie
{
public:
    // Constructor, Destructor
    LandmineGoodie(int stX, int stY, StudentWorld* sWorld) :
    Goodie(IID_LANDMINE_GOODIE, stX, stY, sWorld) {} // Constructor
    virtual ~LandmineGoodie() {} // Destructor
    
    // Mutators
    virtual void doSomething(); // doSomething
};

class Vaccine : public Goodie // Vaccine
{
public:
    // Constructor, Destructor
    Vaccine(int stX, int stY, StudentWorld* sWorld) :
    Goodie(IID_VACCINE_GOODIE, stX, stY, sWorld) {} // Constructor
    virtual ~Vaccine() {} // Destructor
    
    // Mutators
    virtual void doSomething(); // doSomething
};

// Inherits People
class Citizen : public People // Citzens to save
{
public:
    // Constructor, Destructor
    Citizen(int stX, int stY, StudentWorld* w) :
    People(IID_CITIZEN, stX, stY, w), m_inter(IS_SECONDARY_C),
    m_turn(false), m_exited(false), w(getWorld()) {} // Constructor
    virtual ~Citizen() {} // Destructor
    
    //Getter Function
    virtual int interacting() const {return m_inter;} // Return interaction value
    virtual bool turned() {return m_turn;} // Returns if citizen has turned into Zombie
    virtual bool exited() {return m_exited;} // Returns if citizen exited
    
    // Mutators
    virtual void doSomething(); // doSomething
    virtual void exiting() {m_exited = true;}
private:
    void moveC(); // Move citizen
    int m_inter; // Interaction value
    bool m_turn; // Has Citizen turned into a zombie?
    bool m_exited; // Has citizen exited?
    StudentWorld* w;
};

class Penelope : public People // Penelope
{
public:
    // Constructor, Destructor
    Penelope(int stX, int stY, StudentWorld* w) : People(IID_PLAYER, stX, stY, w), m_inter(IS_MAIN), m_vax(0), m_flameAmmo(0), m_mineAmmo(0) {} // Constructor
    virtual ~Penelope() {} // Destructor
    
    // Getter functions
    virtual int interacting() const {return m_inter;} // Return interaction value
    int getVaxCount() {return m_vax;} // Return vaccine count
    int getFlameCount() {return m_flameAmmo;} // Return flamethrower ammo
    int getMineCount() {return m_mineAmmo;} // Return how many mines are in inventory
    
    // Mutators
    virtual void doSomething(); // doSomething
    void incVax() {m_vax++;} // Increase vaccine count by one
    void decVax() {m_vax--;} // Decrease vaccine count by one
    void incFlame() {m_flameAmmo += 5;} // Increase flamethrower ammo by five
    void decFlame() {m_flameAmmo--;} // Increase flamethrower ammo by one
    void incMineAmmo() {m_mineAmmo += 2;} // Increase flamethrower ammo by two
    void decMineAmmo() {m_mineAmmo--;} // Increase flamethrower ammo by one
private:
    int m_inter; // Interaction value
    int m_vax; // Vaccines in inventory
    int m_flameAmmo; // Flamethrower ammo
    int m_mineAmmo; // Mines in inventory
};

// Inherits Projectiles
class Flame : public Projectile // Flames
{
public:
    // Constructor, Destructor
    Flame(int stX, int stY, Direction d, StudentWorld* w) :
    Projectile(IID_FLAME, stX, stY, d, w), m_inter(TAKE_DAMAGE) {} // Constructor
    virtual ~Flame() {} // Virtual destructor
    
    // Getter functions
    virtual int interacting() const {return m_inter;} // Return interaction value
    
    // Mutators
    virtual void doSomething(); // doSomething
private:
    const int m_inter; // Interaction value
};

class Vomit : public Projectile // Vomit
{
public:
    // Constructor, Destructor
    Vomit(int stX, int stY, StudentWorld* w) :
    Projectile(IID_VOMIT, stX, stY, right, w), m_inter(INFECTS) {} // Constructor
    virtual ~Vomit() {} // Virtual Destructor
    
    // Getter functions
    virtual int interacting() const {return m_inter;} // Return interaction value
    
    // Mutators
    virtual void doSomething(); // doSomething
private:
    const int m_inter; // Interaction value
};

// Inherits Zombies
class DumbZombie : public Zombie // Dumb Zombies
{
public:
    //Constructor, Destructor
    DumbZombie(int stX, int stY, StudentWorld* w) :
    Zombie(stX, stY, w) {} // Constructor
    virtual ~DumbZombie() {} // Destructor
    
    // Mutators
    virtual void doSomething(); // doSomething
    virtual bool canDrop() {return true;} // Can this character drop? ///
};

class SmartZombie : public Zombie // Smart Zombies
{
public:
    SmartZombie(int stX, int stY, StudentWorld* w) :
    Zombie(stX, stY, w) {} // Constructor
    virtual ~SmartZombie() {} // virtual destructor
    
    // Mutators
    virtual void doSomething(); // doSomething
};
#endif // ACTOR_H_
