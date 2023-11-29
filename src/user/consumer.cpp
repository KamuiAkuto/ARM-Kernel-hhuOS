#pragma GCC push_options
#pragma GCC optimize ("O0")

#include "src/lib/iostream.hpp"

#include "src/lib/semaphore.hpp"

#include "src/lib/ringBuffer.hpp"

namespace semaphoreDemo {
    extern lib::Semaphore write;
    extern lib::Semaphore read;
    extern lib::mutex lock;

    extern lib::ringBuffer<int> buffer;
}

namespace semaphoreDemo::consumer {

    using lib::cout;
    using lib::cin;
    using lib::endl;

    //extern "C"
    int __attribute__((section(".boot.consumer"))) main() {

        cout << "Hello consumer" << endl;
        while (1) {
            read.acquire();
            lock.lock();

            auto ele = buffer.read();
            cout << "Consumes:" << ele << endl;

            lock.unlock();
            write.release();
        }
        
        return 0x10;
    }

}

#pragma GCC pop_options