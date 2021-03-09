#include "Map.h"
#include <iostream>

Map::Map()
{
	head = nullptr;	//initialize a Map with no nodes
}

Map::Map(const Map& old) //copy constructor
{
	if (old.head == nullptr)	//if old is empty, make new Map empty as well
		head = nullptr;

	else //create a Node for new Map if old is not empty
	{
		Node* p = new Node;
		Node* n = old.head;
		head = p;
		head->key = (old.head)->key;
		head->value = (old.head)->value;
		head->prev = nullptr;

		while (n->next != nullptr)	//go through and copy over all values of Nodes from old into new Nodes corresponding to the new Map
		{
			p->next = new Node;
			p->next->prev = p;
			p = p->next;
			n = n->next;
			p->value = n->value;
			p->key = n->key;
		}

				//set last Node to point to null
			p->next = nullptr;
	}

}

Map::~Map()
{
	if (head != nullptr)	//if Map is empty do nothing, otherwise..
	{
		Node* p = head;

		while (p->next != nullptr)	//goo through and delete all previoius Nodes up to the last one then...
		{
			p = p->next;
			delete p->prev;
		}

		delete p;	//delete the last Node
	}
}

Map& Map::operator=(const Map& src)
{
	if (&src == this)	//check if both Maps are the same. If so don nothin
		return *this;

	if (head != nullptr)	//destroy all dynamically allocated memory
	{
		Node* p = head;

		while (p->next != nullptr)
		{
			p = p->next;
			delete p->prev;
		}

		delete p;
	}

	if (src.head == nullptr)	//check to see if linked list is empty, if so leave Map empty
		head = nullptr;

	else //start to copy over everything from src to new allocated memory
	{
		Node* p = new Node;
		Node* n = src.head;
		head = p;
		head->value = (src.head)->value;
		head->key = (src.head)->key;
		head->prev = nullptr;

		while (n->next != nullptr)
		{
			p->next = new Node;
			p->next->prev = p;
			p = p->next;
			n = n->next;
			p->value = n->value;
			p->key = n->key;
		}

			p->next = nullptr;	//set last next pointer to null
	}


	return *this;
}

bool Map::empty() const
{
	if (head == nullptr)	//check if Map is empty
		return true;

	return false;
}

int Map::size() const
{
	if (head == nullptr)
		return 0;

	int i = 1;	//if Map has a head, there is at least 1 element
	Node* p = head;

	while (p->next != nullptr)	//increment i as many times as there is Nodes, to get amount of Nodes
	{
		p = p->next;
		i++;
	}

	return i;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	if (head == nullptr)	//if empty, make head point to a new Node
	{
		head = new Node;
		head->next = nullptr;
		head->prev = nullptr;
		head->value = value;
		head->key = key;
		return true;
	}

	Node* p = head;

	while (p->next != nullptr)	//check if key exists in list already
	{
		if (p->key == key)
			return false;

		p = p->next;
	}

	if (p->key != key)	//if key does not exist check last node, then proceed to insert new Node at end of list
	{
		p->next = new Node;
		p->next->prev = p;
		p = p->next;
		p->value = value;
		p->key = key;
		p->next = nullptr;
		return true;
	}

	return false;

}

bool Map::update(const KeyType& key, const ValueType& value)
{
	if (head == nullptr)	//if list is empty, there is nothing to update
		return false;

	if (head->key == key)	//if head is to be modified, modify head's value
	{
		head->value = value;
		return true;
	}

	Node* p = head->next;	//if not proceed to next value

	while (p != nullptr)	//check all keys in list, and if a match, update it
	{
		if (p->key == key)
		{
			p->value = value;
			return true;
		}

		p = p->next;
	}

	return false;
}

void Map::dump() const	//print out information of Map
{
	std::cerr << size() << std::endl;
	
	Node* p = head;

	while (p != nullptr)
	{
		std::cerr << p->key << ": " << p->value << std::endl;
		p = p->next;
	}
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	if (head == nullptr)	//if list is empty, insert a new Node
	{
		head = new Node;
		head->next = nullptr;
		head->prev = nullptr;
		head->value = value;
		head->key = key;
		return true;
	}

	Node* p = head;

	while (p->next != nullptr)	//go through and update a Node if it's key matches
	{
		if (p->key == key)
		{
			p->value = value;
			return true;
		}

		p = p->next;
	}

	if (p->key != key)	//otherwise create a new Node at end of list
	{
		p->next = new Node;
		p->next->prev = p;
		p = p->next;
		p->value = value;
		p->key = key;
		p->next = nullptr;
		return true;
	}

	else     //but if the last key matches, update it's value
	{
		p->value = value;
		return true;
	}

}

