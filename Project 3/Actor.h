#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* ptToWorld, int imageID, double startX, double startY, int dir = 0, int depth = 1, double size = 1);

	virtual ~Actor() {};

	virtual bool isAlive() const { return status; }

	virtual void setDead() { status = false; }

	virtual void doSomething() {}

	virtual StudentWorld* getAPointerToMyStudentWorld() { return ptrToWorld; };

	virtual bool BactConsumable() const { return false; };

	virtual bool Damageable() const { return false; };

	virtual void Damage(int amt) { setDead(); }

	virtual bool canBlock() const { return false; } 

private:
	bool status = true;
	StudentWorld* ptrToWorld;
};


class DirtPile : public Actor
{
public:
	DirtPile(StudentWorld* ptrToWorld, double startX, double startY, int dir = 0) : Actor(ptrToWorld, IID_DIRT, startX, startY) {}
	
	virtual ~DirtPile() {};
	
	virtual bool Damageable() const { return true; }
	
	virtual bool canBlock() const { return true; }
};

class Pit : public Actor
{
public:
	Pit(StudentWorld* ptrToWorld, double startX, double startY, int dir = 0) :Actor(ptrToWorld, IID_PIT, startX, startY) {}
	
	virtual ~Pit() {};
	
	virtual void doSomething();

private:
	int regSalm = 5;
	int agrSalm = 3; 
	int eColi = 2;
};


class ActivatingObject : public Actor
{
public:
	ActivatingObject(StudentWorld* ptrToWorld, int imageID, double startX, double startY, int dir = 0, int max = 0, int atk = 0);
	
	virtual ~ActivatingObject() {};
	
	virtual void doSomething();

private:
	int maxDist;
	int dmgToDeal;
};

class Food : public ActivatingObject
{
public:
	Food(StudentWorld* ptrToWorld, double startX, double startY, int dir = 0) : ActivatingObject(ptrToWorld, IID_FOOD, startX, startY, 90) {}
	
	virtual ~Food() {};
	
	virtual bool BactConsumable()  const { return true; };

	virtual void doSomething() {}
};


class Flame : public ActivatingObject
{
public:
	Flame(StudentWorld* ptrToWorld, double startX, double startY, int dir) :ActivatingObject(ptrToWorld, IID_FLAME, startX, startY, dir, 32, 5) {}
	
	virtual ~Flame() {};
};


class Spray : public ActivatingObject
{
public:
	Spray(StudentWorld* ptrToWorld, double startX, double startY, int dir) : ActivatingObject(ptrToWorld, IID_SPRAY, startX, startY, dir, 112, 2) {}
	
	virtual ~Spray() {};
};


class Goodie : public ActivatingObject
{
public:
	Goodie(StudentWorld* ptrToWorld, int imageID, double startX, double startY, int restoreAmt);
	
	virtual ~Goodie() {};
	
	virtual bool Damageable() const { return true; };
	
	virtual void doSomething();

private:
	int lifetime;
	int amtToRestore;
};


class RestoreHealthGoodie : public Goodie
{
public:
	RestoreHealthGoodie(StudentWorld* ptrToWorld, double startX, double startY, int dir = 0) :Goodie(ptrToWorld, IID_RESTORE_HEALTH_GOODIE, startX, startY,  -100) {}
	
	virtual ~RestoreHealthGoodie() {};
};

class FlameThrowerGoodie : public Goodie
{
public:
	FlameThrowerGoodie(StudentWorld* ptrToWorld, double startX, double startY, int dir = 0) :Goodie(ptrToWorld, IID_FLAME_THROWER_GOODIE, startX, startY, 5) {}
	
	virtual ~FlameThrowerGoodie() {};

};

class ExtraLifeGoodie : public Goodie
{
public: 
	ExtraLifeGoodie(StudentWorld* ptrToWorld, double startX, double startY, int dir = 0) : Goodie(ptrToWorld, IID_EXTRA_LIFE_GOODIE, startX, startY, 0) {}
	
	virtual ~ExtraLifeGoodie() {};

};

class Fungus : public Goodie
{
public:
	Fungus(StudentWorld* ptrToWorld, double startX, double startY, int dir = 0) : Goodie(ptrToWorld, IID_FUNGUS, startX, startY, 20) {}
	
	virtual ~Fungus() {};

};


class Agent : public Actor
{
public:
	Agent(StudentWorld* ptrToWorld, int imageID, double startX, double startY, int health, int HurtSound, int deathSound, int dir = 90, bool isaBact = true);
	
	virtual ~Agent() {};
	
	int getHealth() const { return hp; };
	
	virtual void Damage(int amt);

private:
	int hp;
	int dieSound;
	int hurtSound;
	bool isBact;
};


class Socrates : public Agent
{
public:
	Socrates(StudentWorld* ptr) : Agent(ptr, IID_PLAYER, 0, 128, 100, SOUND_PLAYER_HURT, SOUND_PLAYER_DIE, 0) {}

	virtual ~Socrates() {};

	virtual void doSomething();

	int getSprays() const { return m_numSprays; };

	int getCharges() const { return m_numCharges; };

	void restoreCharge() { m_numCharges = 5; };

private:
	int m_numSprays = 20;
	int m_numCharges = 5;
};

class Bacterium : public Agent
{
public:
	Bacterium(StudentWorld* ptrToWorld, double startX, double startY, int imageID, int health, int hurtSound, int deathSound, int atk);
	
	virtual ~Bacterium() {};
	
	virtual bool Damageable() const { return true; };
	
	virtual void doSomething();
private:
	int foodEaten = 0;
	int dmg;
};

class Ecoli : public Bacterium
{
public:
	Ecoli(StudentWorld* ptrToWorld, double startX, double startY, int dir = 0) :Bacterium(ptrToWorld, startX, startY, IID_ECOLI, 5, SOUND_ECOLI_HURT, SOUND_ECOLI_DIE, 4) {}
	
	virtual ~Ecoli() {};
	
	virtual void doSomething();

};

class Salmonella : public Bacterium
{
public:
	Salmonella(StudentWorld* ptrToWorld, double startX, double startY, int health, int atk) :Bacterium(ptrToWorld, startX, startY, IID_SALMONELLA, health, SOUND_SALMONELLA_HURT, SOUND_SALMONELLA_DIE, atk) {}
	
	virtual ~Salmonella() {};
	
	virtual void doSomething();

private:
	int movePlan = 0;
};


class RegularSalmonella : public Salmonella
{
public:
	RegularSalmonella(StudentWorld* ptrToWorld, double startX, double startY, int dir = 0) :Salmonella(ptrToWorld, startX, startY, 4, 1) {}


	virtual ~RegularSalmonella() {};

};


class AgressiveSalmonella : public Salmonella
{
public:
	AgressiveSalmonella(StudentWorld* ptrToWorld, double startX, double startY, int dir = 0) : Salmonella(ptrToWorld, startX, startY, 10, 2) {}
	
	virtual ~AgressiveSalmonella() {};
	
	virtual void doSomething();

};

#endif // ACTOR_H_
