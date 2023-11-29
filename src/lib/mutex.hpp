#pragma once

#include <cstdint>

// svc codes see svc.hpp

// code is based on a modified version from
// https://developer.arm.com/documentation/dht0008/a/arm-synchronization-primitives/practical-uses/implementing-a-mutex
// resources for the monitored access
// https://developer.arm.com/documentation/den0024/a/Multi-core-processors/Multi-processing-systems/Synchronization

namespace lib {
    class mutex {
        private:
            uint64_t value = false;

        public:
            inline void lock() {
                __asm __volatile__(
                    R"(     mov     x1, #0x01       // write constant 1 to x1 as lock value
                        1:   
                            ldxr    x2, [  %[value] ]
                            cmp     x2, x1          // Test if mutex is locked or unlocked
                                beq     2f          // If locked - wait for it to be released, from 2
                            stxr    w2, x1, [ %[value] ]  // Not locked, attempt to lock it
                            cmn     w2, #0x01       // Check if Store-Exclusive failed
                                beq     1b          // Failed - retry from 1
                            // unsure if ish is correct https://developer.arm.com/documentation/100941/0101/Barriers
                            dmb ish                 // Required before accessing protected resource
                            b       3f
                        2:  // Take appropriate action while waiting for mutex to become unlocked
                            //svc     0x10            // signal the kernel that this threads is waiting on lock
                            b       1b              // Retry from 1
                        3:  // end
                            CLREX
                    )"
                    : /*no out*/
                    : [value] "r" (&value)
                    : "x1", "x2");
            }

            inline bool try_lock() {
                int ret = 0;
                __asm __volatile__(
                    R"(     mov     x1, #0x01       // write constant 1 to x1 as lock value
                            mov     %[result], #0x01    // write true result, if lock not possible will be change to false  
                        1:   
                            ldxr    x2, [  %[value] ]
                            cmp     x2, x1          // Test if mutex is locked or unlocked
                                beq     2f          // If locked return 0
                            stxr    w2, x1, [ %[value] ]  // Not locked, attempt to lock it
                            cmn     w2, #0x01       // Check if Store-Exclusive failed
                                beq     1b          // Failed - retry from 1
                            // unsure if ish is correct https://developer.arm.com/documentation/100941/0101/Barriers
                            dmb ish                 // Required before accessing protected resource
                            b       3f
                        2:  // Take appropriate action while waiting for mutex to become unlocked
                            mov     %[result], #0x00 // lock failure return value to false
                        3:  // end
                    )"
                    : [result] "=&r" (ret)
                    : [value] "r" (&value)
                    : "x1", "x2");
                    return ret;
            }

            inline void unlock() {
                __asm __volatile__(
                    R"(
                        mov     x1, #0x00           // write constant 0 to x1 as unlock value
                        // unsure if ish is correct https://developer.arm.com/documentation/100941/0101/Barriers
                        dmb     ish                 // Required before releasing protected resource
                        str     x1, [ %[value] ]    // Unlock mutex normal str is fine here
                        //svc 0x11
                    )"
                    : /*no out*/
                    : [value] "r" (&value)
                    : "x1");
            }
    };

    class lock_guard {
        private:
            mutex& lock;
        public:
            inline lock_guard(mutex & m) : lock(m) {
                lock.lock();
            }

            lock_guard( const lock_guard& ) = delete;

            inline ~lock_guard() {
                lock.unlock();
            }
    };
}