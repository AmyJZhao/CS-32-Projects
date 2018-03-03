#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
class Actor;
class NachenBlaster;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void removeDeadGameObjects();
    //decrease the number of stars
    void decStars();
    //decrease the number of aliens
    void decAliens();
    //increase the number of aliens destroyed
    void killedAlien();
    //returns true if the nachenblaster is dead and decreases lives
    bool NachenBlasterDied();
    //returns true if nachenblaster killed required number of aliens
    bool NachenBlasterCompletedLevel();
    //adds an actor to StudentWorld's vector
    void addActor(Actor* m_actor);
    //returns a pointer to NachenBlaster
    NachenBlaster* getPlayer() const;
    //returns true if the two Actors collide
    bool collide(const Actor* a1, const Actor* a2);
    // If there's at least one alien that's collided with a, return
    // a pointer to one of them; otherwise, return a null pointer.
    Actor* getOneCollidingAlien(Actor* a);
    //takes an Actor as a parameter and finds all the projectiles that collide with it
    void collideWithProjectiles(Actor* a);
    //returns true if the actor collides with NachenBlaster
    bool collideWithPlayer(Actor* a);
    //returns true if player is within 4 pixels of the actor
    bool playerInLineOfFire(const Actor* a) const;
    //updates the game text
    void updateGameText();
    ~StudentWorld();
private:
    NachenBlaster* player;
    std::vector<Actor*> actors;
    int m_numStars;
    int m_currentAliens;
    int m_aliensDestroyed;
    std::ostringstream oss;
};

#endif // STUDENTWORLD_H_
