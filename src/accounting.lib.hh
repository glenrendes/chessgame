#ifndef ACCT
#define ACCT

#include <iostream>

void *operator new(size_t size);
void operator delete(void* pt);
void accounting();

#endif
