#include "src/lib/iostream.hpp"

#include "src/devices/timer.hpp"

#include "src/lib/thread.hpp"

/*extern volatile uintptr_t  _<name>_stack_top;
#define PROCESS_1_STACK_ADD  (int64_t)& _<name>_stack_top
extern volatile uintptr_t  _<name>_start;
#define PROCESS_1_BEGIN_ADD  (int64_t)& _<name>_start
*/

extern volatile uintptr_t  _semaphoreDemo_stack_top;
#define SEMAPHORE_DEMO_STACK_ADD  (int64_t)& _semaphoreDemo_stack_top
extern volatile uintptr_t  _semaphoreDemo_start;
#define SEMAPHORE_DEMO_ADD  (int64_t)& _semaphoreDemo_start

extern volatile uintptr_t  _counter_stack_top;
#define COUNTER_STACK_ADD  (int64_t)& _counter_stack_top
extern volatile uintptr_t  _counter_start;
#define COUNTER_BEGIN_ADD  (int64_t)& _counter_start

extern volatile uintptr_t  _counterEndless_stack_top;
#define COUNTER_ENDLESS_STACK_ADD  (int64_t)& _counterEndless_stack_top
extern volatile uintptr_t  _counterEndless_start;
#define COUNTER_ENDLESS_BEGIN_ADD  (int64_t)& _counterEndless_start


namespace console {

    using namespace lib;

   // extern "C"
    int __attribute__((section(".boot.console"))) main() {
        cout << "Hello Console" << endl;

        uint64_t counterEndlessId = 0;

        while (1)
        {
            auto input = cin.getLine();

            cout << input << endl;

            if(input == "start counter") {
                auto id = SVC::createThread(COUNTER_BEGIN_ADD, COUNTER_STACK_ADD);
                id = 2;
                int64_t ret = 0;
                auto retId = lib::this_thread::join(id, ret);
                cout << "Return Value: " << ret << endl;

            } else if (input == "start counterEndless") {
                cout <<"\"" <<  counterEndlessId <<"\""<< endl;
                if(counterEndlessId == 0)
                    counterEndlessId = SVC::createThread(COUNTER_ENDLESS_BEGIN_ADD, COUNTER_ENDLESS_STACK_ADD);
                else 
                    cout << "Already Started" << endl;

            } else if (input == "stop counterEndless") {
                cout <<"\"" <<  counterEndlessId <<"\""<< endl;
                if(counterEndlessId != 0) {
                    this_thread::stop(counterEndlessId);
                    counterEndlessId = 0;
                }
                

            } else if ((input == "start sema") || (input == "start semaphore")) {
                auto id = SVC::createThread(SEMAPHORE_DEMO_ADD, SEMAPHORE_DEMO_STACK_ADD);
                int64_t ret = 0;
                auto retId = lib::this_thread::join(id, ret);
                cout << "Return Value: " << ret << endl;

            } else if (input == "yield") {
                cout << "How many times?" << endl;
                int number = 0;
                cin >> number;

                for(; number < 0; number--)
                    this_thread::yield();

            }
        }
    }
}
