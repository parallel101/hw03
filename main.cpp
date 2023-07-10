#include <iostream>
#include <vector>
#include <variant>

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
template <class T1, class T2>   // 第一个加法，  vector相加
std::vector<std::common_type_t<T1,T2>> operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}
    std::vector<std::common_type_t<T1,T2>> sum;
    size_t N = std::min(a.size(), b.size());
    for (size_t i=0;i<N;i++){
        sum.push_back(a[i] + b[i]);
    }
    return sum;
}

template <class T1, class T2>   // 第二个加法，   variant+variant
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, std::variant<T1, T2> const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分

    return std::visit([&](const auto& i, const auto& j)->std::variant<T1, T2> {return i + j;}, a, b);
}

template <class T1, class T2>   // 第二个加法，   variant+vector
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, T2 const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分

    return a + std::variant<T1, T2>{b};
}

template <class T1, class T2>   // 第二个加法，   vector+variant
std::variant<T1, T2> operator+(T1  const &a, std::variant<T1, T2> const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    return b + std::variant<T1, T2>{a};
}

template <class T1,class... Targs>
std::ostream &operator<<(std::ostream &os, std::variant<T1,Targs...> const &args){
    std::visit([&](const auto& i){std::cout << i << std::endl;}, args);
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

    std::variant<int> temp{666};
    std::cout << temp << std::endl;

    return 0;
}
