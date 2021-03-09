#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Socrates;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);

	~StudentWorld();

    virtual int init();

    virtual int move();

    virtual void cleanUp();

	bool AffectBetter(Actor* ptr, int distance, bool (Actor::* f)() const, bool willDamage = false, int amt = 0);

	double angleToFood(Actor* ptr);

	double calcDistance(Actor* ptr1, Actor* ptr2, double& diffx, double& diffy );

	bool prevOverLap();

	void affectBactLeft(int amt) { amtofBactLeft += amt; };

	double calcAngle(double diffX, double diffY);

	Socrates* getAPointerToSocrates() { return ptrSocrates; }

	template<typename Item>
	void addObj(Item& a) {	m_Actors.push_back(new Item(this, a.getX(), a.getY(), a.getDirection()));}

private:
	int amtofBactLeft = 0;
	Socrates* ptrSocrates = nullptr;
	std::vector<Actor*> m_Actors;

};

#endif // STUDENTWORLD_H_
