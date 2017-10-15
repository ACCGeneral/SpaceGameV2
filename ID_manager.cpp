#include "ID_manager.h"


ID_manager::ID_manager() // set id number to 0
{
  ID = 0;

}


unsigned long long int ID_manager::getID() //return current id num and then add 1 for new id
{
  unsigned long long int temp = ID;
  ID++;
  return temp;

}