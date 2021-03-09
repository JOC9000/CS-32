// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.
#include <vector>

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;

	void associate(const KeyType& key, const ValueType& value);

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
	int m_bucketAmt = 8;	//stores amount of buckets that currently exist
	int m_used = 0;		//stores amount of buckets currently in use
	double m_load = .5;		//stores maximum load of hash map, default is .5

	struct Bucket	//used as buckets, stores key, value, and whether in use
	{
		KeyType curKey;
		ValueType curVal;
		bool used = false;
	};

	std::vector<Bucket> buckets;	//container will be vector, to accomadate fact that amount of buckets may need to be doubled
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
	:m_load(maximumLoadFactor)
{
	buckets.resize(8);	//default bucket size is 8
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
	reset();	//upon deletion, map set to default
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
	buckets.resize(8);
	m_bucketAmt = 8;
	m_used = 0;

	for (unsigned int i = 0; i < buckets.size(); i++)	//opens up defaulted map for use
		buckets[i].used = false;

}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
	return m_used;	
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	unsigned int hasher(const KeyType& k);	//only hasher used through project is GeoCoord provided

	unsigned int bucket = hasher(key);
	bucket = bucket % m_bucketAmt;

	ValueType* editVal = find(key);

	if (editVal != nullptr)	//if the GeoCoord passed in is found, change its value
	{
		*editVal = value;
		return;
	}

	m_used++;

	if ((double)m_used / m_bucketAmt > m_load)	//if current load exceeds max load, rehash vector to larger one
	{
		int temp = m_bucketAmt;
		m_bucketAmt *= 2;
		std::vector<Bucket> newVect(m_bucketAmt);

		for (int position = 0; position < temp; position++)
		{
			if (buckets[position].used == true)	//for every bucket in use, rehash to new vector
			{
				unsigned int curBuck = hasher(buckets[position].curKey);
				curBuck = curBuck % m_bucketAmt;

				for (int tries = 0; tries < m_bucketAmt; tries++)	//repeat until GeoCoord map is put into an available bucket
				{
					if (newVect[curBuck].used == false)
					{
						newVect[curBuck].curKey = buckets[position].curKey;
						newVect[curBuck].curVal = buckets[position].curVal;
						newVect[curBuck].used = true;
						break;
					}

					curBuck = (curBuck + 1) % m_bucketAmt;
				}
			}
		}

		buckets = newVect;
	}


	bucket = hasher(key);
	bucket = bucket % m_bucketAmt;

	for (int tries = 0; tries < m_bucketAmt; tries++)	//add new value-key pair to map, where there is space
	{
		if (buckets[bucket].used == false)
		{
			buckets[bucket].curKey = key;
			buckets[bucket].curVal = value;
			buckets[bucket].used = true;
			return;
		}

		bucket = (bucket + 1) % m_bucketAmt;
	}
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
	unsigned int hasher(const KeyType & k);
	unsigned int curBuck = hasher(key);
	curBuck = curBuck % m_bucketAmt;

	for (int tries = 0; tries < m_bucketAmt; tries++)
	{
		if (buckets[curBuck].used == false)	//if location of hash does not contain, not here
			return nullptr;

		if (buckets[curBuck].curKey == key)	//returns pointer to value if found
			return &buckets[curBuck].curVal;

		curBuck = (curBuck + 1) % m_bucketAmt;	//continue through allbcukets after hash in loop
	}

	return nullptr; //return null if kay not found through a full map
}
