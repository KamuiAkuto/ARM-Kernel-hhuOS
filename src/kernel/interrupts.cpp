#include "src/devices/peripherals/stdio.hpp"
#include "src/kernel/svc.hpp"

#include "Allocator.hpp"
#include "os.hpp"

#include "src/lib/thread.hpp"

#include "src/lib/semaphore.hpp"

namespace lib {
    peripherals::stdio::stdout stdcout { peripherals::stdio::outLock };
    lib::basic_ostream<char> &cout = stdcout;

    peripherals::stdio::stdin stdcin { peripherals::stdio::inLock };
    lib::basic_istream<char> &cin = stdcin;
}


extern "C" int svc_handler (int64_t w0, int64_t w1, int64_t w2, int64_t w3, int64_t w4, int64_t svcNumber)  {

    switch (svcNumber)
    {
    case startOs_NUM:
        kernel::start();
        return 0;
    case exit_NUM:
        kernel::exitThread(w0);
        return 0;
    case halt_NUM:
        kernel::halt();
        return 0;


    case waitingOnLock_NUM:
        kernel::waitingOnLock();
        return 0;
    case signalUnlock_NUM:
        kernel::signalUnlock();
        return 0;


    case schedulerBlock_NUM:
        kernel::schedular::block();
        return 0;
    case schedulerDeblock_NUM:
        kernel::schedulerDeblock(w0);
        return 0;


    case createThread_NUM:
        return kernel::createThread(w0, w1);
    case waitOnThread_NUM:
        return kernel::waitOnThread(w0);
    case getReturn_NUM:
        return kernel::getReturn(w0, (int64_t *)w1);
    case yield_NUM:
        kernel::schedular::yield();
        return 0;
    case killThread_NUM:
        return kernel::killThread(w0);
    case createThreadNoStack_NUM:
        return kernel::createThread(w0);


    default:
        /* put stuff here so the debugger can see the code
        lib::cin.getLine();
        lib::Semaphore s {5};
        s.acquire();
        s.release();
        */
        lib::this_thread::delay(1); //delay breaks if it is not position in the kernel space
                                    //i do not know why and i do not have the time to find out
        break;
    }
    return 0;
}

extern "C" void irg_handler ()  {
    auto intid = peripherals::GIC::getCurrentINTID();
    
    switch (intid)
    {
    case TIMER_IRQ:
        kernel::timerInterrupt();
        break;
    
    default:
        break;
    }
   
    
    peripherals::GIC::writeEol(intid);
}

extern "C" void __cxa_pure_virtual()
{
    lib::cout << "Error Pure Virtual was called" << lib::endl;
    while(1);
}
