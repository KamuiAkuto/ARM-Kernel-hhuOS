// basic_streambuf
// Beschreibung:    Die Klasse StringBuffer stellt einen Puffer fuer die
//                  Sammlung von Zeichen zur Darstellung auf dem Bildschirm
//                  oder anderen Ausgabegeraeten bereit. Die Ausgabe der
//                  Zeichen erfolgt, sobald der Puffer voll ist oder wenn
//                  explizit die Methode flush() aufgerufen wird.
//                  Da StringBuffer geraeteunabhaengig sein soll, ist
//                  flush() eine virtuelle Methode, die von den abgeleiteten
//                  Klassen definiert werden muss.
//
// Autor:           Olaf Spinczyk, TU Dortmund
//                  Aenderungen von Michael Schoettner, HHU, 06.04.20
//                  Aenderungen von Markus Schäfer

#pragma once 
#include <cstddef>

#define STREAMBUFFER_SIZE    80

namespace lib {
    template<class CharT>
    class basic_streambuf {
            
        private:
            basic_streambuf(const basic_streambuf &copy) = delete; // Verhindere Kopieren

        protected:
            CharT buffer[STREAMBUFFER_SIZE];
            size_t readPos;
            size_t writePos;

            // StringBuffer: Im Konstruktor wird der Puffer als leer markiert.
            basic_streambuf () : readPos(0), writePos(0) {}

            virtual void flush() = 0;
            virtual void readLine() = 0;

            inline bool isEmpty() { return writePos == readPos; }
            inline bool isFull() { return ((writePos + 1) % STREAMBUFFER_SIZE) == readPos; }

            inline size_t getUsedSpace() { 
                return (writePos + STREAMBUFFER_SIZE - readPos - 1) % STREAMBUFFER_SIZE + 1;
            }

            bool put(CharT c) {
                if (isFull()) flush();

                if (isFull()) return false;

                buffer[writePos] = c;
                writePos = (writePos + 1) % STREAMBUFFER_SIZE;
                return true;
            }

            CharT get() {
                if (isEmpty()) readLine();

                if(isEmpty()) return 0;

                auto c = buffer[readPos];
                readPos = (readPos + 1) % STREAMBUFFER_SIZE;
                return c;
            }
    };
}