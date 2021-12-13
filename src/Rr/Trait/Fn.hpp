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

#if RRO_STATIC_CAST_FN_CONVERSION
# define rr_fn_cast static_cast
#else
# define rr_fn_cast reinterpret_cast
#endif

namespace Trait {

template <class Tret, class Ti, class Trr, class TiCb, class TrrCb, class ...Ta>
struct FnImplMember : RrObject {
	using CallbackType = TiCb;
	using ReturnType = Tret;
	using InstanceType = Ti;

	Trr *instance;
	TrrCb callback;

	Tret operator()(Ta... aArgs)
	{
		return (instance->*callback)(aArgs...);
	}

	Tret operator()(Ta... aArgs) const
	{
		return (instance->*callback)(aArgs...);
	}

	template <class TanyCallback, class TanyInstance>
	FnImplMember(TanyCallback aCallback, TanyInstance aInstance):
		instance(rr_fn_cast<decltype(instance)>(aInstance)),
		callback(rr_fn_cast<decltype(callback)>(aCallback))
	{
	}
};

template <class Treturn, class ...Ta>
struct FnImplStatic : RrObject {
	using CallbackType = Treturn(*)(Ta...);
	using ReturnType = Treturn;

	CallbackType callback;

	Treturn operator()(Ta ...aArgs)
	{
		return callback(aArgs...);
	}

	Treturn operator()(Ta ...aArgs) const
	{
		return callback(aArgs...);
	}

	FnImplStatic(Treturn (*aCallback)(Ta...)): callback(aCallback)
	{
	}
};

template <class ...T>
struct Fn;

template<class T, class Tret, class ...Targs>
struct Fn<T, Tret(Targs...)> :
	FnImplMember<Tret, T, RrObject, Tret(T::*)(Targs...), Tret(RrObject::*)(Targs...), Targs...>
{
	using BaseType = FnImplMember<Tret, T, RrObject, Tret(T::*)(Targs...), Tret(RrObject::*)(Targs...), Targs...>;
	using BaseType::BaseType;
	using BaseType::operator();

	using BaseType::CallbackType;
	using BaseType::InstanceType;
	using BaseType::ReturnType;

	static constexpr auto kIsConst = false;
};

template<class T, class Tret, class ...Targs>
struct Fn<const T, Tret(Targs...)const> :
	FnImplMember<Tret, const T, const RrObject, Tret(T::*)(Targs...)const, Tret(RrObject::*)(Targs...)const, Targs...>
{
	using BaseType = FnImplMember<Tret, const T, const RrObject, Tret(T::*)(Targs...)const, Tret(RrObject::*)(Targs...)const, Targs...>;
	using BaseType::BaseType;
	using BaseType::operator();

	using BaseType::CallbackType;
	using BaseType::InstanceType;
	using BaseType::ReturnType;

	static constexpr auto kIsConst = true;
};

template<class Tret, class ...Targs>
struct Fn<Tret(Targs...)> : FnImplStatic<Tret, Targs...> {
	using BaseType = FnImplStatic<Tret, Targs...>;
	using BaseType::BaseType;
	using BaseType::operator();

	using BaseType::CallbackType;
	using BaseType::ReturnType;

	static constexpr auto kIsConst = false;
};

template<class Tret, class ...Targs>
struct Fn<Tret(Targs...)const> : FnImplStatic<Tret, Targs...> {
	using BaseType = FnImplStatic<Tret, Targs...>;
	using BaseType::BaseType;
	using BaseType::operator();

	using BaseType::CallbackType;
	using BaseType::ReturnType;

	static constexpr auto kIsConst = true;
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_FN_HPP
