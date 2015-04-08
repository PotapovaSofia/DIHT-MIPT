#include <iostream>
#include "hierarchical_mutex.h"

int main() {
    hierarchical_mutex mtx1(1);
    hierarchical_mutex mtx2(2);

    mtx2.lock();
    mtx1.lock();
    mtx1.unlock();
    mtx2.unlock();
/*
* ERROR: incorrect level value
    mtx1.lock();
    mtx2.lock();
    mtx2.unlock();
    mtx1.unlock();
*/
    return 0;
}