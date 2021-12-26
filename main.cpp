#include <iostream>
#include <vector>
#include <variant>
#include <concepts>


// 请修复这个函数的定义：10 分
template <class T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& a) {
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
auto operator+(std::vector<T1> const& a, std::vector<T2> const& b) {
	using TR = std::decay_t<decltype(a.back() + b.back())>;
	std::vector<TR> res;
	const auto size = std::min(a.size(), b.size());
	res.reserve(size);
	for (size_t i = 0; i < size; i++) {
		res.emplace_back(a[i] + b[i]);
	}
	return res;
	// 请实现列表的逐元素加法！10 分
	// 例如 {1, 2} + {3, 4} = {4, 6}
}

template <class ...Ts>
std::variant<Ts...> operator+(std::variant<Ts...> const& a, std::variant<Ts...> const& b) {
	std::variant<Ts...> res;
	std::visit([&](auto&& arg) {res = arg + b; }, a);
	return res;
	// 请实现自动匹配容器中具体类型的加法！10 分
}

template <class TR, class ...Ts> requires (std::same_as<TR, Ts> || ...)
std::variant<Ts...> operator+(std::variant<Ts...> const& a, TR const& b) {
	std::variant<Ts...> res;
	std::visit([&](auto&& arg) {res = arg + b; }, a);
	return res;
	// 请实现自动匹配容器中具体类型的加法！10 分
}

template <class TL, class ...Ts> requires (std::same_as<TL, Ts> || ...)
std::variant<Ts...> operator+(TL const& a, std::variant<Ts...> const& b) {
	std::variant<Ts...> res;
	std::visit([&](auto&& arg) {res = a + arg; }, b);
	return res;
	// 请实现自动匹配容器中具体类型的加法！10 分
}

template <class ...Ts> requires (sizeof...(Ts) > 0)
std::ostream& operator<<(std::ostream& os, std::variant<Ts...> const& a) {
	std::visit([&](auto&& arg) { os << arg; }, a);
	return os;
	// 请实现自动匹配容器中具体类型的打印！10 分
}

int main() {
	std::vector<int> a = { 1, 4, 2, 8, 5, 7 };
	std::cout << a << std::endl;
	std::vector<double> b = { 3.14, 2.718, 0.618 };
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
