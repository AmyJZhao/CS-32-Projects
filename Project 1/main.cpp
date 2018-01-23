

#include "Game.h"

int main()
{
    // Create a game
    // Use this instead to create a mini-game:
    //Game g(3, 4, 2);
    Game g(7, 8, 25);
    //Game g(4, 4, 4);
    
    // Play the game
    g.play();
}



/*
#include "Game.h"
#include "Game.h"
#include "City.h"
#include "City.h"
#include "History.h"
#include "History.h"
#include "Player.h"
#include "Player.h"
#include "Flatulan.h"
#include "Flatulan.h"
#include "globals.h"
#include "globals.h"
int main()
{}
*/
/*
#include "History.h"
int main()
{
    History h(2, 2);
    h.record(1, 1);
    h.display();
}
*/
/*
#include "Flatulan.h"
int main()
{
    Flatulan f(nullptr, 1, 1);
}
*/
/*
#include "Player.h"
int main()
{
    Player p(nullptr, 1, 1);
}
*/
/*
#include "City.h"
int main()
{
    City c(10, 18);
    c.addPlayer(2, 2);
}
*/
/*
#include "globals.h"
#include "Player.h"
#include "City.h"
int main()
{
    City c(10, 20);
    Player p(&c, 2, 3);
}
*/
/*
#include "City.h"
#include "Player.h"
int main()
{
    City c(10, 20);
    Player p(&c, 2, 3);
}
 */
/*
#include "City.h"
#include "History.h"
#include <iostream>
using namespace std;

int main()
{
    City c(2, 3);
    c.addPlayer(1, 2);
    do
    {
        c.addFlatulan(2, 1);
        c.preachToFlatulansAroundPlayer();
    } while (c.nFlatulansAt(2, 1) == 0);
    c.history().display();
    cout << "===" << endl;
}
*/
/*
#include "Flatulan.h"
#include "City.h"
int main()
{
    City a(10, 20);
    Player p(&a, 2, 3);
    Flatulan f(&a, 1, 1);
}
*/
/*
#include "Player.h"
#include "City.h"
int main()
{
    City a(10, 20);
    Player p(&a, 2, 3);
    Flatulan f(&a, 1, 1);
}
*/
/*
#include "globals.h"
#include "Flatulan.h"
#include "Player.h"
int main()
{
    City a(10, 10);
}
*/
/*
 #include "History.h"
 int main()
 {
 History h;
 }
 */



