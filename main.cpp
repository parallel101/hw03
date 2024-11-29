#include <algorithm>
#include <iostream>
#include <type_traits>
#include <variant>
#include <vector>

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

// 请修复这个函数的定义：10 分
template <class T1, class T2>
auto operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
  using T0 = decltype(T1{} + T2{});
  auto lens = std::min(a.size(), b.size());
  std::vector<T0> res(lens);
  if (lens == a.size()) {
    std::transform(a.begin(), a.end(), b.begin(), res.begin(), std::plus<>());
  } else {
    std::transform(b.begin(), b.end(), a.begin(), res.begin(), std::plus<>());
  }
  return res;
  // 请实现列表的逐元素加法！10 分
  // 例如 {1, 2} + {3, 4} = {4, 6}
}

template <class T1, class T2, class U>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, const U &b) {
  using tb = std::decay_t<U>;
  std::variant<T1, T2> res;
  if constexpr (std::is_same_v<T1, tb> || std::is_same_v<T2, tb>) {
    std::visit([&](const auto &x) { res = x + b; }, a);
  } else if constexpr (std::is_same_v<std::variant<T1, T2>, tb>) {

    std::visit([&](const auto &x, const auto &y) { res = x + y; }, a, b);
  }
  return res;
}
// template <class T1, class T2>
// std::variant<T1, T2> operator+(std::variant<T1, T2> const &a,
//                                std::variant<T1, T2> const &b) {

//   std::variant<T1, T2, decltype(T1{} + T2{})> res{};
//   std::visit([&](const auto &a, const auto &v) { res = a + b; }, a, b);
//   return res;
//   // 请实现自动匹配容器中具体类型的加法！10 分
// }

template <class T1, class T2>
std::ostream &operator<<(std::ostream &os, std::variant<T1, T2> const &a) {
  std::visit([&](const auto &a) { os << a; }, a);
  return os;
  // 请实现自动匹配容器中具体类型的打印！10 分
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
