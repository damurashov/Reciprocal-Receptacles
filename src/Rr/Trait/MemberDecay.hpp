//
// MemberDecay.hpp
//
// Created on: 2021-12-26
//     Author: dmtr <DOT> murashov <AT> <GMAIL COM>
//

#if !defined(RR_TRAIT_MEMBERDECAY_HPP)
#define RR_TRAIT_MEMBERDECAY_HPP

#include <Rr/Trait/AddCvpref.hpp>
#include <Rr/Trait/StoreType.hpp>
#include <Rr/Trait/RemoveReference.hpp>
#include <Rr/Trait/IsSame.hpp>
#include <Rr/Trait/IntegralConstant.hpp>
#include <Rr/Trait/Conditional.hpp>

namespace Rr {
namespace Trait {

namespace MemberDecayImpl {

struct Stub;

template <class ...Targs>
struct ArgsListImpl;

template <class T, bool Fconst, bool Fvolatile>
using Cv = AddCvpreft<Stript<T>, Fconst, Fvolatile, false, false, false>;  ///< Strip type, add const and volatile qualifiers depending on template args arguments

template <class Tret, class Tcb, class Ti, bool Fconst, bool Fvolatile, bool Fptrform, class ...Targs>
struct MemberDecayStore {
	using StaticCallbackType = Tret(*)(Targs...);
	using CallbackType = Tcb;
	using InstanceType = Cv<Ti, Fconst, Fvolatile>;
	using VoidType = Cv<Ti, Fconst, Fvolatile>;  ///< Useful, because it preserves CV-qualifiers
	using ReturnType = Tret;
	using ArgsList = ArgsListImpl<Targs...>;

	static constexpr auto kIsConst = Fconst;
	static constexpr auto kIsVolatile = Fvolatile;
	static constexpr auto kIsMethod = IsSame<Stub, Ti>::value;
	static constexpr auto kIsPointerForm = Fptrform;  ///< The signature has form `Ret(Type::*)(Args...)` or `Ret(*)(Args...)`

	using AsMemberCallback = Tret(Ti::*)(Targs...);
	using AsConstMemberCallback = Tret(Ti::*)(Targs...) const;
	using AsVolatileMemberCallback = Tret(Ti::*)(Targs...) volatile;
	using AsCvMmberCallback = Tret(Ti::*)(Targs...) volatile;

	using AsRefMemberCallback = Tret(Ti::*)(Targs...) &;
	using AsRefConstMemberCallback = Tret(Ti::*)(Targs...) const&;
	using AsRefVolatileMemberCallback = Tret(Ti::*)(Targs...) volatile&;
	using AsRefCvMmberCallback = Tret(Ti::*)(Targs...) volatile&;

