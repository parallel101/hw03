#include <iostream>
#include <vector>
#include <variant>

// 请修复这个函数的定义：10 分
template <typename T>
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

template <bool B, class T1, class T2>
struct _if;

template <class T1,class T2>
struct _if<true, T1, T2>
{
    using value = T1;
};

template <class T1,class T2>
struct _if<false, T1, T2>
{
    using value = T2;
};

template <class T1, class T2>
struct bigger_size
{
    using value = typename _if< bool(sizeof(T1)>sizeof(T2)), T1, T2 >::value;
};

// 请修复这个函数的定义：10 分
template <class T1, class T2, class T0 = typename bigger_size<T1,T2>::value>
std::vector<T0> operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}
    const size_t s = std::min(a.size(), b.size());
    std::vector<T0> v(s);
    for(int i=0; i<s; i++){
        v[i] = a[i] + b[i];
    }
    return v;
}

template<class T1, class T2>
std::variant<T1, T2> 
operator+(std::variant<T1,T2> const &a, T2 const& b){
    std::variant<T1, T2> v;
    if(a.index()==0)
        v = std::get<0>(a) + b;
    else
        v = std::get<1>(a) + b;
    return v;
}

template<class T1, class T2>
std::variant<T1, T2> 
operator+(std::variant<T1,T2> const &a, T1 const& b){
    std::variant<T1, T2> v;
    if(a.index()==0)
        v = std::get<0>(a) + b;
    else
        v = std::get<1>(a) + b;
    return v;
}

template <class T1, class T2>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, std::variant<T1, T2> const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    std::variant<T1, T2> v;
    if(b.index()==0)
        v = a + std::get<0>(b);
    else
        v = a + std::get<1>(b);
    return v;
}

template <class T0, class ...T>
std::ostream &operator<<(std::ostream &os, std::variant<T0, T...> const &a) {
    // 请实现自动匹配容器中具体类型的打印！10 分
    std::visit([&](const auto& t){
        os << t;
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
