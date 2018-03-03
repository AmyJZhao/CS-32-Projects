#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(StudentWorld *world, int imageID, double startX, double startY, int dir, double size, int depth)
: GraphObject(imageID, startX, startY, dir, size, depth)
{
    m_world = world;
    m_alive = true;
}
void Actor::moveTo(double x, double y)
{
    GraphObject::moveTo(x, y);
}
bool Actor::isAlive()
{
    return m_alive;
}
void Actor::setState(bool alive)
{
    m_alive = alive;
}
StudentWorld* Actor::getWorld()
{
    return m_world; 
}
//checks if actor has flown off screen
bool Actor::flownOffScreen(int x, int y)
{
    if(x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT)
        return true;
    return false;
}

//returns false if actor is not alive or has flown off the screen
//Most actors go through these steps in their doSomething() function.
bool Actor::initDoSomething()
{
    if(!isAlive())
        return false;
    if(flownOffScreen(getX(), getY()))
    {
        setState(false);
        return false;
    }
    return true;
}
DamageableObject::DamageableObject(StudentWorld* w, int imageID, double startX, double startY, int startDir, double size, int depth, double hitPoints)
: Actor(w, imageID, startX, startY, startDir, size, depth)
{
    m_hitpoints = hitPoints;
}
bool DamageableObject::isAlive()
{
    if(m_hitpoints <= 0)
        setState(false);
    return Actor::isAlive();
}
double DamageableObject::hitPoints() const
{
    return m_hitpoints;
}
void DamageableObject::increaseHitPoints(double amt)
{
    m_hitpoints += amt;
}
void DamageableObject::decreaseHitPoints(double amt)
{
    m_hitpoints -= amt;
}
bool DamageableObject::isCollidable()
{
    return true;
}
int DamageableObject::isFriendlyOrEnemy()
{
    return NA;
}
bool DamageableObject::isDamageable()
{
    return true;
}
void DamageableObject::sufferDamage(double amt, int cause)
{
    decreaseHitPoints(amt);
    if(cause == HIT_BY_PROJECTILE)
    {
        getWorld()->playSound(SOUND_BLAST);
    }
    else if(cause == HIT_BY_SHIP)
        getWorld()->playSound(SOUND_DEATH);
}
int DamageableObject::identifier()
{
    return NA;
}

Star::Star(StudentWorld *world, double startX, double startY)
: Actor(world, IID_STAR, startX, startY, STAR_DIRECTION, randInt(5, 50) / 100.0, STAR_DEPTH)
{
    
}
void Star::doSomething()
{
    moveTo(getX() - 1, getY());
    if(flownOffScreen(getX(), getY()))
    {
        setState(false);
        getWorld()->decStars(); 
    }
}
bool Star::isCollidable()
{
    return false;
}
bool Star::isDamageable()
{
    return false;
}
int Star::isFriendlyOrEnemy()
{
    return NA;
}

int Star::identifier()
{
    return STAR;
}
void Star::sufferDamage(double amt, int cause)
{
    return;
}
Explosion::Explosion(StudentWorld* w, double startX, double startY)
: Actor(w, IID_EXPLOSION, startX, startY, EXPLOSION_DIRECTION, EXPLOSION_INIT_SIZE, EXPLOSION_DEPTH)
{
    m_ticks = 0;
    //getWorld()->playSound(SOUND_DEATH);
}
void Explosion::doSomething()
{
    setSize(getSize() * 1.5);
    m_ticks++;
    if(m_ticks == 4)
    {
        setState(false);
        m_ticks = 0;
    }
}
bool Explosion::isCollidable()
{
    return false;
}
bool Explosion::isDamageable()
{
    return false;
}
int Explosion::isFriendlyOrEnemy()
{
    return NA;
}

