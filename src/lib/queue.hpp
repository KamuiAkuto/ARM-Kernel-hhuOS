#pragma once
#include "list.hpp"

namespace lib
{
    template <class T>
    class queue {
        public:

            list<T> container;
            
            queue() {}
            
            //~queue() {}
            
            T* front() {
                return container.front();
            }

            T* back() {
                return container.back();
            }

            bool empty() {
                return (container.size() == 0);
            }

            size_t size() {
                return container.size();
            }

            int push(T&& data) {
                return container.push_back(data);
            }

            int push(T& data) {
                return container.push_back(data);
            }

            void pop() {
                container.pop_front();
            }

            queue& operator=(const queue& other)
            {
                // Guard self assignment
                if (this == &other)
                    return *this;
            
                this->container = other.container;
                return *this;
            }

            //is not included in the std::queue but i need it
            size_t remove(const T& value) { 
                return container.remove(value); 
            }
            

    };
}