bool Map::erase(const KeyType& key)
{
	if (head == nullptr)	//if no list, nothing to erase
		return false;

	if (head->key == key)	//if first item in list is to be erased, erase it with ease. Then create/update new head pointer
	{
		if (head->next != nullptr)
		{
			Node* p = head;
			head = head->next;
			delete p;
			head->prev = nullptr;
			return true;
		}

		delete head;
		head = nullptr;
		return true;
	}

	Node* p = head->next;

	while (p != nullptr)	//otherwise go through and update the list accordingly
	{
		if (p->key == key)
		{
			if (p->next != nullptr)
			{
				p->next->prev = p->prev;
				p->prev->next = p->next;
			}

			else     //specific for if last element in list to be erased
				p->prev->next = nullptr;

			delete p;
			return true;
		}
		p = p->next;
	}

	return false;
}

bool Map::contains(const KeyType& key) const
{
	if (head == nullptr)	//if list is empty, no matches will be found
		return false;

	Node* p = head;

	while (p != nullptr)	//otherwise go through and check
	{
		if (p->key == key)
			return true;

		p = p->next;
	}

	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	if (head == nullptr)	//if list is empty, no match will be found
		return false;

	Node* p = head;

	while (p != nullptr)	//otherwise, check
	{
		if (p->key == key)
		{
			value = p->value;
			return true;
		}

		p = p->next;
	}

	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i < 0 || i >= size() || head == nullptr)	//check if given i works. If so, proceed
		return false;

	Node* p = head;

	for (int l = 0; l < i; l++)//move forward in list i times and return those values
		p = p->next;

	key = p->key;
	value = p->value;

	return true;
}

void Map::swap(Map& other)
{
	Map* p = &other;
	Node* temp = head;	//change pointers of heads to accomplish the same as a swap
	head = p->head;
	p->head = temp;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
	bool check = true;
	KeyType keycheck1, keycheck2;
	ValueType valcheck1, valcheck2;
	Map lol;

	for (int i = 0; i < m1.size(); i++)	//go through each key in m1 to check if it is repeated in m2
	{
		m1.get(i, keycheck1, valcheck1);

		for (int l = 0; l < m2.size(); l++)
		{
			m2.get(l, keycheck2, valcheck2);
			if (keycheck1 == keycheck2 && valcheck1 != valcheck2)	//if so, check if they share the same value or not, and act accordingly
			{
				check = false;
				break;
			}

			if (keycheck1 == keycheck2 && valcheck1 == valcheck2)
			{
				lol.insert(keycheck1, valcheck1);
				break;
			}
		}

		if (keycheck1 != keycheck2)	//if same key is not found in m2, insert key and value of element in m1 to result
			lol.insert(keycheck1, valcheck1);
	}

	for (int i = 0; i < m2.size(); i++)	//do corresponding actions in reverse. Do so for m2 checking againse all elements in m1
	{
		m2.get(i, keycheck2, valcheck2);

		for (int l = 0; l < m1.size(); l++)
		{
			m1.get(l, keycheck1, valcheck1);
			if (keycheck1 == keycheck2 && valcheck1 != valcheck2)
			{
				check = false;
				break;
			}

			if (keycheck1 == keycheck2 && valcheck1 == valcheck2)
			{
				lol.insert(keycheck1, valcheck1);
				break;
			}
		}

		if (keycheck1 != keycheck2)	//no repeats will occur as insert prevents this from happening
			lol.insert(keycheck2, valcheck2);
	}

	result = lol;
	return check;
}

void reassign(const Map& m, Map& result)
{
	result = m;	//make result the same size as m, and share all the same keys for easy modification
	Map l = m;	//in case m is the same as result

	if (m.size() == 0 || m.size() == 1)
		return;

	KeyType tempkey, changekey;
	ValueType tempval;

	for (int i = 0; i < m.size(); i++)
	{
		if (i == m.size() - 1)	//when at last element, give the first element the value of the last element
		{
			m.get(0, tempkey, tempval);
			changekey = tempkey;
			m.get(i, tempkey, tempval);
			l.update(changekey, tempval);
		}

		else     //when not at last element of list, give next Node the value of the current Node
		{
			m.get(i + 1, tempkey, tempval);
			changekey = tempkey;
			m.get(i, tempkey, tempval);
			l.update(changekey, tempval);
		}
	}

	result = l;
}
