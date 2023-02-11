#include "LinkedListCargo.h"
cargo* LinkedListCargo::Search(int ID, int& pos) const
{
	for (int i = 1;i <= itemCount;i++)
	{
		Node<cargo*>* nodePtr = getNodeAt(i);
		cargo* x = nodePtr->getItem();
		if (*x == ID)
		{
			pos = i;
			return x;
		}
	}
	return nullptr; //returns null if item not found
}
