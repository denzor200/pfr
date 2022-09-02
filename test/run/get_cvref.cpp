// Copyright (c) 2022 Denis Mikhailov
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/pfr/core.hpp>
#include <memory>
#include <type_traits>
#include <boost/core/lightweight_test.hpp>

struct anon {
    int a;
    int b;
};

int main() {
    anon x = {};
    const anon cx = {};
    //volatile anon vx = {};
    //const volatile anon cvx = {};
    static_assert(std::is_same<std::remove_reference_t<decltype(boost::pfr::get<0>(x))>, int>::value, "");
    static_assert(std::is_same<std::remove_reference_t<decltype(boost::pfr::get<0>(cx))>, const int>::value, "");
    //static_assert(std::is_same<std::remove_reference_t<decltype(boost::pfr::get<0>(vx))>, volatile int>::value, "");
    //static_assert(std::is_same<std::remove_reference_t<decltype(boost::pfr::get<0>(cvx))>, const volatile int>::value, "");

    return boost::report_errors();
}
