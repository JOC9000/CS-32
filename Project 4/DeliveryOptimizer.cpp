#include "provided.h"
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

class DeliveryOptimizerImpl
{
public:
	DeliveryOptimizerImpl(const StreetMap* sm);
	~DeliveryOptimizerImpl();
	void optimizeDeliveryOrder(
		const GeoCoord& depot,
		vector<DeliveryRequest>& deliveries,
		double& oldCrowDistance,
		double& newCrowDistance) const;
private:

	const StreetMap* map;

	double calcPathDist(vector<DeliveryRequest> requests, const GeoCoord& depot) const	//calculates crow's distance of path of deliveries
	{
		if (requests.size() == 0)
			return 0;

		double totalDist = 0;

		totalDist += distanceEarthMiles(requests[0].location, depot);	//from depot to first location

		for (unsigned int i = 1; i < requests.size(); i++)		//from a location to the location prior, starting from second
			totalDist += distanceEarthMiles(requests[i].location, requests[i - 1].location);	

		totalDist += distanceEarthMiles(requests[requests.size() - 1].location, depot);		//from last location back to depot

		return totalDist;
	}
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
	map = sm;
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
	const GeoCoord& depot,
	vector<DeliveryRequest>& deliveries,
	double& oldCrowDistance,
	double& newCrowDistance) const
{
	oldCrowDistance = calcPathDist(deliveries, depot);  //uses function to find crow's distance
	newCrowDistance = oldCrowDistance;		//is the same until changed
	double minimum = oldCrowDistance;		//distance to compare against

	vector<DeliveryRequest> randVector = deliveries;	//vector that will contained shuffled requests
	vector<DeliveryRequest> bestOrder;					//vector that will contain the current best order according to calcPathDist
	bool changedOrder = false;

	random_device rd;  // Will be used to obtain a seed for the random number engine
	mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	uniform_int_distribution<> dis(1000, 1999); // Same distribution as before, but explicit and without bias

	unsigned int limit = deliveries.size();

	for (unsigned int i = 0; i < limit*limit*limit; i++)	//only repeats n^3 times, where n is number of items in deliveries
	{

		shuffle(randVector.begin(), randVector.end(), gen);
		double temp = calcPathDist(randVector, depot);

		if (temp < minimum)		//if the randomly shuffled randVector has less distance between points, store it
		{
			minimum = temp;
			bestOrder = randVector;
			changedOrder = true;		//we did find a more optimized route
		}
	}

	if (changedOrder == true)		//if we found a more optimized route
	{
		deliveries = bestOrder;		//we will use it
		newCrowDistance = minimum;		//and get our new crow's distance based off it
	}

}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
	m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
	delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
	const GeoCoord& depot,
	vector<DeliveryRequest>& deliveries,
	double& oldCrowDistance,
	double& newCrowDistance) const
{
	return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}