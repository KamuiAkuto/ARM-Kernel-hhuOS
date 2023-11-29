#pragma once 
#include "streambuf.hpp"
#include "string.hpp"

#include "mutex.hpp"

namespace lib {
    template<class CharT>
    class basic_istream : public basic_streambuf<CharT> {
    
        private:
            basic_istream(const basic_istream &copy) = delete;

            mutex& lock;

            inline bool isWhiteSpace(CharT w) {
                return (w == ' ' || w == '\t' || w == '\r' || w == '\n' || w == 0x04);
            }

            void getUnitWhiteSpaceUnsafe(CharT* str, size_t len) {
                size_t i = 1;
                auto w = this->get();
                for(; isWhiteSpace(w) && i < len; 
                        w = this->get());
                str[0] = w;
                for(auto c = this->get(); 
                        !(isWhiteSpace(c) || i > len); 
                        ++i, c = this->get()) 
                    str[i] = c;
                str[i] = 0;
            }
        
            void getLineUnsafe(CharT* str, size_t len) {
                size_t idx = 0;
                for(auto c = this->get(); !(c == '\n' || c == 0x04 || len == 0); ++idx, --len, c = this->get()) 
                    str[idx] = c;
                str[idx] = 0;
            }

            unsigned long getNumber() {
                lib::lock_guard g{ lock };
                if(this->isEmpty()) readLine();
                auto size = this->getUsedSpace();
                CharT cstr[size];
                getUnitWhiteSpace(cstr, size);

                int i = 0;
                
                auto oldBase = base;
                if(size > 2 && cstr[i] == '0' && cstr[i+1] == 'x') {
                    base = 16;
                    i = 2;
                }

                unsigned long val = 0;

                for (int i = 0; cstr[i] != '\0'; ++i) {
                    if(base == 16) {
                        if(cstr[i] >= 'a') cstr[i] -= 'a' - 10;
                        else if(cstr[i] >= 'A') cstr[i] -= 'A' - 10;
                        else cstr[i] -= '0';
                    } else {
                        cstr[i] -= '0';
                    }
                    if (cstr[i]> 16 || cstr[i]<0) {
                        break;
                    }
                    val = val*base + cstr[i];
                }

                base = oldBase;
                return val;
            }

            long getSignedNumber() {
                lib::lock_guard g{ lock };
                if(this->isEmpty()) readLine();
                auto size = this->getUsedSpace();
                CharT cstr[size];
                getUnitWhiteSpaceUnsafe(cstr, size);


                int i = 0;
                bool negative = false;
                if(size > 0 && cstr[0] == '-') {
                    i = 1;
                    negative = true;
                }

                auto oldBase = base;
                if(size > i + 2 && cstr[i] == '0' && cstr[i+1] == 'x') {
                    base = 16;
                    i = i + 2;
                }

                long val = 0;

                for (; cstr[i] != '\0'; ++i) {
                    if(base == 16) {
                        if(cstr[i] >= 'a') cstr[i] -= 'a' - 10;
                        else if(cstr[i] >= 'A') cstr[i] -= 'A' - 10;
                        else cstr[i] -= '0';
                    } else {
                        cstr[i] -= '0';
                    }
                    if (cstr[i]> 16 || cstr[i]<0) {
                        break;
                    }
                    val = val*base + cstr[i];
                }
                if(negative) val *= -1;

                base = oldBase;
                return val;
            }
        
        public:
            int base;

            basic_istream (mutex& m) : basic_streambuf<CharT>(), base(10), lock(m) { } 

            virtual void flush () = 0;
            virtual void readLine () = 0;

            void getLine(CharT* str, size_t len) {
                lib::lock_guard g{ lock };
                for(auto c = this->get(); !(c == '\n' || c == 0x04 || len == 0); ++str, --len, c = this->get()) 
                    *str = c;
                *(++str) = 0;
            }

            string getLine() {
                lib::lock_guard g{ lock };
                readLine();
                auto size = this->getUsedSpace();
                CharT cstr[size];
                getLineUnsafe(cstr, size);
                return { cstr };
            }

            basic_istream& operator >> (CharT& c) {
                lib::lock_guard g{ lock };
                c = this->get();
                while(isWhiteSpace(c))
                {
                    c = this->get();
                }
                return *this;
            }

            basic_istream& operator >> (string& str) {
                lib::lock_guard g{ lock };
                if(this->isEmpty()) readLine();
                auto size = this->getUsedSpace();
                CharT cstr[size];
                getUnitWhiteSpaceUnsafe(cstr, size);
                str = { cstr };
                return *this;
            }

            basic_istream& operator >> (short& ival) {
                ival = getSignedNumber();
                return *this;
            }
            basic_istream& operator >> (unsigned short& ival) {
                ival = getNumber();
                return *this;
            }
            basic_istream& operator >> (int& ival) {
                ival = getSignedNumber();
                return *this;
            }
            basic_istream& operator >> (unsigned int& ival) {
                ival = getNumber();
                return *this;
            }
            basic_istream& operator >> (unsigned long& ival) {
                ival = getNumber();
                return *this;
            }
            basic_istream& operator >> (long& ival) {
                ival = getSignedNumber();
                return *this;
            }
            
    };

    
}
