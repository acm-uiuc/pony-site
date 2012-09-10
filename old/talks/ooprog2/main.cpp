#include "LockingPtr.hpp"

volatile MyObj SharedObj;

int main()
{
   {
      LockingPtr<MyObj> lp(SharedObj, SharedObj.mutex);
      lp->foo();
   }
}

