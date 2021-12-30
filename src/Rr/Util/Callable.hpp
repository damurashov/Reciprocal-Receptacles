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

namespace Rr {
namespace Util {

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
	typename Trait::MemberDecay<Tsignature, Rr::Object>::CallbackType memberFunctionCallback;
	typename Trait::MemberDecay<Tsignature, Rr::Object>::InstanceType *instance;

	typename Trait::MemberDecay<Tsignature>::ReturnType
	operator()(Targs ...aArgs) override
	{
		return (instance->*memberFunctionCallback)(aArgs...);
	}

	virtual ~CallMember() = default;
};

template <class Tsignature, class ...Targs, template <class ...> class TtArglist>
struct CallVariant<Tsignature, TtArglist<Targs...>> : CallWrap<Tsignature, TtArglist<Targs...>> {
	union Variant {
		CallMember<Tsignature, TtArglist<Targs...>> callMember;
		CallStatic<Tsignature, TtArglist<Targs...>> callStatic;
	};
	unsigned char variant[sizeof(Variant)];

	typename Trait::MemberDecay<Tsignature>::ReturnType
	operator()(Targs ...aArgs) override
	{
		return (*reinterpret_cast<CallWrap<Tsignature, TtArglist<Targs...>> *>(variant))(aArgs...);
	}

	CallVariant(typename Trait::MemberDecay<Tsignature>::CallbackType aCallback)
	{
		auto callStatic = new (variant) CallStatic<Tsignature, TtArglist<Targs...>>();
		callStatic->staticFunctionCallback = aCallback;
	}

	template<class Tinstance>
	CallVariant(typename Trait::MemberDecay<Tsignature, Trait::Stript<Tinstance>>::CallbackType aCallback, Tinstance aInstance)
	{
		auto callMember = new (variant) CallMember<Tsignature, TtArglist<Targs...>>();
		callMember->memberFunctionCallback = rr_fn_cast<typename Trait::MemberDecay<Tsignature, Rr::Object>::CallbackType>(aCallback);
		callMember->instance = rr_fn_cast<typename Trait::MemberDecay<Tsignature, Rr::Object>::InstanceType *>(aInstance);
	}
};

}  // namespace CallableImpl

template <class Tsignature>
using Callable = typename CallableImpl::CallVariant<Tsignature, typename Trait::MemberDecay<Tsignature>::ArgsList>;

}  // namespace Util
}  // namespace Rr

#endif // RR_UTIL_CALLABLE_HPP
