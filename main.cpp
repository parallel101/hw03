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
template <class T1, class T2>
auto operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}
    using T = std::decay_t<decltype(a[0]+b[0])>; //I find the output type when given two vectors.
    std::vector<T> ret;
    for(int i=0; i<std::min(a.size(), b.size()); i++){
        ret.push_back(a[i]+b[i]);
    }
    return ret;
}

template <class T1, class T2>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, std::variant<T1, T2> const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    std::variant<T1, T2> ret;
    std::visit([&](auto const &aa, auto const &bb){ret = aa + bb;}, a, b);
    return ret;
}
template<class T1, class T2, class T3>
auto operator+(std::variant<T1, T2> const& a, T3 const& b){
    /* 
    因为出现了std::variant跟std::vector进行相加的运算
    然而在之前的代码中没有出现相应的输入类型
    因此对这种加法进行overload
    */
    std::variant<T1,T2> ret = b;
    return a + ret;
}

template<class T1, class T2, class T3>
auto operator+(T3 const& a, std::variant<T1, T2> const& b){
    return b + a;
}


// template <class T1, class T2>
// std::ostream &operator<<(std::ostream &os, std::variant<T1, T2> const &a) {
//     std::visit([&](auto const& input){os<<input;}, a);
//     return os;
// }

template<class... T>
std::ostream &operator<<(std::ostream &os, std::variant<T...> &inputs){
/*
    注：相比起之前的问题来说，variadic template输入的函数中需要考虑到输入可能是std::endl或者是其他的function的问题。
    remove const限定之后能够保证代码的正常运行，但是为什么呢？
    没办法理解。
*/
    std::visit([&](auto const& input){os<<input;}, inputs);
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
