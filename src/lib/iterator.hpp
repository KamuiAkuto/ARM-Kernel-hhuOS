#pragma once

namespace lib {
    template<class T>
    struct iterator {
        T* pointer;
        T& operator*() const { return *pointer; }
        T* operator->() { return &(*pointer); }
        // Prefix increment
        iterator& operator++() { ++pointer; return *this; }  

        // Postfix increment
        iterator operator++(int) { iterator it = *this; ++(*this); return it;  }

        friend bool operator== (const iterator& a, const iterator& b) { return a.pointer == b.pointer; };
        friend bool operator!= (const iterator& a, const iterator& b) { return a.pointer != b.pointer; }; 
    };
}