int Explosion::identifier()
{
    return EXPLOSION;
}
void Explosion::sufferDamage(double amt, int cause)
{
    return;
}
NachenBlaster::NachenBlaster(StudentWorld *world)
: DamageableObject(world, IID_NACHENBLASTER, NACHENBLASTER_START_X, NACHENBLASTER_START_Y, NACHENBLASTER_DIRECTION, NACHENBLASTER_SIZE, NACHENBLASTER_DEPTH, NACHENBLASTER_INITIAL_HIT_POINTS)
{
    m_cabbages = NACHENBLASTER_INITIAL_CABBAGES;
    m_torpedoes = 0;
}
void NachenBlaster::sufferDamage(double amt, int cause)
{
    DamageableObject::sufferDamage(amt, cause);
}

int NachenBlaster::identifier()
{
    return NACHENBLASTER;
}
 
void NachenBlaster::doSomething()
{
    if(!isAlive())
        return;
    int ch;
    if (getWorld()->getKey(ch))
    {
        // user hit a key during this tick!
        switch (ch)
        {
            //after the player moves, check for a collision
            case KEY_PRESS_LEFT:
                if(!flownOffScreen(getX() - 6, getY()))
                {
                    moveTo(getX() - 6, getY());
                    checkCol();
                }
                break;
            case KEY_PRESS_RIGHT:
                if(!flownOffScreen(getX() + 6, getY()))
                {
                    moveTo(getX() + 6, getY());
                    checkCol();
                }
            break;
            case KEY_PRESS_UP:
                if(!flownOffScreen(getX(), getY() + 6))
                {
                    moveTo(getX(), getY() + 6);
                    checkCol();
                }
                break;
            case KEY_PRESS_DOWN:
                if(!flownOffScreen(getX(), getY() - 6))
                {
                    moveTo(getX(), getY() - 6);
                    checkCol();
                }
                break;
            case KEY_PRESS_SPACE:
                //fire a cabbage
                if(m_cabbages >= 5)
                {
                    m_cabbages -= 5;
                    getWorld()->addActor(new Cabbage(getWorld(), getX() + 12, getY()));
                    getWorld()->playSound(SOUND_PLAYER_SHOOT);
                }
                break;
            case KEY_PRESS_TAB:
                //fire a torpedo
                if(m_torpedoes > 0)
                {
                    m_torpedoes--;
                    getWorld()->addActor(new PlayerLaunchedTorpedo(getWorld(), getX() + 12, getY()));
                    getWorld()->playSound(SOUND_TORPEDO);
                }
                break;
        }
    }
    //update cabbage points
    if(m_cabbages < 30)
        m_cabbages++;
}
void NachenBlaster::checkCol()
{
    //check if collided with projectiles
    getWorld()->collideWithProjectiles(this);
    //check if collided with alien
    Actor* alien = getWorld()->getOneCollidingAlien(this);
    if(alien != nullptr)
    {
        if(alien->identifier() == REG_ALIEN)
        {
            decreaseHitPoints(SMALLGON_SMOREGON_DAMAGE_AMT);
            alien->sufferDamage(0, HIT_BY_SHIP);
            getWorld()->increaseScore(250);
            
        }
        else if(alien->identifier() == SNAGGLEGON)
        {
            decreaseHitPoints(SNAGGLEGON_DAMAGE_AMT);
            getWorld()->increaseScore(1000);
            alien->sufferDamage(0, HIT_BY_SHIP);
        }
    }
}
void NachenBlaster::increaseTorpedoes(int amt)
{
    m_torpedoes += amt;
}
// Get the player's health percentage.
int NachenBlaster::healthPct() const
{
    return static_cast<int>(100 * (hitPoints() / 50.0));
}
// Get the player's cabbage energy percentage.
int NachenBlaster::cabbagePct() const
{
    return static_cast<int>(100 * (m_cabbages / NACHENBLASTER_INITIAL_CABBAGES));
}
// Get the number of torpedoes the player has.
double NachenBlaster::numTorpedoes() const
{
    return m_torpedoes;
}
double NachenBlaster::numCabbages() const
{
    return m_cabbages;
}
int NachenBlaster::isFriendlyOrEnemy()
{
    return FRIENDLY;
}
Alien::Alien(StudentWorld* w, int imageID, double startX, double startY, double hitPoints, double damageAmt, double deltaX, double deltaY, double speed, unsigned int scoreValue, int flightPlan)
:DamageableObject(w, imageID, startX, startY, ALIEN_DIRECTION, ALIEN_SIZE, ALIEN_DEPTH, hitPoints)
{
    m_speed = speed;
    m_dx = deltaX;
    m_dy = deltaY;
    m_damageAmt = damageAmt;
    m_scoreValue = scoreValue;
}
int Alien::isFriendlyOrEnemy()
{
    return ENEMY;
}
void Alien::setDeltaY(double dy)
{
    m_dy = dy;
}

