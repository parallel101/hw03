#pragma once

#include <tuple>
#include <variant>
#include <string_view>

#define Forward(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

template<typename UnknownType, typename ReferenceType>
concept SubtypeOf = std::same_as<std::decay_t<UnknownType>, ReferenceType> || std::derived_from<std::decay_t<UnknownType>, ReferenceType>;

template<typename UnknownType, typename ...ReferenceTypes>
concept AnyOf = (SubtypeOf<UnknownType, ReferenceTypes> || ...);

template<typename UnknownType, typename ...ReferenceTypes>
concept AnyBut = !AnyOf<UnknownType, ReferenceTypes...>;

template<typename UnknownType, typename ReferenceType>
concept ExplicitlyConvertibleTo = requires(UnknownType x) { static_cast<ReferenceType>(Forward(x)); };

template<typename UnknownType, typename ReferenceType>
concept ConstructibleFrom = ExplicitlyConvertibleTo<ReferenceType, std::decay_t<UnknownType>>;

template<typename UnknownType>
concept BuiltinArray = std::is_array_v<std::remove_cvref_t<UnknownType>>;

template<typename UnknownType>
concept Advanceable = requires(UnknownType x) { ++x; };

template<typename UnknownType>  //可迭代对象约束，包括普通数组和任意提供以下迭代接口的对象
concept Iterable = BuiltinArray<UnknownType> || requires(UnknownType x) {
	{ x.begin() }->Advanceable;
	{ *x.begin() }->AnyBut<void>;
	{ x.begin() != x.end() }->ExplicitlyConvertibleTo<bool>;
};

// 请修复这个函数的定义：10 分
// 支持打印一般的可迭代对象
auto& operator<<(SubtypeOf<std::ostream> auto& printer, Iterable auto&& container) requires (
	(requires { printer << *container.begin(); } ||    //要求 printer 与 begin() 接口兼容
		requires { printer << container[0]; }) && !ConstructibleFrom<decltype(container), const char*>)  //或者 printer 与数组元素兼容，但有特例：字符串不使用该重载！
{
	auto [Startpoint, Endpoint] = [&] {
		if constexpr (BuiltinArray<decltype(container)>) {
			return std::tuple{ container, container + sizeof(container) / sizeof(container[0]) };
		}
		else {
			return std::tuple{ container.begin(), container.end() };
		}
	}();
	std::string_view separator = "";
	printer << "{";
	for (; Startpoint != Endpoint; ++Startpoint) {
		printer << std::exchange(separator, ", ") << *Startpoint;
	}
	printer << "}";
	return printer;
}

//支持打印可迭代对象的variant
template <class ...Iterable> requires (sizeof...(Iterable) > 0)
auto& operator<<(SubtypeOf<std::ostream> auto& printer, std::variant<Iterable...> const& var)
	requires ((requires { (printer << std::get<Iterable>(var)); }) && ...)
{
	std::visit([&](auto&& arg) { printer << arg; }, var);
	return printer;
	// 请实现自动匹配容器中具体类型的打印！10 分
}