#pragma once

// find implantation from https://en.cppreference.com/w/cpp/algorithm/find
// remove implantation from https://en.cppreference.com/w/cpp/algorithm/remove

namespace lib
{
    // find
    template<class InputIt, class T>
    constexpr InputIt find(InputIt first, InputIt last, const T& value)
    {
        for (; first != last; ++first)
            if (*first == value)
                return first;
    
        return last;
    } 

    template<class InputIt, class UnaryPredicate>
    constexpr InputIt find_if(InputIt first, InputIt last, UnaryPredicate p)
    {
        for (; first != last; ++first)
            if (p(*first))
                return first;
    
        return last;
    }

    template<class InputIt, class UnaryPredicate>
    constexpr InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate q)
    {
        for (; first != last; ++first)
            if (!q(*first))
                return first;
    
        return last;
    }
    
    /*
    //remove
    template<class ForwardIt, class T>
    ForwardIt remove(ForwardIt first, ForwardIt last, const T& value)
    {
        first = find(first, last, value);
        if (first != last)
            for (ForwardIt i = first; ++i != last;)
                if (!(*i == value))
                    *first++ = std::move(*i);
        return first;
    }

    template<class ForwardIt, class UnaryPredicate>
    ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p)
    {
        first = find_if(first, last, p);
        if (first != last)
            for (ForwardIt i = first; ++i != last;)
                if (!p(*i))
                    *first++ = std::move(*i);
        return first;
    }
    */
} 
