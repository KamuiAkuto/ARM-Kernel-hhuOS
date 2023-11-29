#pragma once

#include "streambuf.hpp"

namespace lib {
    template<class T>
    class ringBuffer : basic_streambuf<T> {

        void flush() {}
        void readLine() {}

        public:

            T read() {
                return this->get();
            }

            void write(T value) {
                this->put(value);
            }
    };
}