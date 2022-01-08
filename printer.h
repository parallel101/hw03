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
concept Advanceable = requires(UnknownType x) { ++x; };

template<typename UnknownType>  //�ɵ�������Լ����������ͨ����������ṩ���µ����ӿڵĶ���
concept Iterable = requires(UnknownType x) {
	{ std::begin(x) }->Advanceable;
	{ *std::begin(x) }->AnyBut<void>;
	{ std::begin(x) != std::end(x) }->ExplicitlyConvertibleTo<bool>;
};

// ���޸���������Ķ��壺10 ��
// ֧�ִ�ӡһ��Ŀɵ�������
auto& operator<<(SubtypeOf<std::ostream> auto& printer, Iterable auto&& container) requires (
	requires { printer << *std::begin(container); } && !ConstructibleFrom<decltype(container), const char*>) {   //Ҫ�� printer �� begin() �ӿڼ��ݣ������������ַ�����ʹ�ø����أ�

	std::string_view separator = "";
	auto const endPoint = std::end(container);
	printer << "[";
	for (auto startPoint = std::begin(container); startPoint != endPoint; ++startPoint) {
		printer << std::exchange(separator, ", ") << *startPoint;
	}
	printer << "]";
	return printer;
}

//֧�ִ�ӡ�ɵ��������variant
template <class ...Iterable> requires (sizeof...(Iterable) > 0)
auto& operator<<(SubtypeOf<std::ostream> auto& printer, std::variant<Iterable...> const& var)
	requires ((requires { (printer << std::get<Iterable>(var)); }) && ...)
{
	std::visit([&](auto&& arg) { printer << arg; }, var);
	return printer;
	// ��ʵ���Զ�ƥ�������о������͵Ĵ�ӡ��10 ��
}