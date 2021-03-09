#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* ptToWorld, int imageID, double startX, double startY, int dir, int depth, double size)
	:GraphObject(imageID, startX, startY, dir, depth, size) 
{
	ptrToWorld = ptToWorld;
}


void Pit::doSomething()
{
	if (!isAlive())
		return;

	if (randInt(1, 50) == 1)
	{
		bool done = false;

		while (done == false)
		{
			switch (randInt(1, 3))
			{
			case 1:
				if (regSalm != 0)
				{
					RegularSalmonella a(getAPointerToMyStudentWorld(), getX(), getY());
					getAPointerToMyStudentWorld()->addObj(a);
					regSalm--;
					done = true;
				}
				break;

			case 2:
				if (agrSalm != 0)
				{
					AgressiveSalmonella a(getAPointerToMyStudentWorld(), getX(), getY());
					getAPointerToMyStudentWorld()->addObj(a);
					agrSalm--;
					done = true;
				}
				break;

			case 3:
				if (eColi != 0)
				{
					Ecoli a(getAPointerToMyStudentWorld(), getX(), getY());
					getAPointerToMyStudentWorld()->addObj(a);
					eColi--;
					done = true;
				}
				break;

			}
		}
		getAPointerToMyStudentWorld()->affectBactLeft(1);
		getAPointerToMyStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
	}

	if (eColi <= 0 && regSalm <= 0 && agrSalm <= 0)
	{
		setDead();
		getAPointerToMyStudentWorld()->affectBactLeft(-1);
	}
}

ActivatingObject::ActivatingObject(StudentWorld* ptrToWorld, int imageID, double startX, double startY, int dir, int max, int atk)
	: Actor(ptrToWorld, imageID, startX, startY, dir) 
{
	maxDist = max;
	dmgToDeal = atk;
}

void ActivatingObject::doSomething()
{
	if (!isAlive())
		return;

	if (getAPointerToMyStudentWorld()->AffectBetter(this,  SPRITE_WIDTH, &Actor::Damageable, true, dmgToDeal))
	{
		setDead();
		return;
	}

	moveAngle(getDirection(), SPRITE_WIDTH);

	maxDist -= SPRITE_WIDTH;

	if (maxDist <= 0)
		setDead();

}

Goodie::Goodie(StudentWorld* ptrToWorld, int imageID, double startX, double startY, int restoreAmt)
	:ActivatingObject(ptrToWorld, imageID, startX, startY) 
{
	lifetime = std::max(randInt(0, 300 - (10 * getAPointerToMyStudentWorld()->getLevel()) - 1), 50);
	amtToRestore = restoreAmt;
}

void Goodie::doSomething()
{
	double x;

	if (!isAlive())
		return;

	if (getAPointerToMyStudentWorld()->calcDistance(this, getAPointerToMyStudentWorld()->getAPointerToSocrates(), x, x) <= SPRITE_WIDTH)
	{
		setDead();
		switch (amtToRestore)
		{
		case -100:
			getAPointerToMyStudentWorld()->AffectBetter(this, SPRITE_WIDTH, &Actor::Damageable, false, amtToRestore);
			getAPointerToMyStudentWorld()->increaseScore(250);
			break;

		case 5:
			getAPointerToMyStudentWorld()->getAPointerToSocrates()->restoreCharge();
			getAPointerToMyStudentWorld()->increaseScore(300);
			break;

		case 0:
			getAPointerToMyStudentWorld()->increaseScore(500);
			getAPointerToMyStudentWorld()->incLives();
			break;

		case 20:
			getAPointerToMyStudentWorld()->increaseScore(-50);
			getAPointerToMyStudentWorld()->AffectBetter(this, SPRITE_WIDTH, &Actor::Damageable, false, amtToRestore);
			break;
		}

		if (amtToRestore != 20)
			getAPointerToMyStudentWorld()->playSound(SOUND_GOT_GOODIE);

		return;
	}

	lifetime--;

	if (lifetime <= 0)
		setDead();
}


Agent::Agent(StudentWorld* ptrToWorld, int imageID, double startX, double startY, int health, int HurtSound, int deathSound, int dir, bool isaBact)
	:Actor(ptrToWorld, imageID, startX, startY, dir, 0)
{
	hp = health;
	dieSound = deathSound;
	hurtSound = HurtSound;
	isBact = isaBact;
}


