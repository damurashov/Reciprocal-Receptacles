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
#include <Rr/Trait/EnableIf.hpp>
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
	using GetTypeTp = typename Rr::Trait::IntToType<I, Ta...>::Type;

	template <class T>
	using GetIndex = typename Rr::Trait::TypeToInt<T, Ta...>;

public:
	Variant() : index{-1}
	{
	}

	template <class T>
	Variant(T &&aValue) : index{-1}
	{
		using Type = Trait::RemoveCvprefTp<decltype(aValue), true, true, false, true>;
		static_assert(Rr::Trait::TypeIn<Type, Ta...>::value, "The provided type is not among those the variant can hold");
		destruct<0>();
		emplace<Type>(Trait::forward<T>(aValue));
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
	template <int I>
	GetTypeTp<I> &getAs()
	{
		static_assert(I < sizeof...(Ta), "The provided index exceeds the number of template arguments for the `Variant`");
		return *reinterpret_cast<GetTypeTp<I> *>(cell);
	}

	template <class T>
	T &getAs()
	{
		static_assert(Rr::Trait::TypeIn<T, Ta...>::value, "The provided type is not among those the variant can hold");
		return *reinterpret_cast<T *>(cell);
	}

	template <int I, bool F = (I >= sizeof...(Ta))>
	inline typename Trait::EnableIf<F>::Type destruct()
	{
	}

	template <int I, bool F = (I >= sizeof...(Ta))>
	inline typename Trait::EnableIf<!F>::Type destruct()
	{
		using Type = GetTypeTp<I>;

		if (-1 != index && I < sizeof...(Ta)) {
			if (I == index) {
				getAs<Type>().~Type();
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
bool holdsAlternative(const Variant<Tvargs...> &aVariant)
{
	return aVariant.index >= 0 && Variant<Tvargs...>::template GetIndex<Talt>::value == aVariant.index;
}

}  // VariantImpl

namespace Vrnt {

using Rr::Cont::VariantImpl::holdsAlternative;

}  // Vrnt

using VariantImpl::Variant;

}  // Cont
}  // Rr

#endif  // RR_CONT_VARIANT_HPP_
