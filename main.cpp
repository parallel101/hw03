#include <iostream>
#include <vector>
#include <variant>
#include <utility>

// 请修复这个函数的定义：10 分
template<class T>
std::ostream &operator<<(std::ostream &os, std::vector<T> const &a) {
    os << "{";
    for (size_t i = 0; i < a.size(); i++) {
        os << a[i];
        if (i != a.size() - 1)
            os << ", ";
    }
    os << "}";
    return os;
}

// 请修复这个函数的定义：10 分
template <class T1, class T2>
decltype(auto) operator+(std::vector<T1> const& a, std::vector<T2> const& b) {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}
    std::vector<decltype(T1{} + T2{}) > out;
    auto x = a.cbegin();
    auto y = b.cbegin();
    for (;x != a.cend() && y != b.cend(); ++x, ++y) {
        out.emplace_back(*x + *y);
    }
    return out;
}

template <class T1, class T2>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, std::variant<T1, T2> const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    std::variant<T1, T2> out;
    if (std::holds_alternative<T1>(a) && std::holds_alternative<T1>(b))
        out = std::get<T1>(a) + std::get<T1>(b);
    else if (std::holds_alternative<T1>(a) && std::holds_alternative<T2>(b))
        out = std::get<T1>(a) + std::get<T2>(b);
    else if (std::holds_alternative<T2>(a) && std::holds_alternative<T2>(b))
        out = std::get<T2>(a) + std::get<T2>(b);
    else if (std::holds_alternative<T2>(a) && std::holds_alternative<T1>(b))
        out = std::get<T2>(a) + std::get<T1>(b);
    return out;
}

template <class T1, class T2, class T3>
std::variant<T1, T2> operator+(std::variant<T1, T2> const& a, std::vector<T3> const& b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    std::variant<T1, T2> out;
    if (std::holds_alternative<T1>(a))
        out = std::get<T1>(a) + b;
    else if (std::holds_alternative<T2>(a))
        out = std::get<T2>(a) + b;
    return out;
}

template<class V, size_t N>
constexpr void finner(std::ostream& os, V const& a) {
    //if (std::holds_alternative<decltype(std::get<N>(a))>(a)) {
    //    os << std::get<N>(a);
    //}

    // auto e = std::get<N>(a);
    //if (auto* x = std::get_if<int>(&a)) {
    //    os << *x;
    //}

    try
    {
        if (N == a.index()) {
            auto e = std::get<N>(a);
            using E = decltype(e);
            if (std::holds_alternative<E>(a)) 
                os << e;
        }
    }
    catch (std::bad_variant_access const&)
    {
        // std::cout << ex.what();
    }
}

template<class V, size_t...N>
std::ostream& fout(std::ostream& os, V const& a, std::index_sequence<N...>) {
    //auto func = [&](auto x) {
    //    os << x;
    //};
    //func(std::get<N>(a));
    auto fc = [&](auto n) {
        // auto ix = n;

        return n;
        //if (std::holds_alternative<decltype(std::get<n>(a))>(a)) {
        //    os << std::get<n>(a);
        //}
    };

    static_cast<void>(std::initializer_list<int>{(finner<V, N>(os, a), 0)...});
    // constexpr auto s = std::initializer_list<int>{(finner<V, N>(os, a), 0)...};

    //constexpr auto s = std::initializer_list<size_t>{ (fc(N))... };
    //constexpr auto s = std::initializer_list<int>{(finner(os, a, N), 0)...};
    //for (auto i : s) {
    //  os << i;
        //    if (std::holds_alternative<decltype(std::get<i>(a))>(a)) {
        //        os << std::get<i>(a);
        //    }
    //}

    //static_cast<void>(std::initializer_list<int>{(func(std::get<N>(a)), 0)...});
    //if (std::holds_alternative<decltype(std::get<N>(a))>(a)) {
    //    os << std::get<N>(a);
    //}
    return os;
}

template <class... Args>
    requires (sizeof...(Args) > 0)
std::ostream &operator<<(std::ostream &os, std::variant<Args...> const &a) {
    // 请实现自动匹配容器中具体类型的打印！10 分
    return fout(os, a, std::make_index_sequence<sizeof...(Args)>{});
}

int main() {
    std::vector<int> a = {1, 4, 2, 8, 5, 7};
    std::cout << a << std::endl;
    std::vector<double> b = {3.14, 2.718, 0.618};
    std::cout << b << std::endl;
    auto c = a + b;

    // 应该输出 1
    std::cout << std::is_same_v<decltype(c), std::vector<double>> << std::endl;

    // 应该输出 {4.14, 6.718, 2.618}
    std::cout << c << std::endl;

    std::variant<std::vector<int>, std::vector<double>> d = c;
    std::variant<std::vector<int>, std::vector<double>> e = a;
    d = d + c + e;

    //d = b;
    //std::variant<std::vector<int>, std::vector<std::string>, std::vector<double>> dd;
    //dd = b;
    // 
    // 应该输出 {9.28, 17.436, 7.236}
    std::cout << d << std::endl;

    return 0;
}
