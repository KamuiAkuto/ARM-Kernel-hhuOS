#pragma once
#include <stdint.h>

extern volatile uintptr_t _heap_start;
#define HEAP_START  ((int64_t)&_heap_start)
extern volatile uintptr_t _heap_end;
#define HEAP_END  ((int64_t)&_heap_end)
extern volatile uintptr_t _heap_size;
#define HEAP_SIZE  ((int64_t)&_heap_size)


namespace Allocator {
    typedef uint64_t int_t;

    namespace {
        int_t heap_start = HEAP_START;
        int_t heap_end = HEAP_END;
        // doing HEAP_END - HEAP_START did give the wrong value
        // unsure why and unsure when it would be set
        int_t heap_size = HEAP_SIZE;

        bool notInitialized = true;
    }
}