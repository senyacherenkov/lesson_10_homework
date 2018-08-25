#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <list>
#include <utility>
#include <tuple>
#include <type_traits>
#include "iterate_tuple.h"

namespace  {
    //check stl-container type
    template <typename T, typename = void>
    struct is_container : std::false_type {};

    template <typename T>
    struct is_container<T
       , std::void_t<decltype(std::declval<T>().clear())
          , decltype(std::declval<T>().size())>> : std::true_type {};

    //check homogeneity of types in tuple
    template<typename Type>
    constexpr bool parse() {
        return true;
    }

    template<typename Type, typename FirstType, typename... Args>
    constexpr bool parse() {
        return std::is_same<Type, FirstType>::value && parse<Type, Args...>();
    }

    template<typename... Args>
    void checkHomogeneity(std::tuple<Args...>& t) {
        static_assert (parse<Args...>(), "types in tuple has various types");
    }

    //functor for basic action for one element from tuple
    struct callback
    {
      template<typename T>
      void operator()(int index, T&& t)
      {
          if(index)
              std::cout << ".";
          std::cout << t;
      }
    };
}

//tuple case
template <typename TT>
auto print_ip(TT& input) -> decltype(std::get<0>(std::declval<TT>()), void())
{
    checkHomogeneity(input);
    for_each(input, callback());
}

//base template
template<typename TT>
typename std::enable_if<std::is_integral<typename std::remove_reference<TT>::type>::value, void>::type
print_ip(TT& input)
{
    auto p = (uint8_t*)&input;
    std::cout << static_cast<int>(*(p + sizeof (TT) - 1));

    for(int i = static_cast<int>(sizeof (TT) - 2); i >= 0; i--) {
        std::cout << ".";
        std::cout << static_cast<int>(*(p + i));
    }

    std::cout << std::endl;
    return;
}

//specialization
template <>
void print_ip<char>(char& input)
{
    auto p = (uint8_t*)&input;
    std::cout << static_cast<int>(*(p)) << std::endl;
    return;
}

//overloaded template
template<typename TT>
typename std::enable_if<is_container<typename std::remove_reference<TT>::type>::value, void>::type
print_ip(TT& input)
{    
    std::cout << *(input.begin());
    for(auto & element: input) {        
        std::cout << ".";
        std::cout << element;
    }
    std::cout << std::endl;
}

//specialization
template <>
void print_ip<std::string>(std::string& input)
{
    std::cout << input << std::endl;
}

int main()
{
    char num1 = -1;
    short num2(0);
    int num3(2130706433);
    long num4(8875824491850138409);

    print_ip(num1);
    print_ip(num2);
    print_ip(num3);
    print_ip(num4);

    std::list<int> list {196,168,0,7};
    std::vector<int> vector {255,0,1,81,98,8};
    std::string string("13.58.94.75");

    print_ip(list);
    print_ip(vector);
    print_ip(string);

    std::tuple<int, int, int, int> tuple{192, 167, 0, 1};
    print_ip(tuple);

    return 0;
}
