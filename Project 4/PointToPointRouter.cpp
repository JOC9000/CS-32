#include "provided.h"
#include "ExpandableHashMap.h"
#include <list>
#include <queue>
#include <set>
using namespace std;

class PointToPointRouterImpl
{
public:
	PointToPointRouterImpl(const StreetMap* sm);
	~PointToPointRouterImpl();
	DeliveryResult generatePointToPointRoute(
		const GeoCoord& start,
		const GeoCoord& end,
		list<StreetSegment>& route,
		double& totalDistanceTravelled) const;

private:
	const StreetMap* map;

	struct geoDist
	{
		geoDist(GeoCoord init, double distToTarget, double distUpTo, list<StreetSegment> passedList)
		{
			a = init;
			h = distToTarget;
			g = distUpTo;
			f = distUpTo + distToTarget;
			wayHere = passedList;
		}

		bool operator <(const geoDist& rhs) const
		{
			if (f < rhs.f)
				return true;
			return false;
		}

		double f, g, h;
		GeoCoord a;
		list<StreetSegment> wayHere;
	};

};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
	map = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
	const GeoCoord& start,
	const GeoCoord& end,
	list<StreetSegment>& route,
	double& totalDistanceTravelled) const
{
	
	vector<StreetSegment> toCheck;
	if (map->getSegmentsThatStartWith(start, toCheck) == false || map->getSegmentsThatStartWith(end, toCheck) == false)
		return BAD_COORD;	//return immediately if start or end coords are not in map

	route.clear();
	totalDistanceTravelled = 0;
	set<geoDist> geoSet;//open "list"
	ExpandableHashMap<GeoCoord, double> closed;// closed "list"
	geoSet.insert(geoDist(start, distanceEarthMiles(start, end), 0, route));// add starting node to open
	geoDist curGeo(start, 0, 0, route);

	for (;;)
	{
		vector<StreetSegment> getAdjGeo;

		if (geoSet.size() == 0)
			return NO_ROUTE;

		curGeo = *(geoSet.begin());//gives lowest f cost coord

		geoSet.erase(curGeo);//remove from open list
		closed.associate(curGeo.a, curGeo.f);//associate the coord with its total cost

		if (curGeo.a == end)//we are at the end, edit later
		{
			totalDistanceTravelled = curGeo.f;
			route = curGeo.wayHere;
			return DELIVERY_SUCCESS;
			break;
		}

		getAdjGeo.clear();
		map->getSegmentsThatStartWith(curGeo.a, getAdjGeo);

		for (unsigned int i = 0; i < getAdjGeo.size(); i++)//for each point next to curGeo
		{
			if (closed.find(getAdjGeo[i].end) != nullptr)//if found on closed, skip
				continue;


			double distCurNew = distanceEarthMiles(curGeo.a, getAdjGeo[i].end);
			GeoCoord newPlace = getAdjGeo[i].end;
			double distNewEnd = distanceEarthMiles(getAdjGeo[i].end, end);

			for (set<geoDist>::iterator it = geoSet.begin(); it != geoSet.end(); it++)//iterate through to check if on open list
			{
				if ((*it).a == getAdjGeo[i].end)
				{
					if ((curGeo.g + distCurNew < (*it).g))
					{
						geoSet.erase(it);
						curGeo.wayHere.push_back(getAdjGeo[i]);
						geoDist newGeo(newPlace, distNewEnd, curGeo.g + distCurNew, curGeo.wayHere);
						curGeo.wayHere.remove(getAdjGeo[i]);
						geoSet.insert(newGeo);
					}

					break;
				}

			}
			curGeo.wayHere.push_back(getAdjGeo[i]);
			geoSet.insert(geoDist(getAdjGeo[i].end, distNewEnd, curGeo.g + distCurNew, curGeo.wayHere));
			curGeo.wayHere.remove(getAdjGeo[i]);
		}


	}

	return NO_ROUTE;

}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
	m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
	delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
	const GeoCoord& start,
	const GeoCoord& end,
	list<StreetSegment>& route,
	double& totalDistanceTravelled) const
{
	return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}