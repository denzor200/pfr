











#ifndef BOOST_PFR_DRAFT_HPP
#define BOOST_PFR_DRAFT_HPP
#pragma once

#include <boost/pfr/detail/config.hpp>

#include <boost/pfr/core.hpp>

/////////////////////////////////////////////////////////
// traits_fwd.hpp
namespace boost { namespace pfr {

// Special tag
struct for_everywhat_tag;

template<class T, class WhatFor=for_everywhat_tag>
struct is_reflectable;

template<class T>
struct is_view;

}} // namespace boost::pfr

/////////////////////////////////////////////////////////
// traits.hpp
namespace boost { namespace pfr {
// Special tag
struct for_everywhat_tag;

namespace extension
{
template<class Tag>
struct is_reflectable_impl {
    template<class T>
    struct apply;
};

template<>
struct is_reflectable_impl<for_everywhat_tag> {
    template<class T>
    struct apply : std::integral_constant<bool, false> {
    // TODO: make detailed message
    static_assert(!boost::pfr::is_view<T>::value, "");
    };
};

} // namespace extension

template<class T, class WhatFor>
struct is_reflectable : extension::is_reflectable_impl<WhatFor>::template apply<T> {
};

// TODO: consider placing all specializations in one file(by using _fwd.hpp)
template<class T>
struct is_view : std::integral_constant<bool, false> {};

}} // namespace boost::pfr


/////////////////////////////////////////////////////////
// REGISTRATE_REFLECTABLE.hpp
#define BOOST_PFR_REGISTRATE_REFLECTABLE_FOR_ALL(T)                                                                                         \
namespace boost { namespace pfr {                                                                                                           \
template<class WhatFor>                                                                                                                     \
struct is_reflectable<T, WhatFor> : std::integral_constant<bool, true> {                                                                    \
};                                                                                                                                          \
}}

#define BOOST_PFR_REGISTRATE_REFLECTABLE_FOR_ONE(T, WHAT_FOR)                                                                               \
namespace boost { namespace pfr {                                                                                                           \
template<>                                                                                                                                  \
struct is_reflectable<T, WHAT_FOR> : std::integral_constant<bool, true> {                                                                   \
};                                                                                                                                          \
}}


/////////////////////////////////////////////////////////
// view_fields.hpp
namespace boost { namespace pfr {

namespace detail {

template <class T>
struct view_impl : guaranteed_nonreflectable {
    T value;
};

} // namespace detail

template<class T, class WhatFor>
struct is_reflectable<detail::view_impl<T>, WhatFor> : std::integral_constant<bool, true> {
};

template<class T>
struct is_view<detail::view_impl<T>> : std::integral_constant<bool, true> {
};

template <class T>
auto view(T&& value) noexcept {
    return detail::view_impl<T>{{}, std::forward<T>(value)};
}

template<std::size_t I, class T>
constexpr decltype(auto) get( detail::view_impl<T>& t ) noexcept {
    return boost::pfr::get<I>(std::forward<T>(t.value));
}

template<std::size_t I, class T>
constexpr decltype(auto) get( detail::view_impl<T>&& t ) noexcept {
    return boost::pfr::get<I>(std::forward<T>(t.value));
}

template<std::size_t I, class T>
constexpr decltype(auto) get( const detail::view_impl<T>& t ) noexcept {
    return boost::pfr::get<I>(std::forward<T>(t.value));
}

template<std::size_t I, class T>
constexpr decltype(auto) get( const detail::view_impl<T>&& t ) noexcept {
    return boost::pfr::get<I>(std::forward<T>(t.value));
}

template<std::size_t I, class T>
struct tuple_element<I, detail::view_impl<T> >
    : boost::pfr::tuple_element<I, std::remove_reference_t<T>>
{};

template <class T>
constexpr auto structure_to_tuple(detail::view_impl<T>& t) noexcept {
    return boost::pfr::structure_to_tuple(std::forward<T>(t.value));
}

template <class T>
constexpr auto structure_to_tuple(detail::view_impl<T>&& t) noexcept {
    return boost::pfr::structure_to_tuple(std::forward<T>(t.value));
}

template <class T>
constexpr auto structure_to_tuple(const detail::view_impl<T>& t) noexcept {
    return boost::pfr::structure_to_tuple(std::forward<T>(t.value));
}

template <class T>
constexpr auto structure_to_tuple(const detail::view_impl<T>&& t) noexcept {
    return boost::pfr::structure_to_tuple(std::forward<T>(t.value));
}

template <class T>
constexpr auto structure_tie(detail::view_impl<T>& t) noexcept {
    return boost::pfr::structure_tie(std::forward<T>(t.value));
}

template <class T>
constexpr auto structure_tie(detail::view_impl<T>&& t) noexcept {
    return boost::pfr::structure_tie(std::forward<T>(t.value));
}

template <class T>
constexpr auto structure_tie(const detail::view_impl<T>& t) noexcept {
    return boost::pfr::structure_tie(std::forward<T>(t.value));
}

template <class T>
constexpr auto structure_tie(const detail::view_impl<T>&& t) noexcept {
    return boost::pfr::structure_tie(std::forward<T>(t.value));
}

template <class T, class F>
void for_each_field(detail::view_impl<T>& t, F&& func) {
    boost::pfr::for_each_field(std::forward<T>(t.value), func);
}

template <class T, class F>
void for_each_field(detail::view_impl<T>&& t, F&& func) {
    boost::pfr::for_each_field(std::forward<T>(t.value), func);
}

template <class T, class F>
void for_each_field(const detail::view_impl<T>& t, F&& func) {
    boost::pfr::for_each_field(std::forward<T>(t.value), func);
}

template <class T, class F>
void for_each_field(const detail::view_impl<T>&& t, F&& func) {
    boost::pfr::for_each_field(std::forward<T>(t.value), func);
}

template <class T>
struct tuple_size<detail::view_impl<T>>
    : boost::pfr::tuple_size<std::remove_reference_t<T>>
{};

}} // namespace boost::pfr

#endif // BOOST_PFR_DRAFT_HPP