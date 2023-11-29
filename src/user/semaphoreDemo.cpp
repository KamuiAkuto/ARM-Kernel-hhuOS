#pragma GCC push_options
#pragma GCC optimize ("O0")

#include "src/lib/iostream.hpp"
#include "src/lib/thread.hpp"


#include "src/lib/semaphore.hpp"

#include "src/lib/ringBuffer.hpp"

extern volatile uintptr_t  _consumer_stack_top;
#define CONSUMER_STACK_ADD  (int64_t)& _consumer_stack_top
extern volatile uintptr_t  _consumer_start;
#define CONSUMER_BEGIN_ADD  (int64_t)& _consumer_start

extern volatile uintptr_t  _producer_stack_top;
#define PRODUCER_STACK_ADD  (int64_t)& _producer_stack_top
extern volatile uintptr_t  _producer_start;
#define PRODUCER_BEGIN_ADD  (int64_t)& _producer_start


namespace semaphoreDemo {

    using lib::Semaphore;
    using lib::mutex;

    using lib::cout;
    using lib::cin;
    using lib::endl;

    mutex lock;
    Semaphore read { 0 };
    Semaphore write { 5 };

    lib::ringBuffer<int> buffer;

    //extern "C"
    int __attribute__((section(".boot.semaphoreDemo"))) main() {

        cout << "Hello semaphoreDemo" << endl;
        
        auto idP = SVC::createThread(PRODUCER_BEGIN_ADD, PRODUCER_STACK_ADD);
        auto idC = SVC::createThread(CONSUMER_BEGIN_ADD, CONSUMER_STACK_ADD);
        
        lib::this_thread::join(idP);
        lib::this_thread::join(idC);
        
        return 0x10;
    }

}

#pragma GCC pop_options