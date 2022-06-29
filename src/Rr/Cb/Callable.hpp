//
// Callable.hpp
//
// Created: 2021-12-06
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_CALLABLE_HPP)
#define RR_UTIL_CALLABLE_HPP

#include <Rr/Util/DefaultConfig.hpp>
#include <Rr/Trait/Max.hpp>
#include <Rr/Trait/MemberDecay.hpp>
#include <Rr/Trait/RemoveReference.hpp>
#include <Rr/Trait/Forward.hpp>
#include <Rr/Trait/IsSame.hpp>
#include <Rr/Trait/EnableIf.hpp>

namespace Rr {
namespace Cb {

namespace CallableImpl {

template <class...>
struct CallWrap;

template <class...>
struct CallMember;

template <class ...>
struct CallStatic;

template <class...>
struct CallVariant;

template <class Tsignature, class ...Targs, template <class ...> class TtArglist>
struct CallWrap<Tsignature, TtArglist<Targs...>> {
	using CallerType = typename Trait::MemberDecay<Tsignature>::ReturnType(*)(
		CallWrap<Tsignature, TtArglist<Targs...>> */* Cb */, Targs... aArgs);  ///< Type erasure-based caller

	CallerType caller;
};

template <class Tsignature, class ...Targs, template <class ...> class TtArglist>
struct CallStatic<Tsignature, TtArglist<Targs...>> : CallWrap<Tsignature, TtArglist<Targs...>> {
	typename Trait::MemberDecay<Tsignature>::ReturnType(*staticCaller)(Targs ...aArgs);

	static typename Trait::MemberDecay<Tsignature>::ReturnType call(
		CallWrap<Tsignature, TtArglist<Targs...>> *aInstance, Targs...aArgs)
	{
		auto *thisInstance = static_cast<CallStatic<Tsignature, TtArglist<Targs...>> *>(aInstance);
		return thisInstance->staticCaller(aArgs...);
	}

	void init(decltype(staticCaller) aCaller)
	{
		staticCaller = aCaller;
		this->caller = call;
	}
};

template <class Tsignature, class ...Targs, template <class ...> class TtArglist>
struct CallMember<Tsignature, TtArglist<Targs...>> : CallWrap<Tsignature, TtArglist<Targs...>> {

	/// \brief Helper stub that enables assessing how much of a storage a pointer to a virtual member takes
	///
	struct Stub {
		virtual unsigned call(int,int,int,int)
		{
			return 0;
		}
	};

	/// \brief Stores a method and instance pointers
	///
	template <class Tptr, class Tinst>
	struct Cb {
		Tptr ptr;  ///< Pointer to a method
		Tinst inst;  ///< Pointer to an instance
	};

	/// \brief Template stub for creating callers at compile time
	///
	template <class Tptr, class Tinst>
	static typename Trait::MemberDecay<Tsignature>::ReturnType call(
		CallWrap<Tsignature, TtArglist<Targs...>> *aInstance, Targs...aArgs)
	{
		auto *thisInstance = static_cast<CallMember<Tsignature, TtArglist<Targs...>> *>(aInstance);
		auto &cb = *reinterpret_cast<Cb<Tptr, Tinst> *>(thisInstance->cell);
		return (cb.inst->*cb.ptr)(aArgs...);
	}

	/// \brief Initialize `cell` and `caller`
	///
	template <class Tptr, class Tinst>
	void init(Tptr aPtr, Tinst aInst)
	{
		this->caller = call<Tptr, Tinst>;
		new (cell, Rr::Object()) Cb<Tptr, Tinst>{aPtr, aInst};
	}

	alignas(decltype(sizeof(int *))) unsigned char cell[sizeof(Cb<decltype(&Stub::call), Stub>)];  ///< Polymorphic cell storing call information
};

template <class Tsignature, class ...Targs, template <class ...> class TtArglist>
struct CallVariant<Tsignature, TtArglist<Targs...>> {
	union Variant {
		CallMember<Tsignature, TtArglist<Targs...>> callMember;
		CallStatic<Tsignature, TtArglist<Targs...>> callStatic;
	};
	alignas(decltype(sizeof(int *))) unsigned char variant[sizeof(Variant)];

	typename Trait::MemberDecay<Tsignature>::ReturnType operator()(Targs ...aArgs)
	{
		auto *callWrap = reinterpret_cast<CallWrap<Tsignature, TtArglist<Targs...>> *>(variant);
		return (callWrap->caller)(callWrap, aArgs...);
	}

	CallVariant(typename Trait::MemberDecay<Tsignature>::CallbackType aCallback)
	{
		auto callStatic = new (variant, Rr::Object{}) CallStatic<Tsignature, TtArglist<Targs...>>();
		callStatic->init(aCallback);
	}

	template<class Tinstance>
	CallVariant(typename Trait::MemberDecay<Tsignature, Trait::Stript<Tinstance>>::CallbackType aCallback, Tinstance aInstance)
	{
		auto callMember = new (variant, Rr::Object{}) CallMember<Tsignature, TtArglist<Targs...>>();
		callMember->init(aCallback, aInstance);
	}
};

}  // namespace CallableImpl

template <class Tsignature>
using Callable = typename CallableImpl::CallVariant<Tsignature, typename Trait::MemberDecay<Tsignature>::ArgsList>;

}  // namespace Cb
}  // namespace Rr

#endif // RR_UTIL_CALLABLE_HPP
