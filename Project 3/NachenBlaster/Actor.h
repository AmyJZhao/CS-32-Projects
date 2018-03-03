#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
const int HIT_BY_SHIP = 200;
const int HIT_BY_PROJECTILE = 201;
const int STAR = 298;
const int EXPLOSION = 299;
const int NACHENBLASTER = 300;
const int REG_ALIEN = 301;
const int SNAGGLEGON = 302;
const int REG_PROJECTILE = 303;
const int TORPEDO = 304;
const int EL_GOODIE = 305;
const int R_GOODIE = 306;
const int F_GOODIE = 307;
const int NACHENBLASTER_START_X = 0;
const int NACHENBLASTER_START_Y = 128;
const int NACHENBLASTER_INITIAL_HIT_POINTS = 50;
const int NACHENBLASTER_INITIAL_CABBAGES = 30;
const int NACHENBLASTER_DIRECTION = 0;
const double NACHENBLASTER_SIZE = 1;
const int NACHENBLASTER_DEPTH = 0;
const int STAR_DIRECTION = 0;
const int STAR_DEPTH = 3;
const int EXPLOSION_DIRECTION = 0;
const double EXPLOSION_INIT_SIZE = 1;
const int EXPLOSION_DEPTH = 0;
const int ALIEN_DIRECTION = 0;
const double ALIEN_SIZE = 1.5;
const int ALIEN_DEPTH = 1;
const int SMALLGON_SMOREGON_FP = 0;
const double SMALLGON_SMOREGON_DAMAGE_AMT = 5;
const double SMALLGON_SMOREGON_SPEED = 2;
const int SMALLGON_SMOREGON_SCORE_VALUE = 250;
const double SNAGGLEGON_DAMAGE_AMT = 15;
const double SNAGGLEGON_SPEED = 1.75;
const int SNAGGLEGON_SCORE_VALUE = 1000;
const double PROJECTILE_SIZE = 0.5;
const int PROJECTILE_DEPTH = 1;
const double CABBAGE_TURNIP_DAMAGE_AMT = 2;
const int CABBAGE_TURNIP_DIR = 0;
const double TORPEDO_DAMAGE_AMT = 8;
const int PLAYER_TORPEDO_DIR = 0;
const int ALIEN_TORPEDO_DIR = 180;
const double UP = 50;
const double DOWN = 51;
const double LEFT = 52;
const double RIGHT = 53;
const double TURNIP_SPEED = 6;
const double TORPEDO_SPEED = 8;
const int GOODIE_DIRECTION = 0;
const double GOODIE_SIZE = 0.5;
const int GOODIE_DEPTH = 1;
const int FRIENDLY = 101;
const int ENEMY = 100;
const int NA = -1;
class StudentWorld; 
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
    public:
        Actor(StudentWorld* world, int imageID, double startX, double startY, int dir, double size, int depth);
        //pure virtual functions
        virtual void doSomething() = 0;
        //returns true if actor can collide with other actors
        virtual bool isCollidable() = 0;
        //identifies actor as a friendly or enemy
        virtual int isFriendlyOrEnemy() = 0;
        //identifies whether actor is a ship
        virtual bool isDamageable() = 0;
        //identifies the particular type of actor
        virtual int identifier() = 0;
        //allows actor to call another actor's sufferDamage() through StudentWorld
        virtual void sufferDamage(double amt, int cause) = 0;
        //returns state (dead or alive)
        virtual bool isAlive();
        //eliminate repetitive code in doSomething()
        bool initDoSomething();
        //sets state
        void setState(bool alive);
        //returns a pointer to StudentWorld
        StudentWorld* getWorld();
        //checks if the actor has flown off screen
        bool flownOffScreen(int x, int y);
        virtual void moveTo(double x, double y);
    
    private:
        StudentWorld* m_world;
        bool m_alive;
};
class DamageableObject : public Actor
{
    public:
        DamageableObject(StudentWorld* w, int imageID, double startX, double startY, int startDir, double size, int depth, double hitPoints);
        virtual bool isAlive();
        // returns amount of hit points
        virtual void sufferDamage(double amt, int cause);
        //specifies what the damageable actor is
        virtual int identifier();
        //returns true because damageable actors are collidable
        virtual bool isCollidable();
        //returns true
        virtual bool isDamageable();
        //returns NA
        virtual int isFriendlyOrEnemy();
        double hitPoints() const;
        // Increase this actor's hit points by amt.
        void increaseHitPoints(double amt);
        //Decrease this actor's hit points by amt
        void decreaseHitPoints(double amt);
        // This actor suffers an amount of damage caused by being hit by either a ship or a projectile (see constants above).
    private:
        double m_hitpoints;
};

