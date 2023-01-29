//HW 1 - Templated Priority Queue - Trevor Dunn
#pragma once
#include <iostream>

using namespace std;

template <class IncomingClass>
class TPQ
{
public:
	IncomingClass* queue; //pointer to an array of IncomingClass objects
	int entries; //tracks the amount of objects in the queue
	int totalSize; //tracks the max size of the queue

	TPQ()
	{
		//Default Constructor
		cout << "Queue Default Constructor Called" << endl;
		queue = new IncomingClass[5];
		entries = 0;
		totalSize = 5;
	}

	~TPQ()
	{
		//Destructor
		cout << "Queue Destructor Called" << endl;
		delete[] queue;
		queue = nullptr;
		entries = 0;
		totalSize = 0;
	}

	TPQ(const TPQ& other)
	{
		//Queue Copy Constructor
		cout << "Queue Copy Constructor Called" << endl;

		entries = other.entries;
		totalSize = other.totalSize;

		queue = new IncomingClass[totalSize];

		IncomingClass* otherQueue = other.queue;

		for (int i = 0; i < entries; i++)
		{
			queue[i] = otherQueue[i];
		}

		otherQueue = nullptr;
	}

	TPQ& TPQ::operator=(const TPQ& other)
	{
		//Queue Copy Assignment
		cout << "Queue Copy Assignment Called" << endl;

		if (this == &other)
		{
			return *this;
		}

		if (queue != nullptr)
		{
			delete[] queue;
			queue = nullptr;
			entries = 0;
			totalSize = 0;
		}

		totalSize = other.totalSize;
		entries = other.entries;

		queue = new IncomingClass[totalSize];

		IncomingClass* otherQueue = other.queue;
		
		for (int i = 0; i < entries; i++)
		{
			queue[i] = otherQueue[i];
		}

		otherQueue = nullptr;

		return *this;
	}

	IncomingClass& operator *()
	{
		return *queue;
	}

	IncomingClass* operator ->()
	{
		return queue;
	}

	//Push Method
	void Push(IncomingClass newData)
	{
		//Checks if the queue is full, doubles the size of the queue
		if (entries == totalSize)
		{
			totalSize *= 2;
			IncomingClass* newQueue = new IncomingClass[totalSize];
			for (int i = 0; i < entries; i++)
			{
				newQueue[i] = queue[i];
			}
			delete[] queue;
			queue = newQueue;
			newQueue = nullptr;
		}

		//Adds the newData, increments the amount of entries, and calls the sort function
		queue[entries] = newData;
		entries++;
		this->Sort();

		return;
	}

	//Pop Method
	void Pop()
	{
		//Moves each object back one, overwritting the one in the front of the queue
		for (int i = 0; i < totalSize; i++)
		{
			queue[i] = queue[i + 1];
		}

		//Sets the back of the queue equal to NULL and then decrements the amount of entries
		queue[entries] = NULL;
		entries--;

		return;
	}

	//Print Method
	void Print()
	{
		cout << "Queue Contents: ";

		//If the queue is empty, prints out that there is no contents
		if (entries == 0)
		{
			cout << "NONE" << endl;
			return;
		}

		//Loops through the amount of entries and prints them with a comma afterwards except for the last entry which doesn't get a comma
		for (int i = 0; i < entries; i++)
		{
			cout << queue[i];

			if (i < entries - 1)
			{
				cout << ", ";
			}
		}

		cout << endl;

		return;
	}

	//GetSize Method - returns the amount of entries
	int GetSize()
	{
		return entries;
	}

	//IsEmpty Method - Returns true or false depending on if entries is 0 or not
	bool IsEmpty()
	{
		if (entries != 0) return false;
		return true;
	}

	//Sort Method - Orders the queue from smallest to largest
	void Sort()
	{
		IncomingClass swap; //variable used for swapping around the values
		for (int i = 0; i < entries; i++)
		{
			for (int j = i + 1; j < entries; j++)
			{
				if (queue[i] > queue[j])
				{
					swap = queue[j];
					queue[j] = queue[i];
					queue[i] = swap;
				}
			}
		}
	}
};