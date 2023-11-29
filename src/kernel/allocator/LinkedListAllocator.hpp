// LinkedListAllocator
// Beschreibung:    Einfache Speicherverwaltung, welche den freien Speicher
//                  mithilfe einer einfach verketteten Liste verwaltet.
//
// Ursprünglicher  Autor:           Michael Schoettner, HHU, 13.6.2020
// Modifizert

#pragma once

#include "src/kernel/svc.hpp"
#include "addresses.hpp"

//#include "src/devices/peripherals/uart.hpp"

#define HEAP_MIN_FREE_BLOCK_SIZE 64         // min. Groesse eines freien Blocks

namespace Allocator::LinkedListAllocator {


    //namespace out = peripherals::uart;
    //use out::print("Something\n\r"); instead of cout

    // Format eines freien Blocks
    typedef struct free_block {
        int_t size;
        struct free_block *next;
    } free_block;

    // private
    namespace
    {
        // freie Bloecke werden verkettet
        free_block *free_start;
    }
    
    inline void init () {
        // Anker auf Dummy-Element setzen
        free_start = (free_block*) heap_start;
        free_start->size = sizeof(free_block);
        free_start->next = (free_block*)(heap_start + sizeof(free_block));
        
        // 2. Element, umfasst kompletten Freispeicher
        free_start->next->size = heap_size - sizeof(free_block);
        free_start->next->next = 0;
        
        notInitialized = false;
    }    

    inline void dump_free_memory () {
        auto ptr  = free_start;
        
        // sofort zurueck, falls noch nicht initialisiert
        if (notInitialized) return ;
        
        //out::print("Freispeicherliste (mit Dummy-Element\n\r)");
        //out::print("   Heap-Start:   "); out::print(heap_start);
        //out::print(", Heap-End:  "); out::print(heap_end); out::print("\n\r");

        while ( ptr!=0 ) {
            //out::print("   Block-Start:  "); out::print((int_t*)ptr);
            //out::print(", Block-End: "); out::print( ((int_t*)ptr + (ptr->size -1)) );
            //out::print(", Block-Groesse: "); out::print( ptr->size); out::print("\n\r");
            ptr = ptr->next;
        }
        //out::print("   * Ende der Liste *\n\r");
    }

    inline void* alloc (int_t req_size) {
        free_block *ptr;
        free_block *prev;
        int_t found;
            
        //out::print("   mm_alloc: req_size="); out::print(req_size); out::print("B\n\r");
            
        // Noch nicht initialisiert?
        if (notInitialized) init();
        
        // Jetzt sind die Werte sicher intitialisiert.
        ptr = free_start->next;
        prev = free_start;
            
        // Damit delete spaeter funktioniert, muessen wir die Laenge
        // im belegten Block mit abspeichern. Dies erfolgt am Anfang.
        req_size += sizeof(int_t);

        // first-fit Strategie
        while ( ptr!=0 ) {

            // found
            if ( ptr->size >= req_size )
                break;
            
            ptr = ptr->next;
            prev = prev->next;
        }

        // Nicht genuegend Speicher vorhanden -> CPU anhalten
        if ( (ptr == 0) || (ptr->size < req_size) ) {
            //out::print("Memory-Management: panic out of memory, CPU haltet.\n\r");
            //out::print("Trying to allocate: "); out::print(req_size); out::print(" Bytes\n\r");
            SVC::halt();
        }
            
        // Ist der gefundene Block so gross, sodass Abschneiden sinnvoll ist?
        if (ptr->size > (req_size + HEAP_MIN_FREE_BLOCK_SIZE)) {
            ptr->size -= req_size;
            found = (int_t)ptr;
            found += ptr->size;
            
        } else {
            // Gefundener Block komplett zuteilen
            // Muss aus der Liste ausgetragen werden
            prev->next = ptr->next;
            found = (int_t)ptr;
        }

        // Laenge als erste 4 Byte
        auto lp = (int_t*)found;
        *lp = req_size;
        
        //out::print(", allocated="); out::print(req_size); out::print("B");
        //out::print("  returning addr="); out::print((int_t*)(found+sizeof(int_t)));
        //out::print("\n\r");

        // Rueckgabewert: Zeiger danach
        return (void*) (found+sizeof(int_t));
    }

    inline void free (void *ptr) {
        //out::print("   mm_free: ptr= "); out::print(ptr); out::print("\n\r");

        // Zeiger korrigieren, auf Laengenfeld
        auto p = (int_t*)ptr;
        p--;

        // Am Kopf der Freispeicherliste einfuegen
        auto block = (free_block *)p;
        block->next = free_start->next;
        free_start->next = block;
    }

}
