#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <list>
#include <utility>
#include <tuple>
#include <type_traits>
#include "user_compiletime_check.h"
#include "iterate_tuple.h"

namespace  {
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
    int sizeOfType = sizeof (TT);
    std::cout << static_cast<int>(*(p + sizeOfType - 1));

    if(sizeOfType > 1) {
        for(int i = static_cast<int>(sizeOfType - 2); i >= 0; i--) {
            std::cout << ".";
            std::cout << static_cast<int>(*(p + i));
        }
    }

    std::cout << std::endl;
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

template <typename TT>
typename std::enable_if<std::is_same<typename std::remove_reference<TT>::type, std::string>::value, void>::type
print_ip(TT& input)
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
