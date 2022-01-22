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

struct Marker;

enum class Hint {
	Member,
	Static,
	StaticNoArg,
};

template <class ...Ts>
constexpr Hint getStaticHint()
{
	return sizeof...(Ts) > 0 ? Hint::Static : Hint::StaticNoArg;
}

template <Hint I>
struct Fallback {
	static constexpr NoMember call(...)
	{
		return {};
	}
};

template <>
struct Fallback<Hint::Static> {
	///
	/// @brief When choosing b/w `fn(T ...)` and `fn(...)`, the compiler assigns the latter a lower priority
	///
	template <class T>
	static constexpr NoMember call(...)
	{
		return {};
	}
};

template <>
struct Fallback<Hint::StaticNoArg> {
	///
	/// @brief When chossing b/w `fn(void)` and `void(T...)`, the compiler assigns the latter a lower priority.
	///
	template <class T, class ...Ts>
	static constexpr NoMember call(Ts &&...)
	{
		return {};
	}
};

template <Hint I, class Tso, class ...TsOs>
struct Impl {
	template <class T, class ...Ta>
	static auto call(Ta &&...aArgs) -> decltype(
		Trait::ConditionalTp<
			!IsNoMember<
			decltype(Tso::template call<T>(Trait::forward<Ta>(aArgs)...))>
			::value,
			Tso,
			Impl<I, TsOs...>>
			::template call<T>(Trait::forward<Ta>(aArgs)...)
	)
	{
		return Trait::ConditionalTp<
			// if overload is defined for this candidate
			!IsNoMember<
			decltype(Tso::template call<T>(Trait::forward<Ta>(aArgs)...))>  // return type of the candidate
			::value,
			// then use the candidate
			Tso,
			// else try other candidate
			Impl<I, TsOs...>>
			// endif
			// call upon inferred type
			::template call<T>(Trait::forward<Ta>(aArgs)...);
	}
};

template <class Tso, class ...TsOs>
struct Impl<Hint::Member, Tso, TsOs...> {
	template <class ...Ta>
	static auto call(Ta &&...aArgs) -> decltype(
		Trait::ConditionalTp<
			!IsNoMember<
			decltype(Tso::call(Trait::forward<Ta>(aArgs)...))>
			::value,
			Tso,
			Impl<Hint::Member, TsOs...>>
			::call(Trait::forward<Ta>(aArgs)...)
	)
	{
		return Trait::ConditionalTp<
			// if overload is defined for this candidate
			!IsNoMember<
			decltype(Tso::call(Trait::forward<Ta>(aArgs)...))>  // return type of the candidate
			::value,
			// then use the candidate
			Tso,
			// else try other candidate
			Impl<Hint::Member, TsOs...>>
			// endif
			// call upon inferred type
			::call(Trait::forward<Ta>(aArgs)...);
	}
};

template <Hint I>
struct Impl<I, Marker> : Fallback<I> {
};

template <>
struct Impl<Hint::Member, Marker> : Fallback<Hint::Member> {
};

template <Hint I, class T>
struct MakeFullOverload : T, Fallback<I> {
	using T::call;
	using Fallback<I>::call;
};

}  // namespace CallFamilyImpl

///
/// @brief Tries to call a certain method with a certain set of arguments. For usage example, please refer to
/// <Test/Refl/CallFamily.cpp>.
///
/// Motivation: certain containers may provide similar functionality (methods) under diffirent names (`push` for
/// `std::stack` or `push_back` for `std::list`). This set of methods is referred to as "family" here.
///
/// This struct is instrumental to providing compile-time stemming, making it possible to call a method that fits the
/// purpose without knowing its exact signature in advance.
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
	static auto call(T &&arg, Ta &&...aArgs) ->
		decltype(CallFamilyImpl::Impl<CallFamilyImpl::Hint::Member,
		typename CallFamilyImpl::MakeFullOverload<CallFamilyImpl::Hint::Member, TsOverloads>...,
		CallFamilyImpl::Marker>::call(Trait::forward<T>(arg), Trait::forward<Ta>(aArgs)...))
	{
		return CallFamilyImpl::Impl<CallFamilyImpl::Hint::Member,
			typename CallFamilyImpl::MakeFullOverload<CallFamilyImpl::Hint::Member, TsOverloads>...,
			CallFamilyImpl::Marker>::call(Trait::forward<T>(arg), Trait::forward<Ta>(aArgs)...);
	}

	template <class T, class ...Ta>
	static auto call(Ta &&...aArgs) -> decltype(CallFamilyImpl::Impl<CallFamilyImpl::getStaticHint<Ta...>(),
	typename CallFamilyImpl::MakeFullOverload<CallFamilyImpl::getStaticHint<Ta...>(), TsOverloads>...,
	CallFamilyImpl::Marker>::template call<T>(Trait::forward<Ta>(aArgs)...))
	{
		return CallFamilyImpl::Impl<CallFamilyImpl::getStaticHint<Ta...>(),
			typename CallFamilyImpl::MakeFullOverload<CallFamilyImpl::getStaticHint<Ta...>(), TsOverloads>...,
			CallFamilyImpl::Marker>::template call<T>(Trait::forward<Ta>(aArgs)...);
	}
};

template <class ...TsOverloads>
struct CanCallFamily {
	template <class T, class ...Ta>
	static constexpr bool check(T &&arg, Ta &&...aArgs)
	{
		using RetType = decltype(CallFamily<TsOverloads...>::call(Trait::forward<T>(arg), Trait::forward<Ta>(aArgs)...));
		return !IsNoMember<RetType>::value;
	}

	template <class T, class ...Ta>
	static constexpr bool check(Ta &&...aArgs)
	{
		using RetType = decltype(CallFamily<TsOverloads...>::template call<T>(Trait::forward<Ta>(aArgs)...));
		return !IsNoMember<RetType>::value;
	}
};

}  // namespace Refl
}  // namespace Rr