int Alien::identifier()
{
    return REG_ALIEN;
}

void Alien::setSpeed(double speed)
{
    m_speed = speed;
}
int Alien::getFlightPlan()
{
    return m_flightPlan;
}
void Alien::setFlightPlan(int flight_plan)
{
    m_flightPlan = flight_plan;
}
void Alien::checkAndUpdateFlightPlan()
{
    if(getY() >= VIEW_HEIGHT - 1)
    {
        setDeltaY(DOWN);
    }
    if(getY() <= 0)
    {
        setDeltaY(UP);
    }
    if(getFlightPlan() == 0)
    {
        int new_flight = randInt(1, 3);
        switch(new_flight)
        {
            case 1:
                setDeltaY(0);
                break;
            case 2:
                setDeltaY(UP);
                break;
            case 3:
                setDeltaY(DOWN);
                break;
        }
        setFlightPlan(randInt(1, 32));
    }
}
bool Alien::initAlienDoSomething()
{
    //if alien is not alive or has flown off the screen, decrease
    //number of aliens currently on screen
    if(!initDoSomething())
        getWorld()->decAliens();
    return initDoSomething();
}

void Alien::possiblyDropGoodie()
{
    return;
}
//consolidates process for an alien getting killed
void Alien::isKilled()
{
    getWorld()->decAliens();
    getWorld()->killedAlien();
    getWorld()->playSound(SOUND_DEATH);
    getWorld()->addActor(new Explosion(getWorld(), getX(), getY()));
    setState(false);
}
void Alien::sufferDamage(double amt, int cause)
{
    //if hit by ship, get killed
    if(cause == HIT_BY_SHIP)
    {
        //getWorld()->playSound(SOUND_DEATH);
        isKilled();
    }
    //if hit by projectile, assess condition
    else if(cause == HIT_BY_PROJECTILE)
    {
        decreaseHitPoints(amt);
        if(hitPoints() > 0)
            getWorld()->playSound(SOUND_BLAST);
        else
        {
            getWorld()->increaseScore(250);
            //getWorld()->playSound(SOUND_DEATH);
            isKilled();
        }
    }
}
void Alien::move()
{
    if(m_dy == UP)
    {
        moveTo(getX() - m_speed, getY() + m_speed);
    }
    else if(m_dy == DOWN)
    {
        moveTo(getX() - m_speed, getY() - m_speed);
    }
    else
    {
        moveTo(getX() - m_speed, getY());
    }
    m_flightPlan--;
}
//damage colliding player by amt
void Alien::damageCollidingPlayer(double amt)
{
    getWorld()->getPlayer()->decreaseHitPoints(amt);
}
//returns true if alien is in range of player
bool Alien::inRange()
{
    if(getWorld()->playerInLineOfFire(this))
    {
        return true;
    }
    return false;
}
//fires a projectile with appropriate probability
//based on Smallgon and Smoregon
bool Alien::fireProjectile()
{
    int total = 20 / getWorld()->getLevel() + 5;
    int n = randInt(1, total);
    if(n == 1)
    {
        getWorld()->addActor(new Turnip(getWorld(), getX() - 14, getY()));
        getWorld()->playSound(SOUND_ALIEN_SHOOT);
        return true;
    }
    else
        return false;
}
//check if alien has collided with anything
void Alien::beginCheckCollision()
{
    //check if alien has collided with projectiles
    getWorld()->collideWithProjectiles(this);
    //check if alien has collided with player
    if(getWorld()->collideWithPlayer(this))
    {
        //alien suffers damage and dies
        sufferDamage(0, HIT_BY_SHIP);
        //colliding player also suffers damage
        damageCollidingPlayer(SMALLGON_SMOREGON_DAMAGE_AMT);
    }
}
Smallgon::Smallgon(StudentWorld* w, double startX, double startY)
:Alien(w, IID_SMALLGON, startX, startY, 5 * (1 + (w->getLevel() - 1) * 0.1), SMALLGON_SMOREGON_DAMAGE_AMT, LEFT, 0, SMALLGON_SMOREGON_SPEED, SMALLGON_SMOREGON_SCORE_VALUE, SMALLGON_SMOREGON_FP)
{
    
}
void Smallgon::doSomething()
{
    //if Smallgon is dead or has flown off the screen, return
    if(!initAlienDoSomething())
        return;
    //check if Smallgon has collided with anything
    beginCheckCollision();
    //check and update flight plan
    checkAndUpdateFlightPlan();
    //if player is in range of alien, alien may fire projectile
    if(inRange())
    {
        if(fireProjectile())
            return;
    }
    move();
    //check if Smallgon has collided with anything
    beginCheckCollision();
}
Smoregon::Smoregon(StudentWorld* w, double startX, double startY)
:Alien(w, IID_SMOREGON, startX, startY, 5 * (1 + (w->getLevel() - 1) * 0.1), SMALLGON_SMOREGON_DAMAGE_AMT, LEFT, 0, SMALLGON_SMOREGON_SPEED, SMALLGON_SMOREGON_SCORE_VALUE, SMALLGON_SMOREGON_FP)
{
    
}
void Smoregon::doSomething()
{
    //if Smoregon is dead or has flown off the screen, return
    if(!initAlienDoSomething())
        return;
    //check if Smoregon has collided with anything
    beginCheckCollision();
    //check and update flight plan
    checkAndUpdateFlightPlan();
    //if player is in range, Smoregon may fire a projectile or speed up
    if(inRange())
    {
        if(fireProjectile())
            return;
        int n = randInt(1, 20 / getWorld()->getLevel() + 5);
        if(n == 1)
        {
            setDeltaY(0);
            setFlightPlan(VIEW_WIDTH);
            setSpeed(5); 
        }
    }
    move();
    //check if Smoregon has collided with anything
    beginCheckCollision();
}
//Smoregon may drop a goodie
void Smoregon::possiblyDropGoodie()
{
    int goodie = randInt(1, 3);
    if(goodie == 1)
    {
        int type = randInt(1, 2);
        if(type == 1)
            getWorld()->addActor(new RepairGoodie(getWorld(), getX(), getY()));
        else if(type == 2)
            getWorld()->addActor(new TorpedoGoodie(getWorld(), getX(), getY()));
    }
}
void Smoregon::sufferDamage(double amt, int cause)
{
    //if hit by NachenBlaster, increase score and possibly drop goodie
    //then Smoregon dies
    if(cause == HIT_BY_SHIP)
    {
        getWorld()->increaseScore(250);
        possiblyDropGoodie();
        //getWorld()->playSound(SOUND_DEATH);
        isKilled();
    }
    //if hit by projectile, decrease hit points and play sound effect
    else if(cause == HIT_BY_PROJECTILE)
    {
        decreaseHitPoints(amt);
        //check if Smoregon is still alive
        if(hitPoints() > 0)
            getWorld()->playSound(SOUND_BLAST);
        else
        {
            getWorld()->increaseScore(250);
            possiblyDropGoodie();
            //getWorld()->playSound(SOUND_DEATH);
            isKilled();
        }
    }
}
Snagglegon::Snagglegon(StudentWorld* w, double startX, double startY)
:Alien(w, IID_SNAGGLEGON, startX, startY, 10 * (1 + (w->getLevel() - 1) * 0.1), SNAGGLEGON_DAMAGE_AMT, LEFT, DOWN, SNAGGLEGON_SPEED, SNAGGLEGON_SCORE_VALUE, VIEW_WIDTH + 10)
{
    
}
//check if Snagglegon has collided with Nachenblaster or projectiles
void Snagglegon::beginCheckCollision()
{
    //check if Snagglegon has collided with projectiles
    getWorld()->collideWithProjectiles(this);
    if(getWorld()->collideWithPlayer(this))
    {
        //if Snagglegon collides with player, deal appropriate damage to player
        sufferDamage(0, HIT_BY_SHIP);
        damageCollidingPlayer(SNAGGLEGON_DAMAGE_AMT);
    }
}
int Snagglegon::identifier()
{
    return SNAGGLEGON;
}
//Snagglegon may fire a projectile
bool Snagglegon::fireProjectile()
{
    int total = 15 / getWorld()->getLevel() + 10;
    int n = randInt(1, total);
    if(n == 1)
    {
        getWorld()->addActor(new AlienLaunchedTorpedo(getWorld(), getX() - 14, getY()));
        getWorld()->playSound(SOUND_TORPEDO);
        return true;
    }
    else
        return false;
}

