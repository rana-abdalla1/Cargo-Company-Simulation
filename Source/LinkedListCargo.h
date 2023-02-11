#pragma once
#include "cargo.h"
#include "NodeNormal.h"
#include "LinkedList.h"
//Specilization of cargo* of class LinkedList to implement function Search that is specific to cargo* type only.
class LinkedListCargo : public LinkedList<cargo*>
{

public:
	cargo* Search(int ID, int& pos) const;
};