void Agent::Damage(int amt)
{
	hp -= amt;

	if (amt > 0)
		getAPointerToMyStudentWorld()->playSound(hurtSound);

	if (hp <= 0)
	{
		getAPointerToMyStudentWorld()->playSound(dieSound);
		setDead();
		if (isBact)
		{
			getAPointerToMyStudentWorld()->increaseScore(100);
			if (randInt(1, 2) == 1)
			{
				Food a(getAPointerToMyStudentWorld(), getX(), getY());
				getAPointerToMyStudentWorld()->addObj(a);
			}
			getAPointerToMyStudentWorld()->affectBactLeft(-1);
		}
	}

	if (amt < 0)
		hp = 100;
}

void Socrates::doSomething()
{
	if (!isAlive())
		return;

	int ch;
	if (getAPointerToMyStudentWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			moveTo(VIEW_RADIUS, VIEW_RADIUS);
			moveAngle(getDirection() - 175, VIEW_RADIUS);
			setDirection(getDirection() + 5);
			break;

		case KEY_PRESS_RIGHT:
			moveTo(VIEW_RADIUS, VIEW_RADIUS);
			moveAngle(getDirection() + 175, VIEW_RADIUS);
			setDirection(getDirection() - 5);
			break;

		case KEY_PRESS_ENTER:
			if (getCharges() > 0)
			{
				for (int i = 0; i < 16; i++)
				{
					Flame a(getAPointerToMyStudentWorld(), getX(), getY(), getDirection() + i * 22);
					a.moveAngle(a.getDirection(), SPRITE_WIDTH);
					getAPointerToMyStudentWorld()->addObj(a);
				}

				m_numCharges--;
				getAPointerToMyStudentWorld()->playSound(SOUND_PLAYER_FIRE);
			}
			break;

		case KEY_PRESS_SPACE:
			if (getSprays() > 0)
			{
				Spray a(getAPointerToMyStudentWorld(), getX(), getY(), getDirection());
				a.moveAngle(a.getDirection(), SPRITE_WIDTH);
				getAPointerToMyStudentWorld()->addObj(a);
				m_numSprays--;
				getAPointerToMyStudentWorld()->playSound(SOUND_PLAYER_SPRAY);
			}
			break;

		}
	}

	else
	{
		if (getSprays() < 20)
			m_numSprays++;
	}
}

Bacterium::Bacterium(StudentWorld* ptrToWorld, double startX, double startY, int imageID, int health, int hurtSound, int deathSound, int atk)
	: Agent(ptrToWorld, imageID, startX, startY, health, hurtSound, deathSound)
{
	dmg = atk;
}


void Bacterium::doSomething()
{
	if (!isAlive())
		return;

	double x;
	if (getAPointerToMyStudentWorld()->calcDistance(this, getAPointerToMyStudentWorld()->getAPointerToSocrates(), x, x) <= SPRITE_WIDTH)
		getAPointerToMyStudentWorld()->AffectBetter(this, SPRITE_WIDTH, &Actor::Damageable, false, dmg);

	else if (foodEaten == 3)
	{
		double newX = getX();
		double newY = getY();
		if (newX < VIEW_WIDTH / 2)
			newX += SPRITE_WIDTH / 2;
		else
			newX -= SPRITE_WIDTH / 2;
		if (newY < VIEW_WIDTH / 2)
			newY += SPRITE_WIDTH / 2;
		else
			newY -= SPRITE_WIDTH / 2;

		//add a new object here

		if (dmg == 1)
		{
			RegularSalmonella a(getAPointerToMyStudentWorld(), newX, newY);
			getAPointerToMyStudentWorld()->addObj(a);
		}
		else if (dmg == 2)
		{
			AgressiveSalmonella a(getAPointerToMyStudentWorld(), newX, newY);
			getAPointerToMyStudentWorld()->addObj(a);
		}
		else
		{
			Ecoli a(getAPointerToMyStudentWorld(), newX, newY);
			getAPointerToMyStudentWorld()->addObj(a);
		}

		getAPointerToMyStudentWorld()->affectBactLeft(1);
		foodEaten = 0;
	}
	else if (getAPointerToMyStudentWorld()->AffectBetter(this, SPRITE_WIDTH, &Actor::BactConsumable, true, 0))
		foodEaten++;

}