void Snagglegon::sufferDamage(double amt, int cause)
{
    //if hit by player, increase score by 1000 points, possibly drop goodie, and die
    if(cause == HIT_BY_SHIP)
    {
        getWorld()->increaseScore(1000);
        //getWorld()->playSound(SOUND_DEATH);
        isKilled();
        possiblyDropGoodie();
    }
    //if hit by projectile, suffer damage and assess state
    else if(cause == HIT_BY_PROJECTILE)
    {
        decreaseHitPoints(amt);
        if(isAlive())
            getWorld()->playSound(SOUND_BLAST);
        else
        {
            getWorld()->increaseScore(1000);
            //getWorld()->playSound(SOUND_DEATH);
            isKilled();
            possiblyDropGoodie();
        }
    }
}
//possibly drop goodie
void Snagglegon::possiblyDropGoodie()
{
    int goodie = randInt(1, 6);
    if(goodie == 1)
    {
        getWorld()->addActor(new ExtraLifeGoodie(getWorld(), getX(), getY()));
    }
}
void Snagglegon::checkAndUpdateFlightPlan()
{
    if(getY() >= VIEW_HEIGHT - 1)
        setDeltaY(DOWN);
    else if(getY() <= 0)
        setDeltaY(UP);
}
//if Snagglegon is not alive or has flown off the left side of the screen
//return false
bool Snagglegon::initAlienDoSomething()
{
    if(!isAlive())
        return false;
    if(getX() < 0)
    {
        setState(false);
        return false;
    }
    return true;
}

