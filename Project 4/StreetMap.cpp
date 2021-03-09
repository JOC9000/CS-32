#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
	return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
	StreetMapImpl();
	~StreetMapImpl();
	bool load(string mapFile);
	bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;

private:
	ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_HashMap;	//holds all street segments

	void loadSegToHash(StreetSegment toload)//prevents repeat code in load
	{
		vector<StreetSegment> editVect;
		vector<StreetSegment>* ptrToExistingSegVect = m_HashMap.find(toload.start);

		if (ptrToExistingSegVect != nullptr)
			editVect = *ptrToExistingSegVect;
		editVect.push_back(toload);
		m_HashMap.associate(toload.start, editVect);
	}
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
	ifstream inf(mapFile);	//uncomment this
	
	// Test for failure to open
	if (!inf)
		return false;

	string line;
	while (getline(inf, line))
	{
		int linesToTraverse;

		inf >> linesToTraverse;
		inf.ignore(10000, '\n');

		for (int i = 0; i < linesToTraverse; i++)
		{
			string temp1, temp2, temp3, temp4;
			inf >> temp1; //beginx
			inf >> temp2;	//beginy
			inf >> temp3;	//endx
			inf >> temp4;	//endy

			GeoCoord start(temp1, temp2);//new
			GeoCoord end(temp3, temp4);//new
			StreetSegment a(start, end, line);//
			StreetSegment aReverse(end, start, line);//new

			loadSegToHash(a);
			loadSegToHash(aReverse);

			inf.ignore(10000, '\n');
		}
	}

	return true;  // Delete this line and implement this function correctly
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
	const vector<StreetSegment>* temp1;	//stores pointer given by find function
	temp1 = m_HashMap.find(gc);

	if (temp1 != nullptr)
	{
		segs.clear();
		segs = *temp1;
		return true;
	}

	return false;  // Delete this line and implement this function correctly
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
	m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
	delete m_impl;
}

bool StreetMap::load(string mapFile)
{
	return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
	return m_impl->getSegmentsThatStartWith(gc, segs);
}