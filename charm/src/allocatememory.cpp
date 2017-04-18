#include <stddef.h>

#include "system.h"
 
void *operator new(size_t size)
{
    return System::heap.allocate(size);
}
 
void *operator new[](size_t size)
{
    return System::heap.allocate(size);
}
 
void operator delete(void *p)
{
    System::heap.free(p);
}
 
void operator delete[](void *p)
{
    System::heap.free(p);
}
