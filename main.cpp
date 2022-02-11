#include <iostream>
#include <ostream>
#include <type_traits>
#include <vector>
#include <variant>

// 请修复这个函数的定义：10 分
// A: 没有加模板声明
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
// 使用type_traits中的common_type
// 为了让返回值长度短点，能否修改下一行为
// template <class T1, class T2, class T3 = typename std::common_type<T1, T2>::type >
template <class T1, class T2>
std::vector<typename std::common_type<T1, T2>::type> operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}
    using elemtype = typename std::common_type<T1, T2>::type;
    auto len = std::min(a.size(), b.size());
    std::vector<elemtype> vec(len);
    for (size_t i = 0; i != len; i++) {
        vec[i] = a[i] + b[i];
    }
    return vec;
}

template <class T1, class T2>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, std::variant<T1, T2> const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    return std::visit([](const auto &x, const auto &y) -> std::variant<T1, T2> {
        return x + y;
    }, a, b);
}

// 踩坑：如果只有一个变长模板参数，则会默认实例化出来一个变长参数为空的实例出来
// 所以需要是 1 + * 的方式定义模板参数，不能只有一个 *
template <typename Arg, typename ... More>
std::ostream &operator<<(std::ostream &os, std::variant<Arg, More...> const &a) {
    // 请实现自动匹配容器中具体类型的打印！10 分
    return std::visit([&](const auto &x) -> std::ostream& {
        return os << x;
    }, a);
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

    // 这里vector不能隐式转换为variant, 可能是因为T1, T2模板参数推断不出来？
    // 不知道如何解决，只能强制转换了。
    d = d + static_cast<std::variant<std::vector<int>, std::vector<double>>>(c) + e;

    // 应该输出 {9.28, 17.436, 7.236}
    std::cout << d << std::endl;

    return 0;
}
