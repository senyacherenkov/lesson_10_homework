#pragma once
#include <string>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <list>
#include <utility>
#include <tuple>
#include <type_traits>

//check stl-container type
template <typename T>
struct is_container : std::false_type
{};

template <typename ... Args>
struct is_container<std::vector<Args...>> : std::true_type
{};

template <typename ... Args>
struct is_container<std::list<Args...>> : std::true_type
{};

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
