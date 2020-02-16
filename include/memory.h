#pragma once

#include "common.h"

// this is a dead simple memory management

size_t ascrGetAllocatedMemory();

void* ascrMalloc(size_t __size);
void ascrFree (void* __ptr);
