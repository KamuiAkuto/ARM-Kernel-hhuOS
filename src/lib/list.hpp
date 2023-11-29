#pragma once
#include "src/kernel/Allocator.hpp"

namespace lib {

    template <class T>
    struct listNode
    {
        T data;
        listNode* next;
    };

    template <class T>
    struct iteratorList {
        listNode<T>* pointer;
        T& operator*() const { return pointer->data; }
        T* operator->() { return &(pointer->data); }
        // Prefix increment
        iteratorList& operator++() { pointer = pointer->next; return *this; }  

        // Postfix increment
        iteratorList operator++(int) { iteratorList it = *this; ++(*this); return it;  }

        friend bool operator== (const iteratorList& a, const iteratorList& b) { return a.pointer == b.pointer; };
        friend bool operator!= (const iteratorList& a, const iteratorList& b) { return a.pointer != b.pointer; }; 
    };

    template <class T>
    class list
    {
    private:
        listNode<T>* head;
        listNode<T>* tail;
    public:
        
        using Iterator = iteratorList<T>;


        list() { 
            head = nullptr;
            tail = nullptr;
        }
       

        list& operator=(const list& other) {
            // Guard self assignment
            if (this == &other)
                return *this;
        
            for (auto curr = head; curr != nullptr; curr = curr->next) {
                push_back(curr->data);
            }
            
            return *this;
        }

        Iterator begin() {
            return Iterator { head };
        }
        Iterator end() {
            return Iterator { nullptr };
        }
        
        T* front() {
            if (head == nullptr)
                return nullptr;
            return &(head->data);
        }
        T* back() {
            if (tail == nullptr)
                return nullptr;
            return &(tail->data);
        }
        
        void deleteAll() {
            listNode<T>* toDel;
            for (listNode<T>* curr = head; curr != nullptr;) {
                toDel = curr;
                curr = curr->next;
                delete toDel;
            }
        }
       // ~list() { deleteAll(); } // causes undefined reference to `__dso_handle'

        int push_back(T&& data) {
             auto newNode = new listNode<T>();
            if (newNode == nullptr) {
                return -1;
            }
            newNode->data = data;
            newNode->next = nullptr;
            
            if (head == nullptr)
            {
                head = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
            return 0;
        }
        int push_back(T& data) {
            auto newNode = new listNode<T>();
            if (newNode == nullptr) {
                return -1;
            }
            newNode->data = data;
            newNode->next = nullptr;

            if (head == nullptr)
            {
                head = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
            return 0;
        }
        int push_front(T&& data) {
            auto newNode = new listNode<T>();
            if (newNode == nullptr) {
                return -1;
            }
            newNode->data = data;
            newNode->next = head;
            head = newNode;
            if (tail == nullptr) {
                tail = head;
            }
            return 0;
        }
        int push_front(T& data) {
            auto newNode = new listNode<T>();
            if (newNode == nullptr) {
                return -1;
            }
            newNode->data = data;
            newNode->next = head;
            head = newNode;
            if (tail == nullptr) {
                tail = head;
            }
            return 0;
        }

        size_t size() {
            size_t ret = 0;
            for (auto curr = head; curr != nullptr; curr = curr->next) {
                ret++;
            }
            return ret;
        }

        void pop_back() {
             if (tail == head) {
                pop_front();
                return;
            }
            auto curr = head;
            listNode<T>* prev = nullptr;
            for (; curr->next != nullptr; curr = curr->next) {
                prev = curr;
            }
            tail = prev;
            prev->next = nullptr;
            if(curr != nullptr)
                delete curr;
        }
        void pop_front() {
            if (head == nullptr)
                return;
            auto curr = head;
            // was added because if the list only had one element tail was not set to null
            // the if fixed it but do not know if there are other problems. i hate tail
            if(curr == tail) 
                tail = curr->next;
            head = curr->next;
            delete curr;
        }

        void erase(Iterator idx) {
            //change to iterator not tested!!!!!
            if (idx == begin()) {
                pop_front();
                return;
            }
            
            listNode<T>* prev = head;
            auto curr = head;
            for (auto it = begin(); (curr != nullptr) && (it != idx); curr = curr->next, ++it) {
                prev = curr;
            }
            if (curr == nullptr) {
                return;
            }
            if (curr == tail) {
                tail = prev;
            }
            prev->next = curr->next;
            delete curr;
        }

        //T& at(int idx);
        /*T* operator[](int idx) {
            auto curr = head;
            for (; (curr != nullptr) && (idx != 0); curr = curr->next, idx--) {

            }
            if (curr == nullptr) {
                return nullptr;
            }
            return &(curr->data);
        }*/

        size_t remove(const T& value) {
            size_t ret = 0; 
            for (auto i = begin(); i != end(); ++i) {
                if (*i == value) {
                    erase(i);
                    ++ret;
                }   
            }
            return ret;
        }

        template<class UnaryPredicate>size_t remove_if(UnaryPredicate p) {
            size_t ret = 0;
            for (auto i = begin(); i != end(); ++i) {
                if (p(*i)) {
                    erase(i);
                    ++ret;
                }
            }
            return ret;
        }
    };
   
}