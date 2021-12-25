#include <iostream>
#include <vector>
#include <variant>

// 请修复这个函数的定义：10 分
template<typename T>
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
auto operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}

    std::vector<std::common_type_t<T1,T2>> c;

    //add a b with common length
    for(auto index = 0; index != std::min(a.size(),b.size()); ++index)
    {
        c.push_back(a[index]+b[index]);
    }

    return c;
}

template <class T1, class T2, class T>
std::variant<T1, T2> operator+(std::variant<T1, T2>const &a,  T const & b) {
    // 请实现自动匹配容器中具体类型的加法！10 分

    //判断结果容器的元素类型是否为T1的元素类型，或者是T2的元素类型
    static_assert(std::is_same_v<std::common_type_t<typename T1::value_type, typename T::value_type>, typename T1::value_type>||
            std::is_same_v<std::common_type_t<typename T2::value_type, typename T::value_type>, typename T2::value_type>);

    return std::visit([&](auto &&arg1){
        std::variant<T1,T2> v;
        v=arg1+b;
        return v;
    }, a);
}

template <class T1, class T2>
std::variant<T1, T2> operator+(std::variant<T1, T2>const &a, std::variant<T1, T2> const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    return std::visit([&](auto &&arg1, auto && arg2){
        std::variant<T1,T2> v;
        v=arg1+arg2;
        return v;
    }, a, b);
}


template <class T, class ...ARGN>
std::ostream &operator<<(std::ostream &os, std::variant<T, ARGN...> const &a) {
    // 请实现自动匹配容器中具体类型的打印！10 分

    std::visit([&](auto && arg){
       os << arg;
    }, a);

    return os;
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
