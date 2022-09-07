











#ifndef BOOST_PFR_DRAFT_HPP
#define BOOST_PFR_DRAFT_HPP
#pragma once

#include <boost/pfr/detail/config.hpp>

#include <boost/pfr/core.hpp>

// TODO: make detailed message with Boost.PFR mark in every static_assert\
// TODO: fix std::forward<decltype(value)> on the Boost Json example
// TODO: fix is_view_impl on the Boost Fusion example

/////////////////////////////////////////////////////////
// detail/traits.hpp

namespace boost { namespace pfr {
namespace detail {

template< class T >
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template< class T1, class T2 >
using is_same_without_cvref = std::is_same<remove_cvref_t<T1>, remove_cvref_t<T2>>;

// trait to be used with a view
// TODO: use std::is_move_constructible here??
template< class From, class To >
using is_two_corresponding_values = std::integral_constant<bool,
                    !std::is_reference<From>::value && !std::is_reference<To>::value &&
                    is_same_without_cvref<From, To>::value>;

}  // namespace detail
}} // namespace boost::pfr


/////////////////////////////////////////////////////////
// traits_fwd.hpp
namespace boost { namespace pfr {

// Special tag
struct for_everywhat_tag;

template<class T, class WhatFor=for_everywhat_tag>
struct is_reflectable;

template<class T>
struct is_view;

template<typename T>
struct is_reference;

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

template<typename T>
struct is_reference : std::is_reference<T> {};

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
// detail/view.hpp
namespace boost { namespace pfr {
namespace detail {

template<class T, class Enable = void>
struct view_impl_base;

template<class T>
struct view_impl_base<T, std::enable_if_t<std::is_reference<T>::value>> {
    T value;
    static_assert(!std::is_rvalue_reference<T>::value, "");
};

template<class T>
struct view_impl_base<T, std::enable_if_t<!std::is_reference<T>::value>> {
    T value;

    view_impl_base() = default;

    template<class U>
    explicit view_impl_base(U&& value, std::enable_if_t<std::is_rvalue_reference<U&&>::value &&
                                                std::is_constructible<T, U&&>::value>* = nullptr)
        : value(std::move(value))
    {
    }

    view_impl_base( const view_impl_base& ) = delete;
    view_impl_base& operator=( const view_impl_base& ) = delete;

    view_impl_base( view_impl_base&& ) = default;
    view_impl_base& operator=( view_impl_base&& ) = default;
};

}  // detail
}} // boost::pfr

/////////////////////////////////////////////////////////
// view.hpp
namespace boost { namespace pfr {

namespace detail {

template<class T>
struct view_impl : view_impl_base<T>, guaranteed_nonreflectable {
    view_impl() = default;
    template<class U>
    explicit view_impl(U&& value, std::enable_if_t<std::is_constructible<T, U&&>::value>* = nullptr)
        : view_impl_base<T>{std::forward<U>(value)}
    {
    }

    template<class U, std::enable_if_t<std::is_reference<U>::value &&
                                       is_same_without_cvref<T, U>::value &&
                                       std::is_constructible<U, T>::value>* = nullptr>
    constexpr operator view_impl<U>() const & {
        // U may be const or non-const
        return view_impl<U>{view_impl_base<T>::value};
    }

    template<class U, std::enable_if_t<
        is_two_corresponding_values<T, U>::value>* = nullptr>
    constexpr operator view_impl<U>() && {
        return view_impl<U>{std::move(view_impl_base<T>::value)};
    }

