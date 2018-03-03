#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
    
}
void StudentWorld::addActor(Actor* m_actor)
{
    actors.push_back(m_actor);
}
int StudentWorld::init()
{
    //initialize number of stars, number of aliens destroyed, and number of current aliens on screen to 0
    m_numStars = 0;
    m_aliensDestroyed = 0;
    m_currentAliens = 0;
    //create NachenBlaster pointer to hold player
    player = new NachenBlaster(this);
    //push player to vector
    actors.push_back(player);
    //add stars to screen
    for(int i = 0; i < 30; i++)
    {
        m_numStars++;
        Star* star = new Star(this, randInt(0, VIEW_WIDTH), randInt(0, VIEW_HEIGHT - 1));
        actors.push_back(star);
    }
    //set game stat text
    oss << "Lives: 3  Health: 100%  Score: 0  Level: 1  Cabbages: 100%  Torpedoes: 0";
    setGameStatText(oss.str());
    return GWSTATUS_CONTINUE_GAME;
}
//returns true if the nachenblaster is dead, and decrease lives if that is true
bool StudentWorld::NachenBlasterDied()
{
    if(player->isAlive() == false)
    {
        decLives();
        return true;
    }
    return false;
}
//decrease the number of aliens currently on screen
void StudentWorld::decAliens()
{
    m_currentAliens--;
}
//increment number of aliens destroyed
void StudentWorld::killedAlien()
{
    m_aliensDestroyed++;
}
int StudentWorld::move()
{
    //add stars
    if(m_numStars < 30)
    {
        int n = randInt(1, 15);
        if(n == 1)
        {
            Star* star = new Star(this, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1));
            actors.push_back(star);
        }
    }
    //add alien ships
    int numAliensDestroyed = m_aliensDestroyed;
    int requiredAliensDestroyed = 6 + 4 * getLevel();
    int remainingAliensDestroyed = requiredAliensDestroyed - numAliensDestroyed;
    int maxAliensOnScreen = 4 + 0.5 * getLevel();
    if(m_currentAliens < min(maxAliensOnScreen, remainingAliensDestroyed))
    {
        m_currentAliens++;
        int S1 = 60;
        int S2 = 20 + getLevel() * 5;
        int S3 = 5 + getLevel() * 10;
        int S = S1 + S2 + S3;
        int chance = randInt(1, S);
        if(chance <= S1)
        {
            Alien* small = new Smallgon(this, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1));
            actors.push_back(small);
        }
        else if(chance > S1 && chance <= S1 + S2)
        {
            Alien* smore = new Smoregon(this, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1));
            actors.push_back(smore);
        }
        else if(chance > S1 + S2 && chance <= S)
        {
            Alien* snaggle = new Snagglegon(this, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1));
            actors.push_back(snaggle);
        }
    }
    //make all actors do something
    for(int i = 0; i != actors.size(); i++)
    {
        if(actors[i]->isAlive())
        {
            actors[i]->doSomething();
            if(NachenBlasterDied())
                return GWSTATUS_PLAYER_DIED;
            if(NachenBlasterCompletedLevel())
            {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }
    //remove dead actors
    removeDeadGameObjects();
    //update game text
    updateGameText();
    return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::updateGameText()
{
    oss.str("");
    oss.clear();
    oss << "Lives: " << getLives() << "  Health: " << getPlayer()->healthPct() << "%  Score: " << getScore() << "  Level: " << getLevel() << "  Cabbages: " << getPlayer()->cabbagePct() << "%  Torpedoes: " << getPlayer()->numTorpedoes();
    setGameStatText(oss.str());
}
//returns true if NachenBlaster completes level, and set number of aliens destroyed back to 0
bool StudentWorld::NachenBlasterCompletedLevel()
{
    if(m_aliensDestroyed == 6 + 4 * getLevel())
    {
        m_aliensDestroyed = 0;
        return true;
    }
    return false;
}
//delete vector's contents
void StudentWorld::cleanUp()
{
    for(vector<Actor*>::iterator it = actors.begin(); it != actors.end(); it++)
    {
        delete *it;
    }
    actors.clear();
}
//remove the dead actors from the vector
void StudentWorld::removeDeadGameObjects()
{
    
    for(vector<Actor*>::iterator it = actors.begin() + 1; it != actors.end(); it++)
    {
        Actor* a = *it;
        if((*it)->isAlive() == false)
        {
            actors.erase(it);
            it--;
            delete a;
        }
    }
}
//decrement the number of stars
void StudentWorld::decStars()
{
    m_numStars--;
}
//return a pointer to the player
NachenBlaster* StudentWorld::getPlayer() const
{
    return player; 
}
//return true if both actors collide with each other
bool StudentWorld::collide(const Actor* a1, const Actor* a2)
{
    double euclidian_dist = pow(pow(a1->getX() - a2->getX(), 2) + pow(a1->getY() - a2->getY(), 2), 0.5);
    if(euclidian_dist < 0.75 * (a1->getRadius() + a2->getRadius()))
        return true;
    return false;
}
//return pointer to an alien that collides with Actor a
Actor* StudentWorld::getOneCollidingAlien(Actor* a)
{
    Actor* colliding_Actr = nullptr;
    for(int i = 1; i != actors.size(); i++)
    {
        if(actors[i]->isCollidable())
        {
            if(actors[i]->isFriendlyOrEnemy() == ENEMY && actors[i]->isDamageable())
            {
                if(collide(actors[i], a))
                    colliding_Actr = actors[i];
            }
        }
    }
    return colliding_Actr;
}
//returns true if Actor a collides with player
bool StudentWorld::collideWithPlayer(Actor* a)
{
    return collide(player, a);
}
//used many helper methods
//if actor is nachenblaster or a "friendly", can only collide with "enemy" projectiles, and vice versa
void StudentWorld::collideWithProjectiles(Actor *a)
{
    if(a->isDamageable() == false)
        return;
    for(int i = 1; i < actors.size(); i++)
    {
        if(actors[i]->isDamageable() == false || actors[i]->isCollidable() == false)
            continue;
        if(collide(a, actors[i]))
        {
            //find enemy projectiles that collide with player
            //or friendly projectiles (cabbages, torpedoes) that collide with alien
            if((a->isFriendlyOrEnemy() == FRIENDLY && actors[i]->isFriendlyOrEnemy() == ENEMY) ||
               (a->isFriendlyOrEnemy() == ENEMY && actors[i]->isFriendlyOrEnemy() == FRIENDLY))
            {
                if(actors[i]->identifier() == TORPEDO)
                {
                    playSound(SOUND_BLAST);
                    a->sufferDamage(TORPEDO_DAMAGE_AMT, HIT_BY_PROJECTILE);
                    actors[i]->setState(false);
                }
                else if(actors[i]->identifier() == REG_PROJECTILE)
                {
                    playSound(SOUND_BLAST);
                    a->sufferDamage(CABBAGE_TURNIP_DAMAGE_AMT, HIT_BY_PROJECTILE);
                    actors[i]->setState(false);
                }
            }
        }
    }
}
//returns true if player is within 4 pixels of Actor a
bool StudentWorld::playerInLineOfFire(const Actor* a) const
{
    if(getPlayer()->getX() < a->getX() && abs(a->getY() - getPlayer()->getY()) <= 4)
        return true;
    return false;
}

StudentWorld::~StudentWorld()
{
    cleanUp(); 
}
