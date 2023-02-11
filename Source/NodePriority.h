#pragma once

//#ifndef _NODEPRIORITY
//#define _NODEPRIORITY

template < typename T>
class NodePriority
{
private:
	T item; // A data item
	NodePriority<T>* next; // Pointer to next node
	float priority;
	
public:
	NodePriority();
	NodePriority(const T& r_Item);
	NodePriority(const T& r_Item, float key);
	NodePriority(const T& r_Item, NodePriority<T>* nextNodePtr);
	void setItem(const T& r_Item);
	void setNext(NodePriority<T>* nextNodePtr);
	T getItem() const;
	float getKey() const;
	NodePriority<T>* getNext() const;
}; // end Node
//#endif

template < typename T>
NodePriority<T>::NodePriority()
{
	next = nullptr;
}

template < typename T>
NodePriority<T>::NodePriority(const T& r_Item)
{
	item = r_Item;
	next = nullptr;
}
template<typename T>
NodePriority<T>::NodePriority(const T& r_Item, float key)
{
	item = r_Item;
	priority = key;
}


template < typename T>
NodePriority<T>::NodePriority(const T& r_Item, NodePriority<T>* nextNodePtr)
{
	item = r_Item;
	next = nextNodePtr;
}
template < typename T>
void NodePriority<T>::setItem(const T& r_Item)
{
	item = r_Item;
}

template < typename T>
void NodePriority<T>::setNext(NodePriority<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template < typename T>
T NodePriority<T>::getItem() const
{
	return item;
}
template<typename T>
float NodePriority<T>::getKey() const
{
	return priority;
}

template < typename T>
NodePriority<T>* NodePriority<T>::getNext() const
{
	return next;
}