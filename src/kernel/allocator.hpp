// Allocator
// Beschreibung:    Einfache Speicherverwaltung. 'new' und 'delete' werden
//                  durch Ueberladen der entsprechenden Operatoren
//                  realisiert.
// Heap:
//                    0x400000:	Start-Adresse der Heap-Verwaltung
//                    0x4FFFFF: Ende: Letzte Adresse des phys. Speichers
// 
// Achtung:         Benötigt einen PC mit mindestens 5 MB RAM!
// 
// Ursprünglicher Autor:           Michael Schoettner, HHU, 1.3.2022
// Modifizert

#pragma once

#include "allocator/BumpAllocator.hpp"
#include "allocator/LinkedListAllocator.hpp"

//namespace allocator = Allocator::BumpAllocator;
namespace allocator = Allocator::LinkedListAllocator;

#include <stddef.h>

/*****************************************************************************
 * Nachfolgend sind die Operatoren von C++, die wir hier ueberschreiben      *
 * und entsprechend 'mm_alloc' und 'mm_free' aufrufen.                       *
 *****************************************************************************/
inline void* operator new ( size_t size ) {
     return allocator::alloc(size);
}

inline void* operator new[]( size_t count ) {
    return allocator::alloc(count);
}

inline void operator delete ( void* ptr )  {
    allocator::free(ptr);
}
 
inline void operator delete[] ( void* ptr ) {
    allocator::free(ptr);
}

inline void operator delete(void*ptr, unsigned long sz) {
    allocator::free(ptr);
}
