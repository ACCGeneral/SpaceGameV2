#pragma once

// all entities need a unique ID
class ID_manager
{
private:
	unsigned long long int ID;


public:
  ID_manager();
  unsigned long long int getID();


};