#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath) // Default
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp
double StudentWorld::ActorX(int i)
{
    if(i < 0 || i >= m_actor.size())
        return INVALID;
    return m_actor[i]->getX();
}

double StudentWorld::ActorY(int i)
{
    if(i < 0 || i >= m_actor.size())
        return INVALID;
    return m_actor[i]->getY();
}

int StudentWorld::ActorInt(int i)
{
    if(i < 0 || i >= m_actor.size())
        return INVALID;
    return m_actor[i]->interacting();
}

void StudentWorld::killObj(int i)
{
    if(i < 0 || i >= m_actor.size())
        return;
    return m_actor[i]->objDeath();
}

void StudentWorld::cureObj(int i)
{
    if(i < 0 || i >= m_actor.size())
        return;
    return m_actor[i]->cure();
}

void StudentWorld::infectObj(int i)
{
    if(i < 0 || i >= m_actor.size())
        return;
    return m_actor[i]->infect();
}

void StudentWorld::exiter(int i)
{
    if(i < 0 || i >= m_actor.size())
        return;
    return m_actor[i]->exiting();
}

StudentWorld::StudentWorld(string assetPath) // Default
: GameWorld(assetPath)
{}

StudentWorld::~StudentWorld() // Destroy everything
{
    cleanUp(); // Call cleanup
}