class Star : public Actor
{
public:
    Star(StudentWorld* world, double startX, double startY);
    virtual void doSomething();
    //returns false because a star can't collide with anything
    virtual bool isCollidable();
    //returns false because a star is not damageable
    virtual bool isDamageable();
    //returns N/A because a star is not on any side
    virtual int isFriendlyOrEnemy();
    virtual int identifier();
    //useless method, star doesn't suffer any damage
    virtual void sufferDamage(double amt, int cause);
};
class Explosion : public Actor
{
public:
    Explosion(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    //returns false because an explosion can't collide with anything
    virtual bool isCollidable();
    //returns false because an explosion isn't damageable
    virtual bool isDamageable();
    //returns N/A because an explosion is neutral
    virtual int isFriendlyOrEnemy();
    virtual int identifier();
    //useless method, star doesn't suffer any damage
    virtual void sufferDamage(double amt, int cause);
private:
    int m_ticks;
};
class NachenBlaster : public DamageableObject
{
    public:
        NachenBlaster(StudentWorld* world);
        //virtual methods
        virtual void doSomething();
        virtual void sufferDamage(double amt, int cause);
        // return friendly
        virtual int isFriendlyOrEnemy();
        virtual int identifier();
        //if Nachenblaster moves, check for collisions
        void checkCol();
        // Incease the number of torpedoes the player has by amt.
        void increaseTorpedoes(int amt);
        // Get the player's health percentage.
        int healthPct() const;
        // Get the player's cabbage energy percentage.
        int cabbagePct() const;
        // Get the number of torpedoes the player has.
        double numTorpedoes() const;
        //  Get the number of cabbages the player has
        double numCabbages() const;
    private:
        double m_cabbages;
        double m_torpedoes;
};
class Alien : public DamageableObject
{
public:
    Alien(StudentWorld* w, int imageID, double startX, double startY, double hitPoints, double damageAmt, double deltaX,
          double deltaY, double speed, unsigned int scoreValue, int flightPlan);
    //Alien will assume Smallgon's and Smoregon's default values in regards to damaging player
    //virtual methods
    virtual void sufferDamage(double amt, int cause);
    //check for collisions with enemy projectiles and player
    virtual void beginCheckCollision();
    //fire a projectile
    virtual bool fireProjectile();
    //returns enemy
    virtual int isFriendlyOrEnemy();
    //derived classes will override this method to identify themselves
    virtual int identifier();
    // Damage the player and return
    virtual void damageCollidingPlayer(double amt = SMALLGON_SMOREGON_DAMAGE_AMT);
    // If this alien drops goodies, drop one with the appropriate probability.
    virtual void possiblyDropGoodie();
    //check if flight plan is 0 or if ship needs a new flight plan
    virtual void checkAndUpdateFlightPlan();
    //helper method to condense tasks in doSomething()
    //all aliens do initAlienDoSomething() in the beginning of doSomething()
    virtual bool initAlienDoSomething();
    //get flight plan
    int getFlightPlan();
    //set flight plan
    void setFlightPlan(int flight_plan);
    // Move the player by the current speed in the direction indicated
    // by the x and y deltas.
    void move();
    // Set the player's y direction.
    void setDeltaY(double dy);
    // Set the player's speed.
    void setSpeed(double speed);
    // returns true if player is in range of the alien
    bool inRange();
    //if an alien is killed, this method sets its state to dead,
    //decreases current number of aliens in StudentWorld, updates
    //number of aliens destroyed, then play sound and add explosion
    void isKilled();
private:
    double m_speed;
    double m_dx;
    double m_dy;
    double m_damageAmt;
    double m_scoreValue;
    int m_flightPlan;
};
class Smallgon : public Alien
{
public:
    Smallgon(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
};

class Smoregon : public Alien
{
public:
    Smoregon(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    //Smoregon drops either Repair Goodies or Torpedo Goodies
    virtual void possiblyDropGoodie();
    virtual void sufferDamage(double amt, int cause);
};

class Snagglegon : public Alien
{
public:
    Snagglegon(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    //Snagglegon overrides Alien
    //overloads checkAndUpdateFlightPlan() from Alien because Snagglegon
    //follows different steps for its flight plan
    virtual void checkAndUpdateFlightPlan();
    //Snagglegon only fires torpedoes
    virtual bool fireProjectile();
    //Snagglegon drops only Extra Life Goodies
    virtual void possiblyDropGoodie();
    //Snagglegons don't disappear when they fly off screen in the y-direction
    //hence Snagglegons needs a different initial doSomething() helper method
    virtual bool initAlienDoSomething();
    //does higher damage to player when collided with, so overrides beginCheckCollision()
    virtual void beginCheckCollision();
    //identifies as Snagglegon
    virtual int identifier();
    //increases player's score by a different value, so overrides Alien's sufferDamage() method
    virtual void sufferDamage(double amt, int cause);
};

class Projectile : public Actor
{
public:
    Projectile(StudentWorld* w, int imageID, double startX, double startY, double damageAmt, double deltaX, bool rotates, int imageDir);
    //virtual methods
    //returns true because projectiles can collide
    virtual bool isCollidable();
    //returns false because projectiles are not damageable
    virtual bool isDamageable();
    //returns NA so Projectile's derived classes can override
    virtual int isFriendlyOrEnemy();
    virtual int identifier();
    //useless method, projectiles don't suffer damage
    virtual void sufferDamage(double amt, int cause);
    void rotate(int change_dir);
    //checks if projectile has collided with an alien
    bool checkC(double amt);
private:
    double m_damageAmt;
    double m_dx;
    bool m_rotates;
};

class Cabbage : public Projectile
{
public:
    Cabbage(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    //returns friendly
    virtual int isFriendlyOrEnemy();
    
};

class Turnip : public Projectile
{
public:
    Turnip(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    //returns enemy
    virtual int isFriendlyOrEnemy();
};

class Torpedo : public Projectile
{
public:
    Torpedo(StudentWorld* w, double startX, double startY, double deltaX,
            int imageDir);
    virtual int isFriendlyOrEnemy();
    virtual int identifier();
};

class PlayerLaunchedTorpedo : public Torpedo
{
public:
    PlayerLaunchedTorpedo(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    //returns friendly
    virtual int isFriendlyOrEnemy();
};

class AlienLaunchedTorpedo : public Torpedo
{
public:
    AlienLaunchedTorpedo(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    //returns enemy
    virtual int isFriendlyOrEnemy();
};

class Goodie : public Actor
{
public:
    Goodie(StudentWorld* w, int imageID, double startX, double startY);
    //returns false because a goodie is not collidable
    virtual bool isCollidable();
    //returns false because a goodie is not damageable
    virtual bool isDamageable();
    //returns enemy
    virtual int isFriendlyOrEnemy();
    //returns NA so derived classes can override and identify themselves
    virtual int identifier();
    //useless method
    virtual void sufferDamage(double amt, int cause);
    bool checkCollide();
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual int identifier();
};

class RepairGoodie : public Goodie
{
public:
    RepairGoodie(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual int identifier();
};

class TorpedoGoodie : public Goodie
{
public:
    TorpedoGoodie(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual int identifier();
};

#endif // ACTOR_H_
