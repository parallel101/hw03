#include <iostream>
#include <vector>
#include <variant>
#include <utility>

// 请修复这个函数的定义：10 分
template<class T>
std::ostream &operator<<(std::ostream &os, std::vector<T> const &a) {
    os << "{";
    for (size_t i = 0; i < a.size(); ++i) {
        os << a[i];
        if (i != a.size() - 1)
            os << ", ";
    }
    os << "}";
    return os;
}

// 支持加减乘除
enum class Operator {
    Add,
    Sub,
    Mul,
    Div
};

template <class T1, class T2, Operator ops>
constexpr decltype(auto) cal_ops(const T1& a, const T2& b) noexcept {
    static_assert(ops == Operator::Add || ops == Operator::Sub || ops == Operator::Mul || ops == Operator::Div);
    if constexpr (ops == Operator::Add) return a + b;
    else if constexpr (ops == Operator::Sub) return a - b;
    else if constexpr (ops == Operator::Mul) return a * b;
    else if constexpr (ops == Operator::Div) return a / b; // RVO
}

template <class T1, class T2, Operator ops>
struct ops_result {
    using type = decltype(cal_ops<T1, T2, ops>(T1{}, T2{}));
};

template <class T1, class T2, Operator ops>
using ops_result_t = ops_result<T1, T2, ops>::type;

template <class T1, class T2, Operator ops>
constexpr decltype(auto) vector_operator(std::vector<T1> const& a, std::vector<T2> const& b) {
    std::vector<ops_result_t<T1, T2, ops>> out;
    out.reserve(std::min(a.size(), b.size()));
    auto x = a.cbegin();
    auto y = b.cbegin();
    for (; x != a.cend() && y != b.cend(); ++x, ++y)
        out.push_back(std::move(cal_ops<T1, T2, ops>(*x, *y))); // NO RVO
    return out; //NRVO
}

// 请修复这个函数的定义：10 分
template <class T1, class T2>
constexpr decltype(auto) operator+(std::vector<T1> const& a, std::vector<T2> const& b) noexcept {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}
    return vector_operator<T1, T2, Operator::Add>(a, b);
}

template <class T1, class T2>
constexpr decltype(auto) operator-(std::vector<T1> const& a, std::vector<T2> const& b) noexcept {
    return vector_operator<T1, T2, Operator::Sub>(a, b);
}

template <class T1, class T2>
constexpr decltype(auto) operator*(std::vector<T1> const& a, std::vector<T2> const& b) noexcept {
    return vector_operator<T1, T2, Operator::Mul>(a, b);
}

template <class T1, class T2>
constexpr decltype(auto) operator/(std::vector<T1> const& a, std::vector<T2> const& b) noexcept {
    return vector_operator<T1, T2, Operator::Div>(a, b);
}

template<class O, class V, class R, Operator ops, size_t N>
constexpr decltype(auto) op_vector_v(O& o, V const& a, R const& b) noexcept {
    if (N == a.index()) {
        auto& e = std::get<N>(a);
        if (e.size() == 0) // support empty type
            o = b;
        else {
            if constexpr (ops == Operator::Add) o = e + b;
            else if constexpr (ops == Operator::Sub) o = e - b;
            else if constexpr (ops == Operator::Mul) o = e * b;
            else if constexpr (ops == Operator::Div) o = e / b;
        }
    }
}

template<class O, class V, class R, Operator ops, size_t...N>
constexpr decltype(auto) unpack_n_vector(O& o, V const& a, R const& b, std::index_sequence<N...>) noexcept {
    static_cast<void>(std::initializer_list<int>{(op_vector_v<O, V, R, ops, N>(o, a, b), 0)...});
}

template <class... Args, class T>
constexpr decltype(auto) operator+(std::variant<Args...> const& a, std::vector<T> const& b) noexcept {
    // 请实现自动匹配容器中具体类型的加法！10 分
    std::variant<Args...> out;
    unpack_n_vector<decltype(out), decltype(a), decltype(b), Operator::Add>(out, a, b, std::index_sequence_for<Args...>{});
    return out;
}

template <class... Args, class T>
constexpr decltype(auto) operator-(std::variant<Args...> const& a, std::vector<T> const& b) noexcept {
    std::variant<Args...> out;
    unpack_n_vector<decltype(out), decltype(a), decltype(b), Operator::Sub>(out, a, b, std::index_sequence_for<Args...>{});
    return out;
}

template <class... Args, class T>
constexpr decltype(auto) operator*(std::variant<Args...> const& a, std::vector<T> const& b) noexcept {
    std::variant<Args...> out;
    unpack_n_vector<decltype(out), decltype(a), decltype(b), Operator::Mul>(out, a, b, std::index_sequence_for<Args...>{});
    return out;
}