void Salmonella::doSomething()
{
	if (!isAlive())
		return;

	Bacterium::doSomething();
	if (movePlan > 0)
	{
		movePlan--;
		Actor* temp = new Bacterium(getAPointerToMyStudentWorld(), getX(), getY(), IID_SALMONELLA, 0, 0, 0, 0);
		temp->moveAngle(getDirection(), 3);
		if (getAPointerToMyStudentWorld()->AffectBetter(temp, SPRITE_WIDTH/2, &Actor::canBlock) || sqrt((temp->getX() - VIEW_RADIUS) * (temp->getX() - VIEW_RADIUS) + (temp->getY() - VIEW_RADIUS) * (temp->getY() - VIEW_RADIUS)) >= VIEW_RADIUS)
		{
			setDirection(randInt(0, 359));
			movePlan = 10;
		}
		else
			moveAngle(getDirection(), 3);
		delete temp;
		return;
	}
	else
	{
		double newAngle = getAPointerToMyStudentWorld()->angleToFood(this);
		if (newAngle != -1)
		{
			Actor* temp = new Bacterium(getAPointerToMyStudentWorld(), getX(), getY(), IID_SALMONELLA, 0, 0, 0, 0);
			temp->moveAngle(newAngle, 3);
			if (getAPointerToMyStudentWorld()->AffectBetter(temp, SPRITE_WIDTH / 2, &Actor::canBlock) || sqrt((temp->getX() - VIEW_RADIUS) * (temp->getX() - VIEW_RADIUS) + (temp->getY() - VIEW_RADIUS) * (temp->getY() - VIEW_RADIUS)) >= VIEW_RADIUS)
			{
				setDirection(randInt(0, 359));
				movePlan = 10;
			}
			else 
			{
				setDirection(newAngle);
				moveTo(temp->getX(), temp->getY());
			}

			delete temp;
			return;
		}
		else
		{
			setDirection(randInt(0, 359));
			movePlan = 10;
			return;
		}

	}
}

void AgressiveSalmonella::doSomething()
{

	if (!isAlive())
		return;

	double x;
	bool cont = true;

	if (getAPointerToMyStudentWorld()->calcDistance(this, getAPointerToMyStudentWorld()->getAPointerToSocrates(), x, x) <= 75)
	{
		cont = false;
		double x;
		double y;
		getAPointerToMyStudentWorld()->calcDistance(this, getAPointerToMyStudentWorld()->getAPointerToSocrates(), x, y);
		setDirection(getAPointerToMyStudentWorld()->calcAngle(x, y));
		Actor* temp = new AgressiveSalmonella(getAPointerToMyStudentWorld(), getX(), getY());
		temp->moveAngle(getDirection(), 3);
		if (!getAPointerToMyStudentWorld()->AffectBetter(temp, SPRITE_WIDTH / 2, &Actor::canBlock))
			moveAngle(getDirection(), 3);

		delete temp;

		Bacterium::doSomething();
		return;
	}

	Salmonella::doSomething();
}

void Ecoli::doSomething()
{

	if (!isAlive())
		return;

	double x;
	Bacterium::doSomething();

	if (getAPointerToMyStudentWorld()->calcDistance(this, getAPointerToMyStudentWorld()->getAPointerToSocrates(), x, x) <= 256)
	{
		double x;
		double y;
		getAPointerToMyStudentWorld()->calcDistance(this, getAPointerToMyStudentWorld()->getAPointerToSocrates(), x, y);
		setDirection(getAPointerToMyStudentWorld()->calcAngle(x, y));
		for (int i = 0; i < 10; i++)
		{
			Actor* temp = new Ecoli(getAPointerToMyStudentWorld(), getX(), getY());
			temp->setDirection(getDirection());
			temp->moveAngle(getDirection(), 3);
			if (!getAPointerToMyStudentWorld()->AffectBetter(temp, (SPRITE_WIDTH / 2), &Actor::canBlock))
			{
				moveAngle(getDirection(), 3);
				delete temp;
				return;
			}
			delete temp;
			int x = getDirection();
			x += 10;
			if (x > 360)
				x -= 360;
			setDirection(x);

		}
	}
}








