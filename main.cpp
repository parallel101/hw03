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
    using T0 = decltype(T1{} + T2{});
    std::vector<T0> res;
    for( size_t i = 0; i < std::min(a.size(), b.size());i++){
        res.push_back(a[i] + b[i]);
    }
    return res;
}

template <class T1, class T2>
auto operator+(std::variant<T1, T2> const &a, std::variant<T1, T2> const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    return std::visit([&] (auto const& t1,auto const& t2){
        return std::variant<T1,T2>{t1+t2};
    },a , b);
}

template <class T1, class T2>
auto operator+(std::variant<T1, T2> const &a, T1 const &b) {
    // variant + vector
    return a + std::variant<T1,T2>{b};
}

template <class T1, class T2>
auto operator+(std::variant<T1, T2> const &a, T2 const &b) {
    // variant + vector
    // 为什么 vector<T2> const& b  - > T2 const &b ?  T2 is more generic type
    return a + std::variant<T1,T2>{b};
}


//template <class T1, class T2>
//std::ostream &operator<<(std::ostream &os, std::variant<T1, T2> const &a) {
//    // 请实现自动匹配容器中具体类型的打印！10 分
//    std::visit([&](auto const& t){
//        os<<t;
//    },a);
//    return os;
//}
//
template <class T1, class... args>
std::ostream &operator<<(std::ostream &os, std::variant<T1, args...> const &a) {
    // 请实现自动匹配容器中具体类型的打印！10 分
    std::visit([&](auto const& t){
        os<<t;
    },a);
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

    //test for Variadic Templates
    std::variant<std::vector<int>,std::string,std::vector<double>> test_val;
    test_val = c;
    std::cout << test_val << std::endl;
    test_val = "i am a string";
    std::cout << test_val;

    return 0;
}
