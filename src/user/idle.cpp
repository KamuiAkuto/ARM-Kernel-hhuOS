#pragma GCC push_options
#pragma GCC optimize ("O0")

#include "src/lib/iostream.hpp"

namespace idle {

    using lib::cout;
    using lib::endl;

    int __attribute__((section(".boot.idle"))) main() {
        cout << "Idle start" << endl;
        while(1) {
            
        }
    }

}

#pragma GCC pop_options