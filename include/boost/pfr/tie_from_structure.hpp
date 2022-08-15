// Copyright (c) 2022 Denis Mikhailov
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PFR_TIE_FROM_STRUCTURE_HPP
#define BOOST_PFR_TIE_FROM_STRUCTURE_HPP
#pragma once

#include <boost/pfr/detail/config.hpp>

#include <boost/pfr/core.hpp>
// TODO: inlude boost/pfr/view_fields.hpp
// TODO: include boost/pfr/view.hpp

#include <boost/pfr/detail/tie_from_structure_tuple.hpp>

/// \file boost/pfr/tie_from_structure.hpp
/// Contains one function `tie_from_structure` and all overloads of interface from `boost/pfr/core.hpp`, required by it function.
///
/// \b Synopsis:

namespace boost { namespace pfr {

/// \brief std::tie-like function that allows assigning to tied values from aggregates.
///
/// \returns an object with lvalue references to `args...`; on assignment of an \aggregate value to that
/// object each field of an aggregate is assigned to the corresponding `args...` reference.
///
/// \b Example:
/// \code
///     auto f() {
///       struct { struct { int x, y } p; short s; } res { { 4, 5 }, 6 };
///       return res;
///     }
///     auto [p, s] = f();
///     tie_from_structure(p, s) = f();
/// \endcode
///
/// \note Caution you may get compiler errors when your program uses `boost::pfr::tie_from_structure` and includes
///       only `boost/pfr/core.hpp`. If this happens, add inclusion 'boost/pfr/tie_from_structure.hpp' or just 
///       replace all `boost/pfr/*.hpp` inclusions with one `boost/pfr.hpp` inclusion.
///
template <typename... Elements>
constexpr detail::tie_from_structure_tuple<Elements...> tie_from_structure(Elements&... args) noexcept {
    return detail::tie_from_structure_tuple<Elements...>(args...);
}

}}

#endif // BOOST_PFR_TIE_FROM_STRUCTURE_HPP
