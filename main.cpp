#include "printer.h"

#include <iostream>
#include <vector>

// 请修复这个函数的定义：10 分
//template <class T>
//std::ostream& operator<<(std::ostream& os, std::vector<T> const& a) {
//	os << "{";
//	for (size_t i = 0; i < a.size(); i++) {
//		os << a[i];
//		if (i != a.size() - 1)
//			os << ", ";
//	}
//	os << "}";
//	return os;
//}

template<class T1, class T2>
concept Same = std::same_as<std::decay_t<T1>, std::decay_t<T2>>;

template<class T1, class T2>  //约束：可加对象
concept Addable = requires (T1 a, T2 b) {
	{a + b} -> AnyBut<void>;
};

template<template<typename...> typename TypeSet, typename ...Ts1, typename ...Ts2>  //类模板参数拼接：递归基
constexpr auto concat(TypeSet<Ts1...>, TypeSet<Ts2...>) -> TypeSet<Ts1..., Ts2...> {
	return TypeSet<Ts1..., Ts2...>{};
}

template<template<typename...> typename TypeSet, typename ...Ts1, typename ...Ts2, typename... Rest>
constexpr auto concat(TypeSet<Ts1...>, TypeSet<Ts2...>, Rest...) {    //类模板参数拼接
	return concat(TypeSet<Ts1..., Ts2...>{}, Rest{}...);
}

template<template<typename, typename...> typename TypeSet, typename T, typename ...Rest>  //类模板参数去重
constexpr auto make_unique_typeset(TypeSet<T, Rest...>) {
	if constexpr ((std::same_as<T, Rest> || ...)) {
		return make_unique_typeset(TypeSet<Rest... >{});
	}
	else {
		if constexpr (sizeof...(Rest) > 0) {
			return concat(TypeSet<T>{}, make_unique_typeset(TypeSet<Rest...>{}));
		}
		else {
			return TypeSet<T>{};
		}
	}
}

template <class T1, class T2> requires Addable<T1, T2>   //向量加法，要求向量元素是可加的
auto operator+(std::vector<T1> const& a, std::vector<T2> const& b) {
	using TR = std::decay_t<decltype(std::declval<T1>() + std::declval<T2>())>;
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

template <class TR, class ...Ts> requires (Addable<Ts, TR> && ...) && (!Same<TR, std::variant<Ts...>>)
auto operator+(std::variant<Ts...> const& a, TR const& b) {  //variant对象+对象，要求variant的每个子类型和对象都是可加的
	std::variant<std::decay_t<decltype(std::declval<Ts>() + std::declval<TR>())>...> new_res;  //variant通过加法新生成的类型
	auto res = make_unique_typeset(concat(std::variant<Ts...>{}, new_res));  //variant新旧类型进行拼接，然后去重
	std::visit([&](auto&& arg) {res = arg + b; }, a);
	return res;
	// 请实现自动匹配容器中具体类型的加法！10 分
}

template <class TL, class ...Ts> requires (Addable<TL, Ts> && ...) && (!Same<TL, std::variant<Ts...>>)
auto operator+(TL const& a, std::variant<Ts...> const& b) {  //对象+variant对象，要求对象和variant的每个子类型都是可加的
	std::variant<std::decay_t<decltype(std::declval<TL>() + std::declval<Ts>())>...> new_res;  //variant通过加法新生成的类型
	auto res = make_unique_typeset(concat(std::variant<Ts...>{}, new_res));  //variant新旧类型进行拼接，然后去重
	std::visit([&](auto&& arg) {res = a + arg; }, b);
	return res;
	// 请实现自动匹配容器中具体类型的加法！10 分
}

template <class ...TLs, class ...TRs> requires (Addable<TLs, std::variant<TRs...>> && ...) && (Addable<std::variant<TLs...>, TRs> && ...)
auto operator+(std::variant<TLs...> const& a, std::variant<TRs...> const& b) { //两个variant对象相加，要求第一个variant的任意一个子类型与第二个variant的任意一个子类型可加
	auto res = make_unique_typeset(
		concat(a, b, (TLs{} + std::variant<TRs...>{})..., (std::variant<TLs...>{} + TRs{})...));  //variant新旧类型进行拼接，然后去重
	std::visit([&](auto&& arg1, auto&& arg2) {res = arg1 + arg2; }, a, b);
	return res;
	// 请实现自动匹配容器中具体类型的加法！10 分
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
