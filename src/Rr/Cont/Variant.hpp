//
// Variant.hpp
//
// Created on:  May 13, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#if !defined(RR_CONT_VARIANT_HPP_)
#define RR_CONT_VARIANT_HPP_

#include <Rr/Trait/Max.hpp>
#include <Rr/Trait/IntegralToType.hpp>
#include <Rr/Trait/TypeToIntegral.hpp>
#include <Rr/Trait/Forward.hpp>
#include <Rr/Util/DefaultConfig.hpp>

namespace Rr {
namespace Cont {
namespace VariantImpl {

template <class ...Ta>
class Variant;

template <class Talt, class ...Tvargs>
bool holdsAlternative(const Variant<Tvargs...> &aArgs);

template <class ...Ta>
class Variant final {
private:
	template <int I>
	using GetType = typename Rr::Trait::IntToType<I, Ta...>;

	template <class T>
	using GetIndex = typename Rr::Trait::TypeToInt<T, Ta...>;

public:
	template <class T>
	Variant(T &&aValue) : index{GetIndex<T>::value}
	{
		static_assert(Rr::Trait::TypeIn<T, Ta...>::value, "The provided type is not among those the variant can hold");
		emplace<T>(Rr::Trait::forward(aValue));
	}

	~Variant()
	{
		destruct<0>();
	}


	template <class T, class ...Ts>
	void emplace(Ts &&...aArgs)
	{
		destruct<0>();
		index = GetIndex<T>::value;
		new (cell, Rr::Object()) T{Rr::Trait::forward<T>(aArgs)...};
	}

	template <class Talt, class ...Tvargs>
	friend bool holdsAlternative(const Variant<Tvargs...> &);

private:
	template <int I, class T>
	auto applyVisitor(T &&aVisitor) -> decltype(aVisitor(*reinterpret_cast<typename GetType<I>::Type*>(this->cell)))
	{
		if (index > 0) {
			if (I == index) {
				return aVisitor(*reinterpret_cast<typename GetType<I>::Type*>(cell));
			} else {
				return applyVisitor<I + 1>(Rr::Trait::forward(aVisitor));
			}
		}
	}

	template <class T>
	auto applyVisitor(T &&visitor) -> decltype(applyVisitor<0>(Rr::Trait::forward(visitor)))
	{
		applyVisitor<0>(Rr::Trait::forward(visitor));
	}

	template <int I>
	inline void destruct()
	{
		using Type = typename GetType<I>::Type;

		if (index > 0 && I < sizeof...(Ta)) {
			if (holdsAlternative<Type>(*this)) {
				reinterpret_cast<Type *>(cell)->~Type();
				index = -1;
			} else {
				destruct<I + 1>();
			}
		}
	}

private:
	alignas(decltype(sizeof(int*))) unsigned char cell[Rr::Trait::MaxSizeof<Ta...>::value];
	int index;
};

template <class Talt, class ...Tvargs>
bool holdsAlternative(const Variant<Tvargs...> &aArgs)
{
	return aArgs.index > 0 && Variant<Tvargs...>::template GetIndex<Talt>::value == aArgs.index;
}

}  // VariantImpl

using VariantImpl::Variant;

}  // Cont
}  // Rr

#endif  // RR_CONT_VARIANT_HPP_
