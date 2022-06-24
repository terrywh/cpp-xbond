#pragma once

namespace xbond {
namespace detail {
template <class T, std::size_t N, std::size_t... I>
constexpr std::array<std::remove_cv_t<T>, N>
    to_array_impl(T (&a)[N], std::index_sequence<I...>) {
    return { {a[I]...} };
}
} // namespace detail
 
template <class T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&a)[N]) {
    return detail::to_array_impl(a, std::make_index_sequence<N>{});
}

} // namespace xbond