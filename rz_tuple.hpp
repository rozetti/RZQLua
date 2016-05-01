#pragma once

// Jossutis

#include <iostream>
#include <tuple>
#include <vector>
#include <sstream>

#include "rz_qlua_types.h"

template <typename TElement>
std::ostream &operator<<(std::ostream &str, std::vector<TElement> const &vv)
{
    str << "[";

    str << std::accumulate(std::begin(vv), std::end(vv), std::string(),
                           [](std::string &ss, TElement const &e)
    {
        return ss.empty() ? (ss + e) : (ss + ", " + e);
    });

    str << "]";

    return str;
}

namespace rz
{
    template <int I, int N, typename... TArgs>
    struct tuple_printer
    {
        static void print(std::ostream &str, std::tuple<TArgs...> const &tuple)
        {
            str << ", " << std::get<I>(tuple);

            tuple_printer<I+1, N, TArgs...>::print(str, tuple);
        }
    };

    template <int N, typename... TArgs>
    struct tuple_printer<0, N, TArgs...>
    {
        static void print(std::ostream &str, std::tuple<TArgs...> const &tuple)
        {
            str << std::get<0>(tuple);
            tuple_printer<1, N, TArgs...>::print(str, tuple);
        }
    };

    template <typename... TArgs>
    struct tuple_printer<0, 0, TArgs...>
    {
        static void print(std::ostream &str, std::tuple<TArgs...> const &/*tuple*/)
        {
        }
    };

    template <int N, typename... TArgs>
    struct tuple_printer<N, N, TArgs...>
    {
        static void print(std::ostream &/*str*/, std::tuple<TArgs...> const &/*tuple*/)
        {
        }
    };
}

template <typename... TArgs>
std::ostream &operator<<(std::ostream &str, std::tuple<TArgs...> const &tuple)
{
    str << "{";
    rz::tuple_printer<0, sizeof...(TArgs), TArgs...>::print(str, tuple);
    str << "}";

    return str;
}


