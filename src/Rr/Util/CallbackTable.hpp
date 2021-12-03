//
// Callback.hpp
//
// Created: 2021-12-03
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_CALLBACK_HPP)
#define RR_UTIL_CALLBACK_HPP

namespace Rr {
namespace Util {

///
/// @brief Dynamic wrapper over callable instances
///
template <class Tret, class ...Targs>
struct Callable {
	using result_type = Tret;
	virtual Tret operator()(Targs...) = 0;
	virtual ~Callable() = default;
};

///
/// @brief Dynamic wrapper over member function pointers
///
template <class Tret, class T, class ...Targs>
struct MemberCallable : public Callable <Tret, Targs...>{
	using CallbackType = Tret(T::*)(Targs...);

	Tret operator()(Targs...) override;
private:
	CallbackType cb;
	T *instance;
};

///
/// @brief Dynamic wrapper over static function pointers
///
template <class Tret, class ...Targs>
struct StaticCallable : public Callable <Tret, Targs...> {
	using CallbackType = Tret(*)(Targs...);

	Tret operator()(Targs...) override;
private:
	CallbackType cb;
};

///
/// @brief Enables one to cast a member function pointer omitting the owner type
///
template <class Tto, class Tret, class Tfrom, class ...Targs>
typename MemberCallable<Tret, Tto, Targs...>::CallbackType
memberCast(Tret(Tfrom::*)(Targs...));

template <class T, class Tret, class ...Targs>
struct Fn;

template<class T, class Tret, class ...Targs>
struct Fn<T, Tret(Targs...)> {
	using Type = Tret(T::*)(Targs...);
	using return_type = Tret;
	using InstanceType = T;
};

template<class Tret, class ...Targs>
struct Fn<void, Tret(Targs...)> {
	using Type = Tret(*)(Targs...);
	using ReturnType = Tret;
	using InstanceType = void;
};

///
/// @brief
///
/// @tparam Tret   Callable return type
/// @tparam Targs  Argument types
/// @tparam Itopic An integral value to differentiate similar callables
/// @tparam Tlist  A container w/ support for ::begin, ::end, and ::push_back
///
template <class Tfn, class Tcontainer>
class CallbackTable {
private:
	// Tcontainer
};

}  // namespace Util
}  // namespace Rr

#include "CallbackTable.impl"

#endif // RR_UTIL_CALLBACK_HPP
