#include "Main.h"
#include "TemplatedPriorityQueue.h"
template <typename T> 
void Print(std::deque<T> a_queue)
{
	std::cout << "Begining of queue......." << std::endl;
	uint count = a_queue.size();
	for (uint i = 0; i < count; ++i)
	{
		std::cout << a_queue[i];
		std::cout << std::endl;
	}
	std::cout << "End of queue............" << std::endl;
}

int main(void)
{
	/*
	std::deque<int> myQueue;
	//Push elements
	myQueue.push_back(0);
	std::cout << "	just pushed +++++" << std::endl;
	myQueue.push_back(1);
	std::cout << "	just pushed +++++" << std::endl;
	myQueue.push_back(2);
	std::cout << "	just pushed +++++" << std::endl;
	//Print queue
	Print<int>(myQueue);
	//pop an element
	myQueue.pop_front();
	std::cout << "		just popped -----" << std::endl;
	//add elements
	myQueue.push_back(3);
	std::cout << "	just pushed +++++" << std::endl;
	myQueue.push_back(4);
	std::cout << "	just pushed +++++" << std::endl;
	myQueue.push_back(5);
	std::cout << "	just pushed +++++" << std::endl;
	//Print queue
	Print<int>(myQueue);
	myQueue.pop_front();
	std::cout << "		just popped -----" << std::endl;
	myQueue.pop_front();
	std::cout << "		just popped -----" << std::endl;
	//Print queue
	Print<int>(myQueue);
	//clear queue
	uint count = myQueue.size();
	for (uint i = 0; i < count; ++i)
	{
		myQueue.pop_front();
		std::cout << "		just popped -----" << std::endl;
	}
	//Print queue
	Print<int>(myQueue);

	//Complex Data usage
	std::deque<Foo> myFooQueue;
	for (uint i = 0; i < 10; ++i)
	{
		Foo foo(i);
		myFooQueue.push_back(foo);
		std::cout << "	just pushed +++++" << std::endl;
		if (i % 2 == 0)
		{
			myFooQueue.pop_front();
			std::cout << "		just popped -----" << std::endl;
		}
	}

	std::deque<AlbertoClass> myAlbertoQueue;
	for (uint i = 30; i < 40; ++i)
	{
		AlbertoClass alberto(i);
		myAlbertoQueue.push_back(alberto);
		std::cout << "	just pushed +++++" << std::endl;
	}
	std::cout << "	Content" << std::endl;
	for (uint i = 0; i < 10; ++i)
	{
		std::cout << myAlbertoQueue[i] << std::endl;
	}

	std::cout << "Press Enter to finish." << std::endl;


	
	getchar();
	*/

	std::cout << "--------TESTING DEFAULT CONSTRUCTOR, PUSH, SORT, POP, AND PRINT--------" << std::endl;
	TPQ<int>* intQ1ptr = new TPQ<int>();
	intQ1ptr->Push(1);
	intQ1ptr->Push(2);
	intQ1ptr->Push(3);
	intQ1ptr->Push(4);
	intQ1ptr->Push(5);
	intQ1ptr->Push(6);
	intQ1ptr->Pop();
	intQ1ptr->Print();

	std::cout << std::endl;
	
	std::cout << "--------TESTING COPY ASSIGNMENT OPERATOR--------" << std::endl;
	TPQ<int>* intQ2ptr = new TPQ<int>();
	intQ2ptr->Push(2);
	intQ2ptr->Print();
	*intQ1ptr = *intQ2ptr;
	intQ1ptr->Print();

	std::cout << std::endl;

	std::cout << "--------TESTING COPY CONSTRUCTOR AND ISEMPTY--------" << std::endl;
	TPQ<int>* intQ3ptr = new TPQ<int>(*intQ1ptr);
	std::cout << "Is Queue 3 Empty? ";
	if (intQ3ptr->IsEmpty())
		std::cout << "YES" << std::endl;
	else
		std::cout << "NO" << std::endl;
	intQ3ptr->Print();

	std::cout << std::endl;

	std::cout << "--------TESTING POP, GETSIZE, AND ISEMPTY--------" << std::endl;
	intQ3ptr->Pop();
	std::cout << "Queue 3 Length: " << intQ3ptr->GetSize() << std::endl;
	std::cout << "Is Queue 3 Empty? ";
	if (intQ3ptr->IsEmpty())
		std::cout << "YES" << std::endl;
	else
		std::cout << "NO" << std::endl;
	intQ3ptr->Print();

	std::cout << std::endl;

	std::cout << "--------TESTING DESTRUCTOR--------" << std::endl;
	delete intQ1ptr;
	intQ1ptr = nullptr;

	delete intQ2ptr;
	intQ2ptr = nullptr;

	delete intQ3ptr;
	intQ3ptr = nullptr;

	_CrtDumpMemoryLeaks();

	getchar();
	return 0;
}