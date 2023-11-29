#pragma once

#include "iterator.hpp"

namespace lib {

    template<class CharT>
    class basic_string {
        private:
            using Iterator = iterator<CharT>;
            CharT* cstr;
            size_t len;
        public:
            basic_string() {
                cstr = nullptr;
                len = 0;
            }

            basic_string(CharT (&str)[]) {
                size_t strSize = 0;
                for (; str[strSize] != '\0'; ++strSize);
                
                cstr = new CharT[strSize + 1];
                for (size_t i = 0; i < strSize; ++i)
                    cstr[i] = str[i];
                cstr[strSize] = '\0';
                len = strSize + 1;
            }

            basic_string(const CharT str[]) {
                size_t strSize = 0;
                for (; str[strSize] != '\0'; ++strSize);
                
                cstr = new CharT[strSize + 1];
                for (size_t i = 0; i < strSize; ++i)
                    cstr[i] = str[i];
                cstr[strSize] = '\0';
                len = strSize + 1;
            }

            basic_string(basic_string& str) {
                cstr = new CharT[str.length()];
                auto p = cstr;
                for(auto &x : str) {
                    *p = x;
                    ++p;
                }
                len = str.length();
            }

            Iterator begin() const {
                return Iterator { cstr };
            }
            Iterator end() const {
                return Iterator { (&cstr[len]) + 1 };
            }

            bool isEmpty() { return cstr == nullptr; }
            const CharT* c_str() { return cstr; }

            size_t length() const {
                return len;
            }

            basic_string& operator=(const basic_string& other)
            {
                if (this == &other)
                    return *this;

                auto temp = new CharT[other.length];
                delete[] cstr;
                cstr = temp;
                this->len = other.length();

                auto p = cstr;
                for(auto &x : other) {
                    *p = x;
                    ++p;
                }
                
                return *this;
            }

            CharT& operator[](std::size_t idx)       { return cstr[idx]; }
            const CharT& operator[](std::size_t idx) const { return cstr[idx]; }
            

            inline bool operator==(const basic_string& rhs) { 
                if(this->length() != rhs.length()) {
                    return false;
                }

                for (size_t i = 0; i < this->length(); i++)
                    if(this->cstr[i] != rhs.cstr[i])
                        return false;
                
                return true;

            }
            inline bool operator!=(const basic_string& rhs) { return !(this == rhs); }

    };

    template<class CharT>
    basic_string<CharT> operator+(const basic_string<CharT>& lhs, const basic_string<CharT>& rhs) {
        auto length = lhs.length() + rhs.length();
        CharT str[length];
        int i = 0;
        for(auto c : lhs) {
            if(c != 0) {
                str[i] = c;
                ++i;
            }
        }
        for(auto c: rhs) {
            str[i] = c;
            ++i;
        }
        return { str };
    }

    template< class CharT>
    basic_string<CharT> operator+ ( const basic_string<CharT>& lhs, const CharT* rhs ) {
        basic_string<CharT> str { rhs };
        return lhs + str;
    }

    template<class CharT, class Traits, class Alloc>
    basic_string<CharT> operator+ ( const basic_string<CharT>& lhs, CharT rhs ) {
        auto length = lhs.length + 1;
        CharT str[length];
        int i = 0;
        for(auto c : lhs) {
            if(c != 0) {
                str[i] = c;
                ++i;
            }
        }
        str[i] = rhs;
        ++i;
        str[i] = 0;
        return { str };
    }
    
    template< class CharT>
    basic_string<CharT> operator+ ( const CharT* lhs, const basic_string<CharT>& rhs ) {
        basic_string<CharT> str { lhs };
        return str + rhs;
    }
    
    template< class CharT>
    basic_string<CharT> operator+ ( CharT lhs, const basic_string<CharT>& rhs ) {
        auto length = rhs.length + 1;
        CharT str[length];
        int i = 0;
        str[i] = lhs;
        ++i;
        for(auto c : rhs) {
            str[i] = c;
            ++i;
        }
        return { str };
    }
    
    template< class CharT>
    basic_string<CharT> operator+ ( basic_string<CharT>&& lhs, basic_string<CharT>&& rhs ) {
        auto length = lhs.length + rhs.length;
        CharT str[length];
        int i = 0;
        for(auto c : lhs) {
            if(c != 0) {
                str[i] = c;
                ++i;
            }
        }
        for(auto c: rhs) {
            str[i] = c;
            ++i;
        }
        return { str };
    }

    template< class CharT>
    basic_string<CharT> operator+( basic_string<CharT>&& lhs, const basic_string<CharT>& rhs ) {
        auto length = lhs.length + rhs.length;
        CharT str[length];
        int i = 0;
        for(auto c : lhs) {
            if(c != 0) {
                str[i] = c;
                ++i;
            }
        }
        for(auto c: rhs) {
            str[i] = c;
            ++i;
        }
        return { str };
    }

    template< class CharT>
    basic_string<CharT> operator+( basic_string<CharT>&& lhs, const CharT* rhs ) {
        basic_string<CharT> str { rhs };
        return lhs + str;
    }

    template< class CharT>
    basic_string<CharT> operator+( basic_string<CharT>&& lhs, CharT rhs ) {
        auto length = lhs.length + 1;
        CharT str[length];
        int i = 0;
        for(auto c : lhs) {
            if(c != 0) {
                str[i] = c;
                ++i;
            }
        }
        str[i] = rhs;
        ++i;
        str[i] = 0;
        return { str };
    }

    template< class CharT>
    basic_string<CharT> operator+( const basic_string<CharT>& lhs, basic_string<CharT>&& rhs ) {
        auto length = lhs.length + rhs.length;
        CharT str[length];
        int i = 0;
        for(auto c : lhs) {
            if(c != 0) {
                str[i] = c;
                ++i;
            }
        }
        for(auto c: rhs) {
            str[i] = c;
            ++i;
        }
        return { str };
    }

    template< class CharT>
    basic_string<CharT> operator+( const CharT* lhs, basic_string<CharT>&& rhs ) {
        basic_string<CharT> str { lhs };
        return str + rhs;
    }

    template< class CharT>
    basic_string<CharT> operator+( CharT lhs, basic_string<CharT>&& rhs ) {
        auto length = rhs.length + 1;
        CharT str[length];
        int i = 0;
        str[i] = lhs;
        ++i;
        for(auto c : rhs) {
            str[i] = c;
            ++i;
        }
        return { str };
    }



    typedef basic_string<char> string;
}