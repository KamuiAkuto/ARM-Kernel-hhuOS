#pragma once

#include "src/kernel/svc.hpp"

#include "queue.hpp"
#include "mutex.hpp"

#include "thread.hpp"

namespace lib {
    class Semaphore {

        private:
            Semaphore (const Semaphore &copy); // Verhindere Kopieren
            
            queue<uint64_t> threadIds;
            mutex lock;

            int counter;

        public:
            Semaphore (int c) : counter (c) {
               // counter = c;
            }

            // 'Passieren': Warten auf das Freiwerden eines kritischen Abschnitts.
            inline void acquire() {
                lock_guard guard { lock };

                // Ressource vorhanden -> belegen und zurueckkehren
                if (counter > 0) {
                    counter--;
                    return;
                } else {  // Keine Ressourcen mehr vorhanden -> Aufrufer blockieren

                    // Zugriff auf Scheduler gegenueber PIT synchronisieren
                    // cpu.disable_int();
                    
                    auto id = lib::this_thread::get_id();
                    threadIds.push(id);          // Aufrufer in waitQueue einfuegen
                    lock.unlock();
                    
                    // Aufrufer blockieren und auf naechsten Thread umschalten
                    SVC::schedulerBlock();

                    // cpu.enable_int();
                }
            }

            // 'Vreigeben': Freigeben des kritischen Abschnitts.
            inline void release() {
                lock_guard guard { lock };

                // Wartet ein Thread auf die Freigabe der Semaphore?
                if (!threadIds.empty()) {
                    // Zugriff auf Scheduler gegenueber PIT synchronisieren
                    //cpu.disable_int();

                    auto next = *(threadIds.front()); 
                    threadIds.pop();

                    SVC::schedulerDeblock(next);

                    //cpu.enable_int();
                } else {   // Falls keiner wartet, counter erhoehen und zurueckkehren
                    counter++;
                }
            }
    };
}