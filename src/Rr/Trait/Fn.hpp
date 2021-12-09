//
// Fn.hpp
//
// Created: 2021-12-06
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_FN_HPP)
#define RR_TRAIT_FN_HPP

namespace Rr {

struct RrObject {};

namespace Trait {

template <class ...T>
struct Fn;

namespace FnImpl {
	template <class Tret, class Tcb, class ...Targs>
	struct StaticCallable : RrObject {
		Tcb callback;
		Tret operator()(Targs...aArgs) {
			return callback(aArgs...);
		}
	};

	template <class Tret, class Tinstance, class Tcb, class ...Targs>
	struct MemberCallable : RrObject {
		Tcb callback;
		Tinstance *instance;
		Tret operator()(Targs...aArgs) {
			return (instance->*callback)(aArgs...);
		}
	};
}  // namespace FnImpl

template<class T, class Tret, class ...Targs>
struct Fn<T, Tret(Targs...)> {
	using CallbackType = Tret(T::*)(Targs...);
	using ReturnType = Tret;
	using InstanceType = T;
	using CallableType = FnImpl::MemberCallable<ReturnType, InstanceType, CallbackType, Targs...>;
	static constexpr bool kConst = false;
};

template<class T, class Tret, class ...Targs>
struct Fn<T const, Tret(Targs...) const> {
	using CallbackType = Tret(T::*)(Targs...)const;
	using ReturnType = Tret;
	using InstanceType = const T;
	using CallableType = FnImpl::MemberCallable<ReturnType, InstanceType, CallbackType, Targs...>;
	static constexpr bool kConst = true;
};

template<class Tret, class ...Targs>
struct Fn<Tret(Targs...)> {
	using CallbackType = Tret(*)(Targs...);
	using ReturnType = Tret;
	using InstanceType = void;
	using CallableType = FnImpl::StaticCallable<ReturnType, CallbackType, Targs...>;
	static constexpr bool kConst = false;
};

template<class Tret, class ...Targs>
struct Fn<Tret(Targs...)const> {
	using CallbackType = Tret(*)(Targs...);
	using ReturnType = Tret;
	using InstanceType = void;
	using CallableType = FnImpl::StaticCallable<ReturnType, CallbackType, Targs...>;
	static constexpr bool kConst = true;
};

#if RRO_SAFE_FN_CONVERSION
template <class Tsignature, class TcallbackType>
typename Fn<Tsignature>::CallbackType memberCast(TcallbackType aCallback)
{
	return static_cast<typename Fn<RrObject, Tsignature>::CallbackType>(aCallback);
}
#else
template <class Tsignature, class TcallbackType>
typename Fn<Tsignature>::CallbackType memberCast(TcallbackType aCallback)
{
	return reinterpret_cast<typename Fn<RrObject, Tsignature>::CallbackType>(aCallback);
}
#endif

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_FN_HPP
