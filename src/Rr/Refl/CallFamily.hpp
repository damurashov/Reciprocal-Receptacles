//
// CallFamily.hpp
//
// Created on: 2022-01-16
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <Rr/Refl/NoMember.hpp>
#include <Rr/Trait/Forward.hpp>
#include <Rr/Trait/IsSame.hpp>
#include <Rr/Trait/IntegralConstant.hpp>

namespace Rr {
namespace Refl {

namespace CallFamilyImpl {

struct DefaultOverload {
	static constexpr NoMember call(...)
	{
		return {};
	}
};

template <class Toverload>
struct MakeFullOverload : Toverload, DefaultOverload {
	using DefaultOverload::call;
	using Toverload::call;
};

struct Marker;

template <class Tso, class ...TsOs>
struct CallFamily
{
	template <class T, class ...Ta>
	static auto call(T &&aInstance, Ta &&...aArgs) -> decltype(
		Trait::ConditionalTp<
			// if overload is defined for this candidate
			!Trait::IsSame<
			NoMember,
			decltype(Tso::call(Trait::forward<T>(aInstance), Trait::forward<Ta>(aArgs)...))>  // return type of the candidate
			::value,
			// then use the candidate
			Tso,
			// else try other candidate
			CallFamily<TsOs...>>
			// endif
			// call upon inferred type
			::call(Trait::forward<T>(aInstance), Trait::forward<Ta>(aArgs)...)
	)
	{
		return Trait::ConditionalTp<
			// if overload is defined for this candidate
			!Trait::IsSame<
			NoMember,
			decltype(Tso::call(Trait::forward<T>(aInstance), Trait::forward<Ta>(aArgs)...))>  // return type of the candidate
			::value,
			// then use the candidate
			Tso,
			// else try other candidate
			CallFamily<TsOs...>>
			// endif
			// call upon inferred type
			::call(Trait::forward<T>(aInstance), Trait::forward<Ta>(aArgs)...);
	}
};

///
/// @brief Recursion fallback.
///
template <>
struct CallFamily<Marker> : DefaultOverload {
};

}  // namespace CallFamilyImpl

///
/// @brief Tries to call a certain method with a certain set of arguments. For usage example, please refer to
/// <Test/Refl/CallFamily.cpp>.
///
/// Motivation: certain containers may provide similar functionality under diffirent names (`push` for `std::stack` or
/// `push_back` for `std::list`). This struct is instrumental to providing compile-time stemming, making it possible to
/// call a method that fits the purpose without knowing its exact signature in advance.
///
/// @tparam TsOverloads Overloads. Those are being tried to apply until the first match, or until the set is exausted.
/// In the latter case, the call() method will evaluate to the SFINAE fallback (see `CallFamilyImpl::CallFamily<Marker>`
/// and `CallFamily::DefaultOverload`).
///
template <class ...TsOverloads>
struct CallFamily {
	///
	/// @brief Tries to call an instance's method with the given set of parameters.
	///
	template <class T, class ...Ta>
	static auto call(T &&aInstance, Ta &&...aArgs) -> decltype(CallFamilyImpl::CallFamily<typename CallFamilyImpl::MakeFullOverload<TsOverloads>..., CallFamilyImpl::Marker>::call(Trait::forward<T>(aInstance), Trait::forward<Ta>(aArgs)...))
	{
		return CallFamilyImpl::CallFamily<typename CallFamilyImpl::MakeFullOverload<TsOverloads>..., CallFamilyImpl::Marker>::call(Trait::forward<T>(aInstance), Trait::forward<Ta>(aArgs)...);
	}
};

template <class ...TsOverloads>
struct CanCallFamily {
	template <class T, class ...Ta>
	static constexpr bool check(T &&aInstance, Ta &&...aArgs)
	{
		using RetType = decltype(CallFamily<TsOverloads...>::call(Trait::forward<T>(aInstance), Trait::forward<Ta>(aArgs)...));
		return !Trait::IsSame<Rr::Refl::NoMember, RetType>::value;
	}
};

}  // namespace Refl
}  // namespace Rr