    template<class U, std::enable_if_t<
        is_two_corresponding_values<T, U>::value>* = nullptr>
    constexpr operator U() && {
        return std::move(view_impl_base<T>::value);
    }
};

} // namespace detail

// TODO: make test for `decltype(std::declval<boost::pfr::detail::view_impl<T>>().value) == decltype(boost::pfr::view(std::declval<T>()).value) == T`

template<class T>
struct is_view<detail::view_impl<T>> : std::integral_constant<bool, true> {
};

template<typename T>
struct is_reference<detail::view_impl<T>> : std::is_reference<T> {};


template <class T>
auto view(T&& value) noexcept {
    return detail::view_impl<T>{std::forward<T>(value)};
}

template<class T>
using view_t = decltype(boost::pfr::view(std::declval<T>()));

template<std::size_t I, class T>
constexpr decltype(auto) get( detail::view_impl<T>& t ) noexcept {
    return boost::pfr::get<I>(t.value);
}

template<std::size_t I, class T>
constexpr decltype(auto) get( detail::view_impl<T>&& t ) noexcept {
    return boost::pfr::get<I>(std::forward<T>(t.value));
}

template<std::size_t I, class T>
constexpr decltype(auto) get( const detail::view_impl<T>& t ) noexcept {
    return boost::pfr::get<I>(t.value);
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
    return boost::pfr::structure_to_tuple(t.value);
}

template <class T>
constexpr auto structure_to_tuple(detail::view_impl<T>&& t) noexcept {
    return boost::pfr::structure_to_tuple(std::forward<T>(t.value));
}

template <class T>
constexpr auto structure_to_tuple(const detail::view_impl<T>& t) noexcept {
    return boost::pfr::structure_to_tuple(t.value);
}

template <class T>
constexpr auto structure_to_tuple(const detail::view_impl<T>&& t) noexcept {
    return boost::pfr::structure_to_tuple(std::forward<T>(t.value));
}

template <class T>
constexpr auto structure_tie(detail::view_impl<T>& t) noexcept {
    return boost::pfr::structure_tie(t.value);
}

template <class T>
constexpr auto structure_tie(detail::view_impl<T>&& t) noexcept {
    return boost::pfr::structure_tie(std::forward<T>(t.value));
}

template <class T>
constexpr auto structure_tie(const detail::view_impl<T>& t) noexcept {
    return boost::pfr::structure_tie(t.value);
}

template <class T>
constexpr auto structure_tie(const detail::view_impl<T>&& t) noexcept {
    return boost::pfr::structure_tie(std::forward<T>(t.value));
}

template <class T, class F>
void for_each_field(detail::view_impl<T>& t, F&& func) {
    boost::pfr::for_each_field(t.value, func);
}

template <class T, class F>
void for_each_field(detail::view_impl<T>&& t, F&& func) {
    boost::pfr::for_each_field(std::forward<T>(t.value), func);
}

template <class T, class F>
void for_each_field(const detail::view_impl<T>& t, F&& func) {
    boost::pfr::for_each_field(t.value, func);
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

///////////////////////////////////////////////////////////////////////////////////
// make_reflectable.hpp

namespace boost { namespace pfr {

// TODO: need failed tests for passing reference to structure and reference to view into make_reflectable
template<class T, class... U, std::enable_if_t<!boost::pfr::is_view<T>::value>* = nullptr>
T make_reflectable(U&&... u) {
    static_assert(!boost::pfr::is_reference<T>::value, "====================> Boost.PFR: Can't return a reference to anything");
    return T{std::forward<U>(u)...};
}

template<class T, class... U, std::enable_if_t<boost::pfr::is_view<T>::value>* = nullptr>
T make_reflectable(U&&... u) {
    using underlying_type = detail::remove_cvref_t<decltype(std::declval<T>().value)>;
    static_assert(!boost::pfr::is_reference<T>::value, "====================> Boost.PFR: Can't return a reference to anything");
    return boost::pfr::view(underlying_type{std::forward<U>(u)...});
}

namespace detail {
template<class T, class F, std::size_t... I>
T make_reflectable_impl(F&& make_element_func, std::index_sequence<I...>) {
    return boost::pfr::make_reflectable<T>(make_element_func(size_t_<I>{})...);
}
} // namespace detail

template<class T, class F>
T make_reflectable(F&& make_element_func) {
    constexpr std::size_t fields_count
                = boost::pfr::tuple_size_v<std::remove_reference_t<T>>;
    return detail::make_reflectable_impl<T>(std::forward<F>(make_element_func),
                                            std::make_index_sequence<fields_count>());
}
  
}} // namespace boost::pfr

#endif // BOOST_PFR_DRAFT_HPP
