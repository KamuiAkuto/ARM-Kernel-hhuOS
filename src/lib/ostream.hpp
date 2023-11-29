// basic_ostream
// Beschreibung:    Die Klasse OutStream enthaelt die Definition des
//                  << Operators fuer die wichtigsten der vordefinierten
//                  Datentypen und realisiert somit die bekannte Ausgabe-
//                  funktion der C++ iO_Stream Bibliothek. Zur Zeit wird
//                  die Darstellung von Zeichen, Zeichenketten und ganzen 
//                  Zahlen unterstuetzt. Ein weiterer << Operator erlaubt
//                  die Verwendung von Manipulatoren.
// 
//                  Neben der Klasse OutStream sind hier auch die
//                  Manipulatoren hex, dec, oct und bin fuer die Wahl der
//                  Basis bei der Zahlendarstellung, sowie endl fuer den
//                  Zeilenumbruch definiert.
// Autor:           Olaf Spinczyk, TU Dortmund
//                  Aenderungen von Michael Schoettner, HHU, 06.04.20
//                  Aenderungen von Markus Schäfer

#pragma once 
#include "streambuf.hpp"
#include "string.hpp"

#include "mutex.hpp"

namespace lib {
    template<class CharT>
    class basic_ostream : public basic_streambuf<CharT> {
    
        private:
            basic_ostream(const basic_ostream &copy) = delete; // Verhindere Kopieren

            mutex& lock;

        public:
            int base;   // Basis des Zahlensystems: z.B. 2, 8, 10 oder 16

            basic_ostream (mutex &lock) : basic_streambuf<CharT>(), base(10), lock(lock) { }   // initial Dezimalsystem

            virtual void flush () = 0;

            // OPERATOR << : Umwandlung des angegebenen Datentypes in eine
            //               Zeichenkette.

            // Darstellung eines Zeichens (trivial)
            basic_ostream& operator << (CharT c) {
                lib::lock_guard g{ lock };
                this->put(c);
                return *this;
            }
          /*basic_ostream& operator << (unsigned CharT c) {
                return *this << (char) c;
            }*/

            void writeEndlUnsafe() {
                this->put('\r');
                this->put('\n');
            }

            // Darstellung einer nullterminierten Zeichenkette
            basic_ostream& operator << (CharT* str) {
                lib::lock_guard g{ lock };
                for(auto pos = str; *pos; ++pos)
                    this->put(*pos);
                return *this;
            }

            basic_ostream& operator << (string str) {
                lib::lock_guard g{ lock };
                for(auto pos = str.c_str(); *pos; ++pos)
                    this->put(*pos);
                return *this;
            }

            // Darstellung einer nullterminierten Zeichenkette
            basic_ostream& operator << (const CharT* str) {
                lib::lock_guard g{ lock };
                for(auto pos = str; *pos; ++pos) 
                    this->put(*pos);
                return *this;
            }

            //  Darstellung ganzer Zahlen im Zahlensystem zur Basis base
            basic_ostream& operator << (short ival) {
                return *this << (long) ival;
            }
            basic_ostream& operator << (unsigned short ival) {
                return *this << (unsigned long) ival;
            }
            basic_ostream& operator << (int ival) {
                return *this << (long) ival;
            }
            basic_ostream& operator << (unsigned int ival) {
                return *this << (unsigned long) ival;
            }
            basic_ostream& operator << (long ival) {
                 // Bei negativen Werten wird ein Minuszeichen ausgegeben.
                if (ival < 0) {
                    this->put((CharT)'-');
                    ival = -ival;
                }
                // Dann wird der Absolutwert als vorzeichenlose Zahl ausgegeben.
                return *this << (unsigned long) ival;
            }
            basic_ostream& operator << (unsigned long ival) {
                lib::lock_guard g{ lock };
                unsigned long div;
                char digit;
            
                if (base == 8)
                    this->put ((CharT)'0');         // oktale Zahlen erhalten eine fuehrende Null
                else if (base == 16) {
                    this->put ((CharT)'0');         // hexadezimale Zahlen ein "0x"
                    this->put ((CharT)'x');
                }

                // Bestimmung der groessten Potenz der gewaehlten Zahlenbasis, die
                // noch kleiner als die darzustellende Zahl ist.
                for (div = 1; ival/div >= (unsigned long) base; div *= base);

                // ziffernweise Ausgabe der Zahl
                for (; div > 0; div /= (unsigned long) base) {
                    digit = ival / div;
                    if (digit < 10)
                            this->put ((CharT)'0' + digit);
                    else
                            this->put ((CharT)'a' + digit - 10);
                    ival %= div;
                }
                return *this;
            }
            
            // Darstellung eines Zeigers als hexadezimale ganze Zahl
            basic_ostream& operator << (void* ptr) {
                int oldbase = base;
                base = 16;
                *this << (unsigned long) ptr;
                base = oldbase;
                return *this;
            }

            // Aufruf einer Manipulatorfunktion
            basic_ostream& operator << (basic_ostream& (*f) (basic_ostream&)) {
                lib::lock_guard g{ lock };
                return f(*this);
            }
    };

    //
    // Manipulatorfunktionen
    //
    // Die folgenden Funktionen erhalten und liefern jeweils eine Referenz auf
    // ein OutStream Objekt. Da die Klasse OutStream einen Operator << fuer
    // derartige Funktionen definiert, koennen sie mit Hilfe dieses Operators
    // aufgerufen und sogar in weitere Eingaben eingebettet werden.
    // Aufgabe der Manipulatoren ist, die Darstellung der nachfolgenden Ausgaben
    // zu beeinflussen, z.B durch die Wahl des Zahlensystems.

    // Zeilenumbruch in Ausgabe einfuegen.
    template<class CharT>
    basic_ostream<CharT>& endl (basic_ostream<CharT>& os) {
        os.writeEndlUnsafe();
        os.flush ();
        return os;
    }

    // Waehle binaeres Zahlensystem aus.
    template<class CharT>
    basic_ostream<CharT>& bin (basic_ostream<CharT>& os) {
        os.base = 2;
        return os;
    }

    // Waehle oktales Zahlensystem aus.
    template<class CharT>
    basic_ostream<CharT>& oct (basic_ostream<CharT>& os) {
        os.base = 8;
        return os;
    }

    // Waehle dezimales Zahlensystem aus.
    template<class CharT>
    basic_ostream<CharT>& dec (basic_ostream<CharT>& os) {
        os.base = 10;
        return os;
    }

    // Waehle hexadezimales Zahlensystem aus.
    template<class CharT>
    basic_ostream<CharT>& hex (basic_ostream<CharT>& os) {
        os.base = 16;
        return os;
    }
}
