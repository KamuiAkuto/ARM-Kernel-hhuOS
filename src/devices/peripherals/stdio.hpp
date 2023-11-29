#pragma once
#include "src/lib/ostream.hpp"
#include "src/lib/istream.hpp"
#include "src/lib/mutex.hpp"

#include "uart.hpp"

namespace peripherals::stdio {

    lib::mutex outLock;
    lib::mutex inLock;
    
    class stdout : public lib::basic_ostream<char> {
           
        private:
            stdout(stdout &copy) = delete; // Verhindere Kopieren

        public:
            stdout (lib::mutex& lock) : lib::basic_ostream<char>(lock) { 
                uart::init();
                flush(); 
            }

            virtual void flush() {
                uart::print(buffer, writePos);
                writePos = 0;
            }

            virtual void readLine() { 

            }
    };

    class stdin : public lib::basic_istream<char> {
           
        private:
            stdin(stdout &copy) = delete; // Verhindere Kopieren     

        public:
            stdin (lib::mutex &lock) : lib::basic_istream<char>(lock) { 
                uart::init();
            }

            virtual void flush() {
            
            }

            virtual void readLine() {
                for(auto c = uart::getChar(); !(c == '\r' || c == 0x04 || isFull()); c = uart::getChar()) {
                    put(c);
                }
                if(!isFull()) { 
                    put('\n');
                    uart::putchar('\n');
                }
            }
    };

    
}