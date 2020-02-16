#include "memory.h"

#include <stdlib.h>
#include <stdio.h>
// record dynamic memory usage

static size_t memorySize = 0;

size_t ascrGetAllocatedMemory()
{
    return memorySize;
}

void* ascrMalloc(size_t __size)
{
    void* ptr = malloc(__size);

    if(ptr != NULL)
    {
        memorySize += __size;
    }
    else
    {
        fprintf(stderr, "ascrMalloc error for size of %zu \n", __size);
    }

    return ptr;
}

void ascrFree (void* __ptr)
{
    free(__ptr);
}