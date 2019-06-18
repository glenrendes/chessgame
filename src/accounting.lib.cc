#include "accounting.lib.hh"

int count = 0;
void *operator new(size_t size){
  //os << "Allocated memory.\n"; //also had constructors and destructors printing class identifiers at some point
  count++;
  void *pt = malloc(size);
  return pt;
}
void operator delete(void* pt){
  //os << "Deallocated memory.\n";
  count--;
  free(pt);
}
void accounting(){
  std::cout << "Outstanding memory allocations: " << count << ".\n";
}
