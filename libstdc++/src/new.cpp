#include "new"
#include <stdlib.h>

const std::nothrow_t std::nothrow = {};

void* operator new(std::size_t size)
{
    void* p = malloc(size);
    if (p == NULL) {
        abort();
    }
    return p;
}

void* operator new[](std::size_t size)
{
    void* p = malloc(size);
    if (p == NULL) {
        abort();
    }
    return p;
}

void  operator delete(void* ptr)
{
    free(ptr);
}

void  operator delete[](void* ptr)
{
    free(ptr);
}

void* operator new(std::size_t size, const std::nothrow_t&)
{
    return malloc(size);
}

void* operator new[](std::size_t size, const std::nothrow_t&)
{
    return malloc(size);
}

void  operator delete(void* ptr, const std::nothrow_t&)
{
    free(ptr);
}

void  operator delete[](void* ptr, const std::nothrow_t&)
{
    free(ptr);
}





