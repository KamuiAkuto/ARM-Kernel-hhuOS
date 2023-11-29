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

namespace semaphoreDemo::producer {


    using lib::cout;
    using lib::cin;
    using lib::endl;

    //extern "C"
    int __attribute__((section(".boot.producer"))) main() {

        cout << "Hello producer" << endl;
        int counter = 0;
        while (1) {
            write.acquire();
            lock.lock();

            buffer.write(counter);
            cout << "Produced: " << counter << endl;
            counter++;

            lock.unlock();
            read.release();
        }
        
        return 0x10;
    }

}

#pragma GCC pop_options