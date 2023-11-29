#pragma GCC push_options
#pragma GCC optimize ("O0")

#include "src/lib/iostream.hpp"
#include "src/lib/thread.hpp"

namespace counterEndless {

   
    using lib::cout;
    using lib::cin;
    using lib::endl;

    //extern "C"
    int __attribute__((section(".boot.counterEndless"))) main() {

        cout << "Hello Counter Endless" << endl;

        int counter = 0;
        

        while (1)
        {
            cout << '\r' << counter;
            cout.flush();
            counter++;
            lib::this_thread::delay(1);
        }
        
        return -1;
    }

}

#pragma GCC pop_options