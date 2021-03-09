#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>
#include <iomanip>
#include <tgmath.h>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

#include "Actor.h"

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld()
{
	if (ptrSocrates != nullptr)
		cleanUp();
}

int StudentWorld::init()
{
	ptrSocrates = new Socrates(this);
	bool openSpace = false;

	Pit a(this, VIEW_RADIUS, VIEW_RADIUS);
	
	for (int k = 0; k < getLevel(); k++)
	{
		addObj(a);
		while (openSpace == false)
		{
			m_Actors.back()->moveTo(VIEW_RADIUS, VIEW_RADIUS);
			(m_Actors.back())->moveAngle(randInt(0, 359), randInt(0, VIEW_RADIUS - 8));
			openSpace = prevOverLap();
		}
		openSpace = false;
		amtofBactLeft++;
	}
	
	Food b(this, VIEW_RADIUS, VIEW_RADIUS);
	for (int l = 0; l < min(5 * getLevel(), 25); l++)
	{
		addObj(b);
		while (openSpace == false)
		{
			m_Actors.back()->moveTo(VIEW_RADIUS, VIEW_RADIUS);
			(m_Actors.back())->moveAngle(randInt(0, 359), randInt(0, VIEW_RADIUS - 8));
			openSpace = prevOverLap();
		}
		openSpace = false;
	}

	DirtPile c(this, VIEW_RADIUS, VIEW_RADIUS);
	for (int j = 0; j < max(180 - 20 * getLevel(), 20); j++)
	{
		addObj(c);
		while (openSpace == false)
		{
			m_Actors.back()->moveTo(VIEW_RADIUS, VIEW_RADIUS);
			(m_Actors.back())->moveAngle(randInt(0, 359), randInt(0, VIEW_RADIUS - 8));
			openSpace = prevOverLap();
		}
		openSpace = false;
	}
	

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	ptrSocrates->doSomething();

	for (unsigned int i = 0; i < m_Actors.size(); i++)
	{
		if (!ptrSocrates->isAlive())
		{
			decLives();
			return GWSTATUS_PLAYER_DIED;

		}

		m_Actors[i]->doSomething();

		if (amtofBactLeft <= 0)
		{
			playSound(SOUND_FINISHED_LEVEL);
			return GWSTATUS_FINISHED_LEVEL;
		}
	}

	for (unsigned int i = 0; i < m_Actors.size(); i++)//remove and delete dead game objects
	{
		if (!m_Actors[i]->isAlive())
		{
			vector<Actor*>::iterator it = m_Actors.begin();
			it = it + i;
			delete m_Actors[i];
			m_Actors.erase(it);
		}
	}

	Fungus d(this, VIEW_RADIUS, VIEW_RADIUS);
	if (randInt(0, max(510 - getLevel() * 10, 200)) == 0)
	{
		addObj(d);
		m_Actors.back()->moveAngle(randInt(0, 359), VIEW_RADIUS);
	}

	if (randInt(0, max(510 - getLevel() * 10, 250)) == 0)
	{
		int choose = randInt(1, 10);
		if (choose == 1)
		{
			ExtraLifeGoodie e(this, VIEW_RADIUS, VIEW_RADIUS);
			addObj(e);
		}
		else if (choose > 1 && choose < 5)
		{
			FlameThrowerGoodie f(this, VIEW_RADIUS, VIEW_RADIUS);
			addObj(f);
		}

		else
		{
			RestoreHealthGoodie g(this, VIEW_RADIUS, VIEW_RADIUS);
			addObj(g);
		}
		
		m_Actors.back()->moveAngle(randInt(0, 355), VIEW_RADIUS);
	}

	ostringstream oss, lvl, lvs, hp, sp, fire;//deletelater
	oss.fill('0');
	oss << setw(6) << getScore();
	lvl << getLevel();
	lvs << getLives();
	hp << ptrSocrates->getHealth();
	sp << ptrSocrates->getSprays();
	fire << ptrSocrates->getCharges();

	std::string temp = "Score: " + oss.str() + "  Level: " + lvl.str() + "  Lives: " + lvs.str() + "  health: " + hp.str() + "  Sprays: " + sp.str() + "  Flames: " + fire.str();//delete yote
	setGameStatText(temp);
		//update the status text line

    return GWSTATUS_CONTINUE_GAME;
}



double StudentWorld::calcDistance(Actor* ptr1, Actor* ptr2, double& diffx, double& diffy)
{

	double x1 = ptr1->getX();
	double x2 = ptr2->getX();
	double y1 = ptr1->getY();
	double y2 = ptr2->getY();
	y2 = y2 - y1;
	x2 = x2 - x1;

	diffx = x2;
	diffy = y2;

	y2 = y2 * y2;
	x2 = x2 * x2;

	double temp = sqrt(x2 + y2);
	return temp;
}

bool StudentWorld::prevOverLap() 
{
	for (unsigned int i = 0; i < m_Actors.size() - 1; i++)
	{

		double x;

		if (calcDistance(m_Actors.back(), m_Actors[i], x, x) <= SPRITE_WIDTH)
		{
			return false;
		}
	}

	return true;
}

bool StudentWorld::AffectBetter(Actor* ptr, int distance, bool (Actor::* f)() const, bool willDamage, int amt)
{
	double x;
	bool test = false;

	for (unsigned int i = 0; i < m_Actors.size(); i++)
	{
		Actor* temp = m_Actors[i];

		if (willDamage == false && amt != 0)
		{
			willDamage = true;
			test = true;
			temp = ptrSocrates;
		}

		else if (!(m_Actors[i]->*f)() || !m_Actors[i]->isAlive())
			continue;

		if (calcDistance(ptr, temp, x, x) <= distance)
		{
			if (willDamage)
				temp->Damage(amt);

			return true;
		}

		if (test)
			return false;
	}

	return false;
}

double StudentWorld::angleToFood(Actor* ptr)
{
	double x, y, minAngle = -1,  minDistance = -1;

	for (unsigned int i = 0; i < m_Actors.size(); i++)
	{
		if (!m_Actors[i]->BactConsumable() || !m_Actors[i]->isAlive())
			continue;

		if (calcDistance(ptr, m_Actors[i], x, y) <= VIEW_RADIUS)
		{
			if (minDistance == -1 || minDistance > calcDistance(ptr, m_Actors[i], x, y))
			{
				minAngle = calcAngle(x, y);
				minDistance = calcDistance(ptr, m_Actors[i], x, y);
			}
		}
	}

	return minAngle;
}

void StudentWorld::cleanUp()
{
	delete ptrSocrates;

	vector<Actor*>::iterator it;

	for(vector<Actor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); it++)
	{
		delete* it;
	}

	for (unsigned int i = 0; i < m_Actors.size(); )
	{
		it = m_Actors.begin();
		m_Actors.erase(it);
	}

	amtofBactLeft = 0;
}

double StudentWorld::calcAngle(double diffX, double diffY)
{
	double temp = atan2(diffY, diffX);
	temp = temp * 180 / 3.14159;
	return temp;
}
