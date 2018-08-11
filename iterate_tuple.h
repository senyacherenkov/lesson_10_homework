#pragma once
#include <tuple>

/*Honestly stolen from http://artlang.net/post/c++11-obkhod-elementov-kortezhe-std-tuple/
 *But I totaly understand how it works
 */
namespace hw_utility
{
    template<int index, typename Callback, typename... Args>
    struct iterate_tuple
    {
        static void next(std::tuple<Args...>& t, Callback callback)
        {
            // Decrement of position and call the same method recursively
            iterate_tuple<index - 1, Callback, Args...>::next(t, callback);

            // Call of callback with position and value of element
            callback(index, std::get<index>(t));
        }
    };

    // Partial specialization for zero index
    template<typename Callback, typename... Args>
    struct iterate_tuple<0, Callback, Args...>
    {
        static void next(std::tuple<Args...>& t, Callback callback)
        {
            callback(0, std::get<0>(t));
        }
    };

    // Partial specialization for index -1 (empty tuple)
    template<typename Callback, typename... Args>
    struct iterate_tuple<-1, Callback, Args...>
    {
        static void next(std::tuple<Args...>& t, Callback callback)
        {
            // do nothing
        }
    };
}

//
// "Tricky" for_each for cycle through elements of tuple (compile time!):
//
template<typename Callback, typename... Args>
void for_each(std::tuple<Args...>& t, Callback callback)
{
    // tuple size
    int const t_size = std::tuple_size<std::tuple<Args...>>::value;

    // Recursive cycle through tuple's elements in compile time
    hw_utility::iterate_tuple<t_size - 1, Callback, Args...>::next(t, callback);
}