int StudentWorld::init() // Initialize the level
{
    m_tick = 1; // Tick starts at 1
    m_nCit = 0; // 0 citizens
    m_finishedJob = false; // All citizens gone?
    
    Level lev(assetPath()); // Load level
    string i = to_string(getLevel());
    string levelFile = "level0"+i+".txt";
    Level::LoadResult result = lev.loadLevel(levelFile);
    if(result == Level::load_fail_file_not_found) // If Error 404
    {
        if(m_curLevel <= 0) // Have not done one level at least
        {
            cerr << "Cannot find " << levelFile << " data file" << endl;
            return GWSTATUS_LEVEL_ERROR; // Return error status
        }
        if(m_curLevel > 0) // Have done more than one level
            return GWSTATUS_PLAYER_WON; // Player wins
    }
    else if(stoi(i) > 99) // If over 99 levels
        return GWSTATUS_PLAYER_WON; // Player wins
    else if(result == Level::load_fail_bad_format) // Error 504
    {
        cerr << "Your level was improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if(result == Level::load_success) // No error
    {
        m_zVom = false;
        m_inf = false;
        m_curLevel++; // Increment current level
        m_nextLevel = false;
        //Initializing walls
        for (int k = 0; k < 16; k++) // For all level values
            for (int l = 0; l < 16; l++) // For all level values
            {
                Level::MazeEntry sign = lev.getContentsOf(k, l); // Get sign and make the appropriate character
                if(sign == Level::player)
                {
                    m_mainChar = new Penelope(k * SPRITE_WIDTH, l * SPRITE_HEIGHT, this);
                    m_actor.push_back(m_mainChar);
                    m_mainChar->cure(); // Keep Penelope uninfected on initialization
                }
                else if(sign == Level::wall)
                    m_actor.push_back(new Wall(k * SPRITE_WIDTH, l * SPRITE_HEIGHT, this));
                else if(sign == Level::exit)
                {
                    m_exit.push_back(new Exit(k * SPRITE_WIDTH, l * SPRITE_HEIGHT, this));
                    m_actor.push_back(m_exit.back());
                }
                else if(sign == Level::landmine_goodie)
                    m_actor.push_back(new LandmineGoodie(k * SPRITE_WIDTH, l * SPRITE_HEIGHT, this));
                else if(sign == Level::gas_can_goodie)
                    m_actor.push_back(new GasCan(k * SPRITE_WIDTH, l * SPRITE_HEIGHT, this));
                else if(sign == Level::vaccine_goodie)
                    m_actor.push_back(new Vaccine(k * SPRITE_WIDTH, l * SPRITE_HEIGHT, this));
                else if(sign == Level::pit)
                {
                    m_pit.push_back(new Pit(k * SPRITE_WIDTH, l * SPRITE_HEIGHT, this));
                    m_actor.push_back(m_pit.back());
                }
                else if(sign == Level::smart_zombie)
                    m_actor.push_back(new SmartZombie(k * SPRITE_WIDTH, l * SPRITE_HEIGHT, this));
                else if(sign == Level::dumb_zombie)
                    m_actor.push_back(new DumbZombie(k * SPRITE_WIDTH, l * SPRITE_HEIGHT, this));
                else if(sign == Level::citizen)
                {
                    m_nCit++;
                    m_actor.push_back(new Citizen(k * SPRITE_WIDTH, l * SPRITE_HEIGHT, this));
                    m_actor.back()->cure(); // Keep Citizen safe on initialization
                }
                else
                    continue; // Keep looping
            }
    }
    return GWSTATUS_CONTINUE_GAME; // Start it
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    StudentWorld::setText(); // Update the header
    
    vector<Actor*>::iterator i;
    for (i = m_actor.begin(); i != m_actor.end(); i++) // Move characters
    {
        (*i)->doSomething(); // Allow all characters to do something
    }
    int j = 0;
    for (i = m_actor.begin(); i != m_actor.end();  j++) // Get rid of characters
    {
        if(!(*i)->getStatus()) // If dead
        {
            if((*i)->interacting() == IS_MAIN) // Main is dead
            {
                decLives(); // Lose a life
                playSound(SOUND_PLAYER_DIE); // Cue sound
                (*i)->cure(); // Ensure charcter does not Zombify
                return GWSTATUS_PLAYER_DIED; // Player death
            }
            else if((*i)->interacting() == PICK_UP_OBJECT) // Pick up object destroyed
            {
                if((*i)->pickedUp())
                {
                    playSound(SOUND_GOT_GOODIE); // Cue Sound
                    increaseScore(50); // Increase score
                }
                delete *i; // Delete it
                m_actor.erase(i); // Erase allocated memory space
            }
            else if((*i)->interacting() == IS_SECONDARY_C) // Citizen
            {
                m_nCit--; // Less citizens to save
                if((*i)->turned()) // Infected citizen
                {
                    int w = randInt(1, 10); // Choose
                    playSound(SOUND_ZOMBIE_BORN); // Cue sound
                    if(w <= 3) // 30 percent
                        m_actor.push_back(new SmartZombie((*i)->getX(), (*i)->getY(), this)); // Smart Zombie is born
                    else
                        m_actor.push_back(new DumbZombie((*i)->getX(), (*i)->getY(), this)); // Any other time, dumb zombie born
                    increaseScore(-1000); // Decrease score
                }
                else if((*i)->exited())
                {
                    playSound(SOUND_CITIZEN_SAVED); // Cue sound
                    increaseScore(500); // Increase score
                }
                else
                {
                    playSound(SOUND_CITIZEN_DIE); // Cue sound
                    increaseScore(-1000); // Increase score
                }
                delete *i; // Delete it
                m_actor.erase(i); // Erase allocated memory space
            }
            else if((*i)->interacting() == IS_SECONDARY_Z) // Zombie died
            {
                playSound(SOUND_ZOMBIE_DIE); // Cue sound
                increaseScore(1000); // Increase score
                if(!(*i)->canDrop())
                    increaseScore(1000); // Smart Zombies are worth 2000
                if(randInt(1, 10) == 1 && (*i)->canDrop())
                {
                    int j = 0;
                    while(j <= 4)
                    {
                        int k = randInt(1, 4);
                        j++;
                        if(j == 4)
                            k = 0;
                        if(k == 1)
                        {
                            if(canMove(INVALID, (*i)->getX(), (*i)->getY() + SPRITE_HEIGHT, INVALID, INVALID) == MOVE_TO_NEW_SPACE)
                            {
                                m_actor.push_back(new Vaccine((*i)->getX(), (*i)->getY() + SPRITE_HEIGHT, this));
                                break;
                            }
                        }
                        else if(k == 2)
                        {
                            if(canMove(INVALID, (*i)->getX(), (*i)->getY() - SPRITE_HEIGHT, INVALID, INVALID) == MOVE_TO_NEW_SPACE)
                            {
                                m_actor.push_back(new Vaccine((*i)->getX(), (*i)->getY() - SPRITE_HEIGHT, this));
                                break;
                            }
                        }
                        else if(k == 3)
                        {
                            if(canMove(INVALID, (*i)->getX() - SPRITE_WIDTH, (*i)->getY(), INVALID, INVALID) == MOVE_TO_NEW_SPACE)
                            {
                                m_actor.push_back(new Vaccine((*i)->getX() - SPRITE_WIDTH, (*i)->getY(), this));
                                break;
                            }
                        }
                        else if(k == 4)
                        {
                            if(canMove(INVALID, (*i)->getX() + SPRITE_WIDTH, (*i)->getY(), INVALID, INVALID) == MOVE_TO_NEW_SPACE)
                            {
                                m_actor.push_back(new Vaccine((*i)->getX() + SPRITE_WIDTH, (*i)->getY(), this));
                                break;
                            }
                        }
                        else
                        {
                            m_actor.push_back(new Vaccine((*i)->getX(), (*i)->getY(), this));
                            break;
                        }
                    }
                }
                delete *i; // Delete it
                m_actor.erase(i); // Erase allocated memory space
            }
            else if((*i)->interacting() == KILL_CHARACTER ||
                    (*i)->interacting() == INFECTS ||
                    (*i)->interacting() == TAKE_DAMAGE) // If mine, fire, or vomit is dead
            {
                delete *i;
                m_actor.erase(i);
            }
            else if((*i)->interacting() == VOLATILE_OBJ) // Mines laid
            {
                double x = (*i)->getX();
                double y = (*i)->getY();
                delete *i; // Delete object
                m_actor.erase(i); // Erase allocated memory space
                goBoom(x, y); // Blow up
                return GWSTATUS_CONTINUE_GAME; // Allow flames to build
            }
        }
        else
            i++; // Increment because no delete
    }
    
    if(m_zVom)
    {
        m_zVom = false;
        playSound(SOUND_ZOMBIE_VOMIT);
    }
    if(m_inf)
    {
        m_inf = false;
        playSound(SOUND_CITIZEN_INFECTED);
    }
    
    if(m_mainChar->isInfected()) // If penelope is infected
        m_mainChar->incInfection(); // Increase infection count
    
    if(goToNewLevel() && finished()) // If all citizens saved and penelope is on the exit
    {
        playSound(SOUND_LEVEL_FINISHED);
        return GWSTATUS_FINISHED_LEVEL; // End the level
    }
    else
        jobNotDone(); // Keep going
    
    if(m_mainChar->infectionCount() >= 500) // Infection over 500
    {
        m_mainChar->objDeath(); // Set penelope to dead
        decLives(); // Lose a life
        return GWSTATUS_PLAYER_DIED; // Penelope infected fully
    }
    m_tick++; // Increment ticks passed
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() // Delete
{
    vector<Actor*>::iterator i;
    for (i = m_actor.begin(); i != m_actor.end();) // Repeatedly
    {
        delete *i; // Delete the actor
        i = m_actor.erase(i); // Erase its position
    }
    m_exit.clear(); // Clear exits
    m_pit.clear(); // Clear pits
}

int StudentWorld::canMove(int ignoreValue, double x, double y, double currentX, double currentY) // Determine overlap
{
    for(int i = 0; i < m_actor.size(); i++)
    {
        double xr, yu, xl, yd;
        double xrr, yuu, xll, ydd;
        xl = x;
        yd = y;
        xr = x + SPRITE_WIDTH - 1;
        yu = y + SPRITE_HEIGHT - 1;
        yuu = m_actor[i]->getY() + SPRITE_HEIGHT - 1;
        xrr = m_actor[i]->getX() + SPRITE_WIDTH - 1;
        ydd = m_actor[i]->getY();
        xll = m_actor[i]->getX();
        if(m_actor[i]->interacting() == ignoreValue &&
           xll == currentX && ydd == currentY)
            continue;
        if(((xl <= xrr && xl >= xll) && (yd <= yuu && yd >= ydd)) ||
           ((xr <= xrr && xr >= xll) && (yu <= yuu && yu >= ydd)) ||
           ((xr <= xrr && xr >= xll) && (yd <= yuu && yd >= ydd)) ||
           ((xl <= xrr && xl >= xll) && (yu <= yuu && yu >= ydd))) // If there is overlap
        {
            switch(m_actor[i]->interacting()) // Determine its interaction status
            { /*Returns its value*/
                case IS_SECONDARY_Z:
                    return IS_SECONDARY_Z;
                case IS_SECONDARY_C:
                    return IS_SECONDARY_C;
                case IS_MAIN:
                    return IS_MAIN;
                case DO_NOTHING:
                    return DO_NOTHING;
                case GO_TO_NEW_LEVEL:
                    return GO_TO_NEW_LEVEL;
                default: // Break for anything not in the switch case
                    break;
            }
        }
    }
    return MOVE_TO_NEW_SPACE; // Allow character to move there
}

double StudentWorld::distance(int x1, int y1, int x2, int y2) const // Determine distance
{
    return pow((pow(x2 - x1, 2) + pow(y2 - y1, 2)), .5); // Euclidian determine distance
}

void StudentWorld::fireFlame() // Firing flames
{
    Direction d = m_mainChar->getDirection(); // Get Penelope's direction
    switch(d) // Switch on d
    {
        case Actor::up: // If facing up
        {
            for(int i = 1; i <= 3; i++) // Repeatedly
            {
                if(canMove(INVALID, m_mainChar->getX(), m_mainChar->getY() + (i * SPRITE_HEIGHT), INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) == DO_NOTHING ||
                   canMove(INVALID, m_mainChar->getX(), m_mainChar->getY() + (i * SPRITE_HEIGHT), INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) == GO_TO_NEW_LEVEL) // If it encounters a wall or exit
                    break; // Break
                m_actor.push_back(new Flame(m_mainChar->getX(), m_mainChar->getY() + (i * SPRITE_HEIGHT), Actor::right, this)); // Make a new flame
            }
            break; // Break
        }
        case Actor::down: // If facing down
        {
            for(int i = 1; i <= 3; i++) // Repeatedly
            {
                if(canMove(INVALID, m_mainChar->getX(), m_mainChar->getY() - (i * SPRITE_HEIGHT), INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) == DO_NOTHING ||
                   canMove(INVALID, m_mainChar->getX(), m_mainChar->getY() - (i * SPRITE_HEIGHT), INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) == GO_TO_NEW_LEVEL) // If it encounters a wall or exit
                    break; // Break
                m_actor.push_back(new Flame(m_mainChar->getX(), m_mainChar->getY() - (i * SPRITE_HEIGHT), Actor::right, this)); // Make a new flame
            }
            break; // Break
        }
        case Actor::left: // If facing left
        {
            for(int i = 1; i <= 3; i++) // Repeatedly
            {
                if(canMove(INVALID, m_mainChar->getX() - (i * SPRITE_WIDTH), m_mainChar->getY(), INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) == DO_NOTHING ||
                   canMove(INVALID, m_mainChar->getX() - (i * SPRITE_WIDTH), m_mainChar->getY(), INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) == GO_TO_NEW_LEVEL) // If it encounters a wall or exit
                    break; // Break
                m_actor.push_back(new Flame(m_mainChar->getX() - (i * SPRITE_WIDTH), m_mainChar->getY(), Actor::right, this)); // Make a new Flame
            }
            break; // Break
        }
        case Actor::right: // If facing right
        {
            for(int i = 1; i <= 3; i++) // Repeatedly
            {
                if(canMove(INVALID, m_mainChar->getX() + (i * SPRITE_WIDTH), m_mainChar->getY(), INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) == DO_NOTHING ||
                   canMove(INVALID, m_mainChar->getX() + (i * SPRITE_WIDTH), m_mainChar->getY(), INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) == GO_TO_NEW_LEVEL) // If it encounters a wall or exit
                    break; // Break
                m_actor.push_back(new Flame(m_mainChar->getX() + (i * SPRITE_WIDTH), m_mainChar->getY(), Actor::right, this)); // Make a new flame
            }
            break; // Break
        }
        default: // Default case
            break; // Don't do anything
    }
}

void StudentWorld::vomit(double x, double y, Direction d) // Zombie throws or vomits in a certain direction
{
    switch(d) // Switiching on the direction
    {
        case Actor::right: // Facing right
            if(canMove(INVALID, x + (1 * SPRITE_WIDTH), y, INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) != DO_NOTHING &&
               canMove(INVALID, x + (1 * SPRITE_WIDTH), y, INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) != GO_TO_NEW_LEVEL) //If nothing blocks
                m_actor.push_back(new Vomit(x + (1 * SPRITE_WIDTH), y, this)); // Vomit to the right
            break; // Break
        case Actor::left: // Facing left
            if(canMove(INVALID, x - (1 * SPRITE_WIDTH), y, INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) != DO_NOTHING &&
               canMove(INVALID, x - (1 * SPRITE_WIDTH), y, INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) != GO_TO_NEW_LEVEL)
                //If nothing blocks
                m_actor.push_back(new Vomit(x - (1 * SPRITE_WIDTH), y, this)); // Vomit to the left
            break; // Break
        case Actor::down: // Facing down
            if(canMove(INVALID, x, y - (1 * SPRITE_HEIGHT), INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) != DO_NOTHING &&
               canMove(INVALID, x, y - (1 * SPRITE_HEIGHT), INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) != GO_TO_NEW_LEVEL)
                m_actor.push_back(new Vomit(x, y - (1 * SPRITE_HEIGHT), this)); // Vomit down
            break; // Break
        case Actor::up: // Facing up
            if(canMove(INVALID, x, y + (1 * SPRITE_HEIGHT), INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) != DO_NOTHING &&
               canMove(INVALID, x, y + (1 * SPRITE_HEIGHT), INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) != GO_TO_NEW_LEVEL)
                m_actor.push_back(new Vomit(x, y + (1 * SPRITE_HEIGHT), this)); // Vomit up
            break; // Break
        default: // Do nothing
            break; // Break
    }
    m_zVom = true;
}

void StudentWorld::plantMine(double x, double y) // Plant a mine at position
{
    if(canMove(VOLATILE_OBJ, x, y, x, y) != GO_TO_NEW_LEVEL)
    {
        m_actor.push_back(new Landmine(x, y, this)); // Create the landmine
        m_mainChar->decMineAmmo();
    }
}

void StudentWorld::goBoom(double x, double y) // Blow up the landmine
{
    playSound(SOUND_LANDMINE_EXPLODE); // Play soundtrack
    for(int i = -1; i <= 1; i++) //  Repeatedly
        for(int j = -1; j <= 1; j++) // Repeatedly
        {
            if(canMove(INVALID, x + (i * SPRITE_WIDTH), y + (j * SPRITE_HEIGHT), INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) == DO_NOTHING ||
               canMove(INVALID, x + (i * SPRITE_WIDTH), y + (j * SPRITE_HEIGHT), INVALID * SPRITE_WIDTH, INVALID * SPRITE_HEIGHT) == GO_TO_NEW_LEVEL) //If something blocks
                continue; // Skip
            if(i == 0 && j == 0) //  Where mine was supposed to be
                m_actor.push_back(new Pit(x + (i * SPRITE_WIDTH), y + (j * SPRITE_HEIGHT), this)); // Make a pit
            m_actor.push_back(new Flame(x + (i * SPRITE_WIDTH), y + (j * SPRITE_HEIGHT), Actor::up, this)); // Create fire
        }
}

bool StudentWorld::goToNewLevel()
{
    if(m_nCit == 0) // If no more citizens
        return true; // Return true
    return false; // Return false
}

void StudentWorld::setText() // Sets top text
{
    int s = getScore();
    int l = getLevel();
    int li = getLives();
    int v = m_mainChar->getVaxCount();
    int f = m_mainChar->getFlameCount();
    int m = m_mainChar->getMineCount();
    int i = m_mainChar->infectionCount();
    ostringstream oss;
    oss.setf(ios::fixed);
    
    if(s > 0)
        oss.fill('0');
    oss << "Score: " << setw(5) << s << "  ";
    oss << "Level: " << l << "  ";
    oss << "Lives: " << li << "  ";
    oss << "Vaccines: " << v << "  ";
    oss << "Flames: " << f << "  ";
    oss << "Mines: " << m << "  ";
    oss << "Infected: " << i << "  ";
    
    string text = oss.str();
    GameWorld::setGameStatText(text);
}
