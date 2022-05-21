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

template <class...>
struct CallStatic;

template <class...>
struct CallVariant;

template <class Tsignature, class ...Targs, template <class ...> class TtArglist>
struct CallWrap<Tsignature, TtArglist<Targs...>> {
	virtual typename Trait::MemberDecay<Tsignature>::ReturnType
	operator()(Targs ...) = 0;
	virtual ~CallWrap() = default;
};

template <class Tsignature, class ...Targs, template <class ...> class TtArglist>
struct CallStatic<Tsignature, TtArglist<Targs...>> : CallWrap<Tsignature, TtArglist<Targs...>> {
	typename Trait::MemberDecay<Tsignature>::CallbackType staticFunctionCallback;

	typename Trait::MemberDecay<Tsignature>::ReturnType
	operator()(Targs ...aArgs) override
	{
		return staticFunctionCallback(aArgs...);
	}

	virtual ~CallStatic() = default;
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

	using CallerType = typename Trait::MemberDecay<Tsignature>::ReturnType(*)(void */* Cb */, Targs... aArgs);  ///< Type erasure-based caller

	/// \brief Template stub for creating callers at compile time
	///
	template <class Tptr, class Tinst>
	static typename Trait::MemberDecay<Tsignature>::ReturnType tcaller(void *aCb, Targs ...aArgs)
	{
		auto &cb = *reinterpret_cast<Cb<Tptr, Tinst> *>(aCb);
		return (cb.inst->*cb.ptr)(aArgs...);
	}

	/// \brief Initialize `cell` and `caller`
	///
	template <class Tptr, class Tinst>
	void init(Tptr aPtr, Tinst aInst)
	{
		caller = tcaller<Tptr, Tinst>;
		new (cell, Rr::Object()) Cb<Tptr, Tinst>{aPtr, aInst};
	}

	/// \brief Invoke the caller
	///
	typename Trait::MemberDecay<Tsignature>::ReturnType
	operator()(Targs ...aArgs) override
	{
		return caller(cell, aArgs...);
	}

	virtual ~CallMember() = default;

	alignas(decltype(sizeof(int))) unsigned char cell[sizeof(Cb<decltype(&Stub::call), Stub>)];  ///< Polymorphic cell storing call information
	CallerType caller;  ///< Type erasure-based caller
};

template <class Tsignature, class ...Targs, template <class ...> class TtArglist>
struct CallVariant<Tsignature, TtArglist<Targs...>> : CallWrap<Tsignature, TtArglist<Targs...>> {
	union Variant {
		CallMember<Tsignature, TtArglist<Targs...>> callMember;
		CallStatic<Tsignature, TtArglist<Targs...>> callStatic;
	};
	alignas(Variant) unsigned char variant[sizeof(Variant)];

	typename Trait::MemberDecay<Tsignature>::ReturnType
	operator()(Targs ...aArgs) override
	{
		return (*reinterpret_cast<CallWrap<Tsignature, TtArglist<Targs...>> *>(variant))(aArgs...);
	}

	CallVariant(typename Trait::MemberDecay<Tsignature>::CallbackType aCallback)
	{
		auto callStatic = new (variant, Rr::Object{}) CallStatic<Tsignature, TtArglist<Targs...>>();
		callStatic->staticFunctionCallback = aCallback;
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
