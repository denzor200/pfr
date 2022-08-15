// Copyright (c) 2018 Adam Butcher, Antony Polukhin
// Copyright (c) 2019-2021 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PFR_DETAIL_TIE_FROM_STRUCTURE_TUPLE_HPP
#define BOOST_PFR_DETAIL_TIE_FROM_STRUCTURE_TUPLE_HPP
#pragma once

#include <boost/pfr/detail/config.hpp>

#include <boost/pfr/core.hpp>

#include <tuple>

namespace boost { namespace pfr { namespace detail {

/// \brief A `std::tuple` capable of de-structuring assignment used to support
/// a tie of multiple lvalue references to fields of an aggregate T.
///
/// \sa boost::pfr::tie_from_structure
template <typename... Elements>
struct tie_from_structure_tuple : std::tuple<Elements&...> {
    using base = std::tuple<Elements&...>;
    using base::base;

    template <typename T>
    constexpr tie_from_structure_tuple& operator= (T const& t) {
        base::operator=(boost::pfr::structure_tie(t));
        return *this;
    }
};

}}} // namespace boost::pfr::detail

#endif // BOOST_PFR_DETAIL_TIE_FROM_STRUCTURE_TUPLE_HPP
