#include "Arena.h"
#include "History.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
using namespace std;

int main()
{
    Arena a(4, 4);
    a.addPlayer(2, 4);
    a.addZombie(3, 2);
    a.addZombie(2, 3);
    a.addZombie(1, 4);
    a.player()->moveOrAttack(LEFT);
    a.player()->moveOrAttack(UP);
    a.player()->moveOrAttack(LEFT);
    a.player()->moveOrAttack(LEFT);
    a.player()->moveOrAttack(DOWN);
    a.player()->moveOrAttack(DOWN);
    a.player()->moveOrAttack(LEFT);
    a.player()->moveOrAttack(UP);
    a.player()->moveOrAttack(UP);
    a.player()->moveOrAttack(UP);
    a.history().display();
    cout << "====" << endl;
    }