	using AsRrefMemberCallback = Tret(Ti::*)(Targs...) &&;
	using AsRrefConstMemberCallback = Tret(Ti::*)(Targs...) const&&;
	using AsRrefVolatileMemberCallback = Tret(Ti::*)(Targs...) volatile&&;
	using AsRrefCvMmberCallback = Tret(Ti::*)(Targs...) volatile&&;
};

template<class ...Ta> struct MemberDecay;
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(Args...)                  , T> : MemberDecayStore<Ret, Ret(T::*)(Args...)                  , T, false, false, false, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(Args...) const            , T> : MemberDecayStore<Ret, Ret(T::*)(Args...) const            , T, true , false, false, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(Args...) volatile         , T> : MemberDecayStore<Ret, Ret(T::*)(Args...) volatile         , T, false, true , false, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(Args...) const volatile   , T> : MemberDecayStore<Ret, Ret(T::*)(Args...) const volatile   , T, true , true , false, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(Args...)                & , T> : MemberDecayStore<Ret, Ret(T::*)(Args...)                & , T, false, false, false, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(Args...) const          & , T> : MemberDecayStore<Ret, Ret(T::*)(Args...) const          & , T, true , false, false, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(Args...) volatile       & , T> : MemberDecayStore<Ret, Ret(T::*)(Args...) volatile       & , T, false, true , false, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(Args...) const volatile & , T> : MemberDecayStore<Ret, Ret(T::*)(Args...) const volatile & , T, true , true , false, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(Args...)                &&, T> : MemberDecayStore<Ret, Ret(T::*)(Args...)                &&, T, false, false, false, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(Args...) const          &&, T> : MemberDecayStore<Ret, Ret(T::*)(Args...) const          &&, T, true , false, false, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(Args...) volatile       &&, T> : MemberDecayStore<Ret, Ret(T::*)(Args...) volatile       &&, T, false, true , false, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(Args...) const volatile &&, T> : MemberDecayStore<Ret, Ret(T::*)(Args...) const volatile &&, T, true , true , false, Args...> {};

template<class T, class Ret, class ...Args> struct MemberDecay<Ret(T::*)(Args...)                  > : MemberDecayStore<Ret, Ret(T::*)(Args...)                  , T, false, false, true, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(T::*)(Args...) const            > : MemberDecayStore<Ret, Ret(T::*)(Args...) const            , T, true , false, true, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(T::*)(Args...) volatile         > : MemberDecayStore<Ret, Ret(T::*)(Args...) volatile         , T, false, true , true, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(T::*)(Args...) const volatile   > : MemberDecayStore<Ret, Ret(T::*)(Args...) const volatile   , T, true , true , true, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(T::*)(Args...)                & > : MemberDecayStore<Ret, Ret(T::*)(Args...)                & , T, false, false, true, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(T::*)(Args...) const          & > : MemberDecayStore<Ret, Ret(T::*)(Args...) const          & , T, true , false, true, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(T::*)(Args...) volatile       & > : MemberDecayStore<Ret, Ret(T::*)(Args...) volatile       & , T, false, true , true, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(T::*)(Args...) const volatile & > : MemberDecayStore<Ret, Ret(T::*)(Args...) const volatile & , T, true , true , true, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(T::*)(Args...)                &&> : MemberDecayStore<Ret, Ret(T::*)(Args...)                &&, T, false, false, true, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(T::*)(Args...) const          &&> : MemberDecayStore<Ret, Ret(T::*)(Args...) const          &&, T, true , false, true, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(T::*)(Args...) volatile       &&> : MemberDecayStore<Ret, Ret(T::*)(Args...) volatile       &&, T, false, true , true, Args...> {};
template<class T, class Ret, class ...Args> struct MemberDecay<Ret(T::*)(Args...) const volatile &&> : MemberDecayStore<Ret, Ret(T::*)(Args...) const volatile &&, T, true , true , true, Args...> {};

template<class Ret, class ...Args> struct MemberDecay<Ret(*)(Args...)> : MemberDecayStore<Ret, Ret(*)(Args...), Stub, false, false, true, Args...> {};

template<class Ret, class ...Args> struct MemberDecay<Ret(Args...)                  > : MemberDecayStore<Ret, Ret(*)(Args...), Stub, false, false, false, Args...> {};
template<class Ret, class ...Args> struct MemberDecay<Ret(Args...) const            > : MemberDecayStore<Ret, Ret(*)(Args...), Stub, true , false, false, Args...> {};
template<class Ret, class ...Args> struct MemberDecay<Ret(Args...) volatile         > : MemberDecayStore<Ret, Ret(*)(Args...), Stub, false, true , false, Args...> {};
template<class Ret, class ...Args> struct MemberDecay<Ret(Args...) const volatile   > : MemberDecayStore<Ret, Ret(*)(Args...), Stub, true , true , false, Args...> {};
template<class Ret, class ...Args> struct MemberDecay<Ret(Args...)                & > : MemberDecayStore<Ret, Ret(*)(Args...), Stub, false, false, false, Args...> {};
template<class Ret, class ...Args> struct MemberDecay<Ret(Args...) const          & > : MemberDecayStore<Ret, Ret(*)(Args...), Stub, true , false, false, Args...> {};
template<class Ret, class ...Args> struct MemberDecay<Ret(Args...) volatile       & > : MemberDecayStore<Ret, Ret(*)(Args...), Stub, false, true , false, Args...> {};
template<class Ret, class ...Args> struct MemberDecay<Ret(Args...) const volatile & > : MemberDecayStore<Ret, Ret(*)(Args...), Stub, true , true , false, Args...> {};
template<class Ret, class ...Args> struct MemberDecay<Ret(Args...)                &&> : MemberDecayStore<Ret, Ret(*)(Args...), Stub, false, false, false, Args...> {};
template<class Ret, class ...Args> struct MemberDecay<Ret(Args...) const          &&> : MemberDecayStore<Ret, Ret(*)(Args...), Stub, true , false, false, Args...> {};
template<class Ret, class ...Args> struct MemberDecay<Ret(Args...) volatile       &&> : MemberDecayStore<Ret, Ret(*)(Args...), Stub, false, true , false, Args...> {};
template<class Ret, class ...Args> struct MemberDecay<Ret(Args...) const volatile &&> : MemberDecayStore<Ret, Ret(*)(Args...), Stub, true , true , false, Args...> {};


}  // namespace MemberDecayImpl

using MemberDecayImpl::MemberDecay;

}  // Trait
}  // Rr


#endif // RR_TRAIT_MEMBERDECAY_HPP
