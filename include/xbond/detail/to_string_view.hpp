#pragma once
#include <string_view>
#include <stdexcept>

namespace xbond {
namespace detail {

template <class T>
struct contain_string_data {
    template <typename U> static auto test(int) -> typename std::enable_if<
            std::is_convertible<decltype(std::declval<U>().data()), const char*>::value &&
            std::is_convertible<decltype(std::declval<U>().size()), std::size_t>::value,
            std::true_type
        >::type;
    template <typename> static auto test(...) -> std::false_type;
    static constexpr bool value = std::is_same<decltype(test<T>(0)), std::true_type>::value;
};

template <class T>
struct convertible_to_string_view {
    template <typename U> static auto test(int) -> typename std::enable_if<
        std::is_convertible<U, std::string_view>::value ||
        contain_string_data<U>::value, std::true_type>::type;
    template <typename> static auto test(...) -> std::false_type;
    static constexpr bool value = std::is_same<decltype(test<T>(0)), std::true_type>::value;
};

template <class S, typename = typename std::enable_if<convertible_to_string_view<S>::value, S>::type>
std::string_view to_string_view(const S& s) {
    if constexpr (std::is_convertible<S, std::string_view>::value) {
        return static_cast<std::string_view>(s);
    } else if constexpr (contain_string_data<S>::value) {
        return std::string_view(s.data(), s.size());
    } else {
        throw std::runtime_error("string type not supported");
    }
}

} // namespace detail
} // namespace xbond