template <class... Args, class T>
constexpr decltype(auto) operator/(std::variant<Args...> const& a, std::vector<T> const& b) noexcept {
    std::variant<Args...> out;
    unpack_n_vector<decltype(out), decltype(a), decltype(b), Operator::Div>(out, a, b, std::index_sequence_for<Args...>{});
    return out;
}

template <class... Args, class T>
constexpr decltype(auto) operator+(std::vector<T> const& b, std::variant<Args...> const& a) noexcept {
    // 请实现自动匹配容器中具体类型的加法！10 分
    return operator+(a, b);
}

template <class... Args, class T>
constexpr decltype(auto) operator-(std::vector<T> const& b, std::variant<Args...> const& a) noexcept {
    return operator-(a, b);
}

template <class... Args, class T>
constexpr decltype(auto) operator*(std::vector<T> const& b, std::variant<Args...> const& a) noexcept {
    return operator*(a, b);
}

template <class... Args, class T>
constexpr decltype(auto) operator/(std::vector<T> const& b, std::variant<Args...> const& a) noexcept {
    return operator/(a, b);
}

template<class O, class V, Operator ops, size_t N>
constexpr decltype(auto) op_variant_v(O& o, V const& a) noexcept {
    static_assert(ops == Operator::Add || ops == Operator::Sub || ops == Operator::Mul || ops == Operator::Div);
    if (N == a.index()) {
        auto& e = std::get<N>(a);
        if constexpr (ops == Operator::Add) o = o + e;
        else if constexpr (ops == Operator::Sub) o = o - e;
        else if constexpr (ops == Operator::Mul) o = o * e;
        else if constexpr (ops == Operator::Div) o = o / e;
    }
}

template<class O, class V, Operator ops, size_t...N>
constexpr decltype(auto) unpack_n_variant(O& o, V const& a, std::index_sequence<N...>) noexcept {
    static_cast<void>(std::initializer_list<int>{(op_variant_v<O, V, ops, N>(o, a), 0)...});
}

template <class... Args>
constexpr decltype(auto) operator+(std::variant<Args...> const& a, std::variant<Args...> const& b) noexcept {
    // 请实现自动匹配容器中具体类型的加法！10 分
    std::variant<Args...> out;
    unpack_n_variant<decltype(out), decltype(a), Operator::Add>(out, a, std::index_sequence_for<Args...>{});
    unpack_n_variant<decltype(out), decltype(a), Operator::Add>(out, b, std::index_sequence_for<Args...>{});
    return out;
}

template <class... Args>
constexpr decltype(auto) operator-(std::variant<Args...> const& a, std::variant<Args...> const& b) noexcept {
    std::variant<Args...> out;
    unpack_n_variant<decltype(out), decltype(a), Operator::Add>(out, a, std::index_sequence_for<Args...>{});
    unpack_n_variant<decltype(out), decltype(a), Operator::Sub>(out, b, std::index_sequence_for<Args...>{});
    return out;
}

template <class... Args>
constexpr decltype(auto) operator*(std::variant<Args...> const& a, std::variant<Args...> const& b) noexcept {
    std::variant<Args...> out;
    unpack_n_variant<decltype(out), decltype(a), Operator::Add>(out, a, std::index_sequence_for<Args...>{});
    unpack_n_variant<decltype(out), decltype(a), Operator::Mul>(out, b, std::index_sequence_for<Args...>{});
    return out;
}

template <class... Args>
constexpr decltype(auto) operator/(std::variant<Args...> const& a, std::variant<Args...> const& b) noexcept {
    std::variant<Args...> out;
    unpack_n_variant<decltype(out), decltype(a), Operator::Add>(out, a, std::index_sequence_for<Args...>{});
    unpack_n_variant<decltype(out), decltype(a), Operator::Div>(out, b, std::index_sequence_for<Args...>{});
    return out;
}

template<class V, size_t N>
decltype(auto) print_v(std::ostream& os, V const& a) {
    if (N == a.index())
        os << std::get<N>(a);
}

template<class V, size_t...N>
std::ostream& unpack_n_print(std::ostream& os, V const& a, std::index_sequence<N...>) {
    static_cast<void>(std::initializer_list<int>{(print_v<V, N>(os, a), 0)... });
    return os;
}

template <class... Args>
    requires (sizeof...(Args) > 0)  // no std::variant<>
std::ostream &operator<<(std::ostream &os, std::variant<Args...> const &a) {
    // 请实现自动匹配容器中具体类型的打印！10 分
    return unpack_n_print(os, a, std::index_sequence_for<Args...>{});
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