void Snagglegon::doSomething()
{
    //check Snagglegon's state
    if(!initAlienDoSomething())
    {
        getWorld()->decAliens();
        return;
    }
    //check for collision
    beginCheckCollision();
    //check and update flight plan
    checkAndUpdateFlightPlan();
    //if player is in range, maybe fire projectile
    if(inRange())
    {
        if(fireProjectile())
            return;
    }
    move();
    //check for collision again
    beginCheckCollision();
}
Projectile::Projectile(StudentWorld* w, int imageID, double startX, double startY, double damageAmt, double deltaX, bool rotates, int imageDir)
: Actor(w, imageID, startX, startY, imageDir, PROJECTILE_SIZE, PROJECTILE_DEPTH)
{
    m_damageAmt = damageAmt;
    m_dx = deltaX;
    m_rotates = rotates;
}
int Projectile::identifier()
{
    return REG_PROJECTILE;
}

bool Projectile::isCollidable()
{
    return true;
}
bool Projectile::isDamageable()
{
    return false;
}
int Projectile::isFriendlyOrEnemy()
{
    return NA;
}
void Projectile::rotate(int change_dir)
{
    if(m_rotates)
        setDirection(getDirection() + change_dir);
}
//helper method that checks if Projectile has collided with an Alien
bool Projectile::checkC(double amt)
{
    Actor* pos = getWorld()->getOneCollidingAlien(this);
    if(pos != nullptr)
    {
        pos->sufferDamage(amt, HIT_BY_PROJECTILE);
        return true;
    }
    return false;
}
void Projectile::sufferDamage(double amt, int cause)
{
    return;
}
Cabbage::Cabbage(StudentWorld* w, double startX, double startY)
: Projectile(w, IID_CABBAGE, startX, startY, CABBAGE_TURNIP_DAMAGE_AMT, RIGHT, true, CABBAGE_TURNIP_DIR)
{
    
}
void Cabbage::doSomething()
{
    //check state of Cabbage
    if(!initDoSomething())
        return;
    //check if Projectile has collided with alien
    if(checkC(CABBAGE_TURNIP_DAMAGE_AMT))
    {
        getWorld()->playSound(SOUND_BLAST);
        setState(false);
        return;
    }
    moveTo(getX() + 8, getY());
    rotate(20);
    //check again if Projectile has collided
    if(checkC(CABBAGE_TURNIP_DAMAGE_AMT))
   {
       getWorld()->playSound(SOUND_BLAST);
       setState(false);
       return;
   }
}
int Cabbage::isFriendlyOrEnemy()
{
    return FRIENDLY;
}
Turnip::Turnip(StudentWorld* w, double startX, double startY)
: Projectile(w, IID_TURNIP, startX, startY, CABBAGE_TURNIP_DAMAGE_AMT, RIGHT, true, CABBAGE_TURNIP_DIR)
{
    
}
int Turnip::isFriendlyOrEnemy()
{
    return ENEMY;
}
void Turnip::doSomething()
{
    //check state of Turnip
    if(!initDoSomething())
        return;
    //check if Turnip has collided with Player
    if(getWorld()->collideWithPlayer(this))
    {
        getWorld()->getPlayer()->sufferDamage(CABBAGE_TURNIP_DAMAGE_AMT, HIT_BY_PROJECTILE);
        setState(false);
        return;
    }
    moveTo(getX() - 6, getY());
    rotate(20);
    //check if Turnip has collided with Player
    if(getWorld()->collideWithPlayer(this))
    {
        getWorld()->getPlayer()->sufferDamage(CABBAGE_TURNIP_DAMAGE_AMT, HIT_BY_PROJECTILE);
        setState(false);
    }
}
Torpedo::Torpedo(StudentWorld* w, double startX, double startY, double deltaX, int imageDir)
: Projectile(w, IID_TORPEDO, startX, startY, TORPEDO_DAMAGE_AMT, deltaX, false, imageDir)
{
    
}

