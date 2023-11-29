#pragma once
#include "ostream.hpp"
#include "istream.hpp"

namespace lib
{
    typedef basic_ostream<char> ostream;
    typedef basic_istream<char> istream;

    extern ostream &cout;
    extern istream &cin;
}



