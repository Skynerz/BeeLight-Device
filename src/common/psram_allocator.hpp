#ifndef PSRAM_ALLOCATOR_HPP
#define PSRAM_ALLOCATOR_HPP

#include <stddef.h>

#ifdef BOARD_HAS_PSRAM
#include "esp32-hal.h"
#endif

class PSRAMAllocator {
public:
#ifdef BOARD_HAS_PSRAM
    void* operator new(size_t size) {
        return ps_malloc(size);
    }

    void operator delete(void* ptr) {
        heap_caps_free(ptr);
    }
#endif
};

#endif  // PSRAM_ALLOCATOR_HPP