int Torpedo::identifier()
{
    return TORPEDO;
}
int Torpedo::isFriendlyOrEnemy()
{
    return NA;
}
PlayerLaunchedTorpedo::PlayerLaunchedTorpedo(StudentWorld* w, double startX, double startY)
: Torpedo(w, startX, startY, RIGHT, PLAYER_TORPEDO_DIR)
{
    
}
int PlayerLaunchedTorpedo::isFriendlyOrEnemy()
{
    return FRIENDLY;
}
void PlayerLaunchedTorpedo::doSomething()
{
    if(!initDoSomething())
        return;
    //check if torpedo has collided with an alien
    if(checkC(TORPEDO_DAMAGE_AMT))
    {
        getWorld()->playSound(SOUND_BLAST);
        setState(false);
        return;
    }
    moveTo(getX() + 8, getY());
    //check again if torpedo has collided with an alien
    if(checkC(TORPEDO_DAMAGE_AMT))
    {
        getWorld()->playSound(SOUND_BLAST);
        setState(false);
    }
}
AlienLaunchedTorpedo::AlienLaunchedTorpedo(StudentWorld* w, double startX, double startY)
: Torpedo(w, startX, startY, LEFT, ALIEN_TORPEDO_DIR)
{
    
}
int AlienLaunchedTorpedo::isFriendlyOrEnemy()
{
    return ENEMY;
}
void AlienLaunchedTorpedo::doSomething()
{
    if(!initDoSomething())
        return;
    //check if torpedo has collided with player
    if(getWorld()->collideWithPlayer(this))
    {
        getWorld()->getPlayer()->sufferDamage(TORPEDO_DAMAGE_AMT, HIT_BY_PROJECTILE);
        setState(false);
        return;
    }
    moveTo(getX() - 8, getY());
    //check if torpedo has collided with player
    if(getWorld()->collideWithPlayer(this))
    {
        getWorld()->getPlayer()->sufferDamage(TORPEDO_DAMAGE_AMT, HIT_BY_PROJECTILE);
        setState(false);
    }
}
Goodie::Goodie(StudentWorld* w, int imageID, double startX, double startY)
: Actor(w, imageID, startX, startY, GOODIE_DIRECTION, GOODIE_SIZE, GOODIE_DEPTH)
{
    
}
bool Goodie::isCollidable()
{
    return true;
}
bool Goodie::isDamageable()
{
    return false;
}
int Goodie::isFriendlyOrEnemy()
{
    return ENEMY;
}

