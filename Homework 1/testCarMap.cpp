
#include "CarMap.h"

#include <string>
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    CarMap m;
    assert(m.addCar("Fred"));
    assert(m.addCar("Ethel"));
    assert(m.addGas("Fred", 10));
    assert(m.fleetSize() == 2);
    assert(!m.addGas("Lucy", 10));
    assert(m.gas("Fred") == 10);
    
}

int main()
{
    test();
    cout << " I passed all tests" << endl;
}

