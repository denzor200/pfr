// Copyright (c) 2023 Bela Schaum, X-Ryl669, Denis Mikhailov.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


// Initial implementation by Bela Schaum, https://github.com/schaumb
// The way to make it union and UB free by X-Ryl669, https://github.com/X-Ryl669
//

#ifndef BOOST_PFR_DETAIL_CORE_NAME14_DISABLED_HPP
#define BOOST_PFR_DETAIL_CORE_NAME14_DISABLED_HPP
#pragma once

#include <boost/pfr/detail/config.hpp>
#include <boost/pfr/detail/sequence_tuple.hpp>

namespace boost { namespace pfr { namespace detail {

// TODO: move it outside
template <class... Args>
constexpr auto make_sequence_tuple(Args... args) noexcept {
    return sequence_tuple::tuple<Args...>{ args... };
}

template <class T, std::size_t I>
constexpr auto get_name() noexcept {
    static_assert(
        sizeof(T) && false,
        "====================> Boost.PFR: Field's names extracting functionality requires C++20 and compiler that supports __PRETTY_FUNCTION__ or __FUNCSIG__ macro (GCC, Clang or MSVC)."
    );
    
    return nullptr;
}

template <class T>
constexpr auto tie_as_names_tuple() noexcept {
    static_assert(
        sizeof(T) && false,
        "====================> Boost.PFR: Field's names extracting functionality requires C++20 and compiler that supports __PRETTY_FUNCTION__ or __FUNCSIG__ macro (GCC, Clang or MSVC)."
    );

    return detail::make_sequence_tuple(); 
}

}}}

#endif // BOOST_PFR_DETAIL_CORE_NAME14_DISABLED_HPP