int Goodie::identifier()
{
    return NA;
}
//check if Goodie has collided with player
bool Goodie::checkCollide()
{
    if(getWorld()->collideWithPlayer(this))
    {
        getWorld()->increaseScore(100);
        getWorld()->playSound(SOUND_GOODIE);
        return true;
    }
    return false;
}
void Goodie::sufferDamage(double amt, int cause)
{
    return;
}
ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* w, double startX, double startY)
: Goodie(w, IID_LIFE_GOODIE, startX, startY)
{
    
}
void ExtraLifeGoodie::doSomething()
{
    if(!initDoSomething())
        return;
    //check if player has picked up goodie, then confer benefits on player
    if(checkCollide())
    {
        getWorld()->incLives();
        setState(false);
        return;
    }
    moveTo(getX() - 0.75, getY() - 0.75);
    //check again if player has picked up goodie, then confer benefits on player
    if(checkCollide())
    {
        getWorld()->incLives();
        setState(false);
        return;
    }
}
int ExtraLifeGoodie::identifier()
{
    return EL_GOODIE;
}
RepairGoodie::RepairGoodie(StudentWorld* w, double startX, double startY)
: Goodie(w, IID_REPAIR_GOODIE, startX, startY)
{
    
}
int RepairGoodie::identifier()
{
    return R_GOODIE;
}
void RepairGoodie::doSomething()
{
    if(!initDoSomething())
        return;
    //check if player has picked up goodie, then confer benefits on player
    if(checkCollide())
    {
        if(getWorld()->getPlayer()->hitPoints() < 40)
            getWorld()->getPlayer()->increaseHitPoints(10);
        setState(false);
        return;
    }
    moveTo(getX() - 0.75, getY() - 0.75);
    //check again if player has picked up goodie, then confer benefits on player
    if(checkCollide())
    {
        if(getWorld()->getPlayer()->hitPoints() < 40)
            getWorld()->getPlayer()->increaseHitPoints(10);
        setState(false);
        return;
    }
}
TorpedoGoodie::TorpedoGoodie(StudentWorld* w, double startX, double startY)
: Goodie(w, IID_TORPEDO_GOODIE, startX, startY)
{
    
}
int TorpedoGoodie::identifier()
{
    return F_GOODIE;
}
void TorpedoGoodie::doSomething()
{
    if(!initDoSomething())
        return;
    //check again if player has picked up goodie, then confer benefits on player
    if(checkCollide())
    {
        getWorld()->getPlayer()->increaseTorpedoes(5);
        setState(false);
        return;
    }
    moveTo(getX() - 0.75, getY() - 0.75);
    //check again if player has picked up goodie, then confer benefits on player
    if(checkCollide())
    {
        getWorld()->getPlayer()->increaseTorpedoes(5);
        setState(false);
        return;
    }
}
