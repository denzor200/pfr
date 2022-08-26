











#ifndef BOOST_PFR_DRAFT_HPP
#define BOOST_PFR_DRAFT_HPP
#pragma once

#include <boost/pfr/detail/config.hpp>

#include <boost/pfr/core.hpp>

// TODO: make detailed message with Boost.PFR mark in every static_assert

/////////////////////////////////////////////////////////
// detail/traits.hpp

namespace boost { namespace pfr {
namespace detail {

template< class T >
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template< class T1, class T2 >
using is_same_without_cvref = std::is_same<remove_cvref_t<T1>, remove_cvref_t<T2>>;

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
    explicit view_impl_base(U&& value, std::enable_if_t<std::is_rvalue_reference<U&&>::value
                                        && std::is_constructible<T, U&&>::value>* = nullptr)
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
struct view_impl : guaranteed_nonreflectable {
    T value;
    view_impl( view_impl&& ) = default;
    view_impl& operator=( view_impl&& ) = default;
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
    return detail::view_impl<T>{{}, std::forward<T>(value)};
}

template<class T>
using view_t = decltype(boost::pfr::view(std::declval<T>()));
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

///////////////////////////////////////////////////////////////////////////////////
// make_reflectable.hpp

namespace boost { namespace pfr {

template<class T, class... U, std::enable_if_t<!boost::pfr::is_view<detail::remove_cvref_t<T>>::value, bool> = true>
T make_reflectable(U&&... u) {
    static_assert(!boost::pfr::is_reference<T>::value, "Can't return a reference to anything");
    return T{std::forward<U>(u)...};
}

template<class T, class... U, std::enable_if_t<boost::pfr::is_view<detail::remove_cvref_t<T>>::value, bool> = true>
detail::remove_cvref_t<T> make_reflectable(U&&... u) {
    using clean_t = detail::remove_cvref_t<T>;
    using underlying_type = detail::remove_cvref_t<decltype(std::declval<T>().value)>;
    static_assert(!boost::pfr::is_reference<clean_t>::value, "Can't return a reference to anything");
    return boost::pfr::view(underlying_type{std::forward<U>(u)...});
}

}} // namespace boost::pfr

#endif // BOOST_PFR_DRAFT_HPP