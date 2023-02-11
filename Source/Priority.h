#pragma once
#include "NodePriority.h"
template <typename T>
class PriorityQueue 
{
private:
	int itemCount=0;
	NodePriority<T>* backPtr;
	NodePriority<T>* frontPtr;
public:
	PriorityQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry, float priority);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	int getCount();
	//void operator =(const LinkedQueue<T>& Q); //overloading assingment operator
	~PriorityQueue();

	//copy constructor
	//PriorityQueue(const LinkedQueue<T>& LQ);
};
template <typename T>
PriorityQueue<T>::PriorityQueue()
{
	backPtr = nullptr;
	frontPtr = nullptr;

}
template<typename T>
inline bool PriorityQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}
template <typename T>
bool PriorityQueue<T>::enqueue(const T& newEntry, float priority)
{
	NodePriority<T>* temp = new NodePriority<T>(newEntry, priority);
	NodePriority<T>* ptr;
	if (temp == nullptr)
		return false;
	// Insert the new node
	if (frontPtr == nullptr || (temp->getKey() < frontPtr->getKey()))
	{
		temp->setNext(frontPtr);
		frontPtr = temp;
	}
	else
	{
		ptr = frontPtr;
		while (ptr->getNext() != nullptr && ptr->getNext()->getKey() <= temp->getKey())
			ptr = ptr->getNext();

		temp->setNext(ptr->getNext());
		ptr->setNext(temp);
	}
	itemCount++;
	return true;

} // end enqueue
template <typename T>
bool PriorityQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	NodePriority<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		backPtr = nullptr;

	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;
	itemCount--;
	return true;

}
template <typename T>
bool PriorityQueue<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
template<typename T>
int PriorityQueue<T>::getCount()
{
	return itemCount;
}
template <typename T>
PriorityQueue<T>::~PriorityQueue()
{
	T temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));
}