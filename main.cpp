#include <iostream>
#include <vector>
#include <variant>
#include <algorithm>

// 请修复这个函数的定义：10 分
template <class T>
std::ostream &operator<<(std::ostream &os, std::vector<T> const &a) {
    os << "{";
    std::copy(std::begin(x), std::end(x),std::ostream_iterator(os, ", "));
    os << "}";
    return os;
}

// 请修复这个函数的定义：10 分
template <class T1, class T2>
auto operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}
    using T0=decltype(T1{}+T2{});
    std::vector<T0> c;
    std::transform (a.begin(), a.end(), b.begin(), std::back_inserter(c), std::plus<int>());
    return c;
}

template <class T1, class T2>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, T1 const &b)
{
    return std::static_cast<std::variant<T1, T2>>(b)+a;
}

template <class T1, class T2>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, T2 const &b)
{
    return std::static_cast<std::variant<T1, T2>>(b)+a;
}


template <class T1, class T2>
std::variant<T1, T2> operator+(T1 const &a, std::variant<T1, T2> const &b)
{
    return std::static_cast<std::variant<T1, T2>>(a)+b;
}

template <class T1, class T2>
std::variant<T1, T2> operator+(T2 const &a, std::variant<T1, T2> const &b)
{
    return std::static_cast<std::variant<T1, T2>>(a)+b;
}

template <class T1, class T2>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, std::variant<T1, T2> const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    return std::visit([](auto const& x,auto const& y){return x+y;},a,b);
}

// template <class T1, class... T2>
// std::variant<T1, T2...> operator+(std::variant<T1, T2...> const &a, std::variant<T1, T2...> const &b) {
//     // 请实现自动匹配容器中具体类型的加法！10 分
//     return std::visit([](auto const& x,auto const& y){return x+(y + ...);},a,b);
// }

template <class T1, class T2>
std::ostream &operator<<(std::ostream &os, std::variant<T1, T2> const &a) {
    // 请实现自动匹配容器中具体类型的打印！10 分
    return std::visit(
        [](auto const& x){
            return os<<x;
        },
        a        
    );
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

    // 应该输出 {9.28, 17.436, 7.236}
    std::cout << d << std::endl;

    return 0;
}
