// BumpAllocator
// Beschreibung:    Eine sehr einfache Heap-Verwaltung, welche freigegebenen
//                  Speicher nicht mehr nutzen kann.
// 
// Ursprünglicher Autor:          Michael Schoettner, HHU, 3.3.2022
// Modifizert

#pragma once

#include "addresses.hpp"
#include "src/kernel/svc.hpp"

//#include "src/devices/peripherals/uart.hpp"

namespace Allocator::BumpAllocator {

    //namespace out = peripherals::uart;

    // private
    namespace {
        unsigned char *next;
        int_t allocations;
    }

    inline void init () {
        next = (unsigned char*)heap_start;
        allocations = 0;
        
        notInitialized = false;
    }

    inline void dump_free_memory () {
        // sofort zurueck, falls noch nicht initialisiert
        if (notInitialized) return ;
        
        //out::print("Freier-Speicher\n\r");
        //out::print("   Heap-Start:   "); out::print((int_t*)heap_start); out::print("\n\r");
        //out::print(", Heap-End:  "); out::print(heap_end); out::print(")\n\r");
        //out::print("   next: "); out::print((int_t*)next); 
        //out::print(", allocations: "); out::print(allocations); 
        //out::print("\n\r");
    }

    inline void* alloc (unsigned int req_size) {
        auto ptr = next;
        
        //out::print("   mm_alloc: req_size="); out::print(req_size); out::print("B\n\r");
        
        // Noch nicht initialisiert?
        if (notInitialized) init();
        
        if ( ((int_t)next + req_size) > heap_end) {
            //out::print("Memory-Management: panic out of memory, CPU haltet.\n\r");
            SVC::halt ();
        }
        
        ptr = next;
        next += req_size;
        
        //out::print(", allocated="); out::print(req_size); out::print("B");
        //out::print("  returning addr="); out::print((int_t*)ptr);
        //out::print("\n\r");
        
        allocations++;

        return (void*) ptr;
    }

    inline void free (void *ptr) {
        //out::print("   mm_free: ptr= "); out::print((int_t*)ptr); out::print(", not supported\n\r");
    }

}
