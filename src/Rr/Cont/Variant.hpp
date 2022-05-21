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
#include <Rr/Trait/RemoveReference.hpp>
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
//	template <int I>
//	using GetTypeTp = typename Rr::Trait::IntToType<I, Ta...>::Type;

	template <class T>
	using GetIndex = typename Rr::Trait::TypeToInt<T, Ta...>;

public:
	Variant() : index{-1}
	{
	}

	template <class T>
	Variant(T &&aValue) : index{-1}
	{
		static_assert(Rr::Trait::TypeIn<T, Ta...>::value, "The provided type is not among those the variant can hold");
		emplace<T>(Trait::forward<T>(aValue));
	}

	~Variant()
	{
	}

	template <class T, class ...Ts>
	T &emplace(Ts &&...aArgs)
	{
		static_assert(Rr::Trait::TypeIn<T, Ta...>::value, "The provided type is not among those the variant can hold");
		index = GetIndex<T>::value;
		new (this->cell, Rr::Object()) T{Rr::Trait::forward<T>(aArgs)...};

		return *reinterpret_cast<T *>(cell);
	}

	template <class Talt, class ...Tvargs>
	friend bool holdsAlternative(const Variant<Tvargs...> &);

private:

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
