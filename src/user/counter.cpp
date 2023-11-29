#pragma GCC push_options
#pragma GCC optimize ("O0")

#include "src/lib/iostream.hpp"
#include "src/lib/thread.hpp"

namespace counter {

   
    using lib::cout;
    using lib::cin;
    using lib::endl;

    //extern "C"
    int __attribute__((section(".boot.counter"))) main() {

        cout << "Hello Counter" << endl;

        int counter = 5;
        

        for ( ; counter > 0; counter--)
        {
            cout << '\r' << 'C' << counter;
            cout.flush();
            lib::this_thread::delay(1);
        }
        cout << endl;
        return 1234567890;
    }

}

#pragma GCC pop_options