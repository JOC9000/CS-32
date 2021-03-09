#include "provided.h"
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
	DeliveryPlannerImpl(const StreetMap* sm);
	~DeliveryPlannerImpl();
	DeliveryResult generateDeliveryPlan(
		const GeoCoord& depot,
		const vector<DeliveryRequest>& deliveries,
		vector<DeliveryCommand>& commands,
		double& totalDistanceTravelled) const;
private:
	const StreetMap* map;

	string detDir(double angle) const
	{
		if (angle >= 0 && angle < 22.5)
			return "east";
		if (angle >= 22.5 && angle < 67.5)
			return "northeast";
		if (angle >= 67.5 && angle < 112.5)
			return "north";
		if (angle >= 112.5 && angle < 157.5)
			return "northwest";
		if (angle >= 157.5 && angle < 202.5)
			return "west";
		if (angle >= 202.5 && angle < 247.5)
			return "southwest";
		if (angle >= 247.5 && angle < 292.5)
			return "south";
		if (angle >= 292.5 && angle < 337.5)
			return "east";

		return "east"; //if any other angle(only grater than 337.5 allowed here)
	}

	string detTurnDir(double angle) const
	{
		if (angle >= 1 && angle < 180)
			return "left";

		return "right";	//onlvalues passed should be between 1 and 359, no else needed
	}
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
	map = sm;
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
	const GeoCoord& depot,
	const vector<DeliveryRequest>& deliveries,
	vector<DeliveryCommand>& commands,
	double& totalDistanceTravelled) const
{

	DeliveryOptimizer opt(map);	//setup for optimizing
	double oldDist;
	double newDist;
	vector<DeliveryRequest> placesToGo;
	placesToGo = deliveries;
	opt.optimizeDeliveryOrder(depot, placesToGo, oldDist, newDist);		//optimize deliveries

	commands.clear();		//initializations
	totalDistanceTravelled = 0;
	double temp;
	list<StreetSegment> segList;	//will hold list of segments to traverse
	GeoCoord currentPlace = depot;		//stores location of where we are now

	placesToGo.push_back(DeliveryRequest("depot", depot));	//add depot to end of list to avoid repeat code
	PointToPointRouter a(map);

	for (unsigned int i = 0; i < placesToGo.size() ; i++)
	{
		DeliveryResult res = a.generatePointToPointRoute(currentPlace, placesToGo[i].location, segList, temp);
		totalDistanceTravelled += temp;		//add to distance

		if (res == NO_ROUTE || res == BAD_COORD)	//in case of error return the error message
			return res;

		string curDir;
		double distStreet = 0;		//total distance traversed on current street 
		DeliveryCommand cmd;
		StreetSegment prior;

		for (list<StreetSegment>::iterator it = segList.begin(); it != segList.end(); it++)
		{
			if ((*it).end == placesToGo[i].location)	//if arrived at destination
			{
				distStreet += distanceEarthMiles((*it).start, (*it).end);		//add last bit of distance
				cmd.initAsProceedCommand(curDir, (*it).name, distStreet);	//initialize proceed command
				commands.push_back(cmd);
			}

			else if (prior.name == "")		//if name uninitialized
			{
				curDir = detDir(angleOfLine((*it)));	//set the diection
				distStreet += distanceEarthMiles((*it).start, (*it).end);		//and add to distance
			}

			else if (prior.name == (*it).name)		//if still on same street
				distStreet += distanceEarthMiles((*it).start, (*it).end);		//just add this segments distance to street distance

			else if(prior.name != (*it).name)		//if on a different named street
			{
				cmd.initAsProceedCommand(curDir, prior.name, distStreet);		//make and push the proceed for prior street
				commands.push_back(cmd);
				double tempAngle = angleBetween2Lines(prior, (*it));		//determine if a turn is needed
					
				if(tempAngle >= 1 && tempAngle <= 359)		//if it is create and push the command
				{
					cmd.initAsTurnCommand(detTurnDir(angleBetween2Lines(prior, (*it))), (*it).name);//then turn onto this street
					commands.push_back(cmd);
				}
				distStreet = distanceEarthMiles((*it).start, (*it).end);		//add this segment's distance 
				curDir = detDir(angleOfLine((*it)));		//and initialize the new street's angle
			}

			prior = (*it);		//store value of prior segment for use
		}

		if(i != placesToGo.size() - 1)	//if not the depot, push back deliver command
		{
			cmd.initAsDeliverCommand(placesToGo[i].item);
			commands.push_back(cmd);
		}

		currentPlace = placesToGo[i].location;
	}

	return DELIVERY_SUCCESS;
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
	m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
	delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
	const GeoCoord& depot,
	const vector<DeliveryRequest>& deliveries,
	vector<DeliveryCommand>& commands,
	double& totalDistanceTravelled) const
{
	return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}