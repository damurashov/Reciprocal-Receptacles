//
// Callable.hpp
//
// Created: 2021-12-06
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_CALLABLE_HPP)
#define RR_UTIL_CALLABLE_HPP

#include <Rr/Trait/Fn.hpp>
#include <Rr/Trait/Max.hpp>

namespace Rr {
namespace Util {
namespace CallableImpl {

template <class ...T>
struct Callable;

///
/// @brief  Static callable wrapper
///
template <class Tret, class ...Targs>
struct Callable<Tret(Targs...)> {
	Tret operator()(Targs...);

	Tret (*callback)(Targs...);
};

///
/// @brief Member callable wrapper
///
template <class Tinstance, class Tret, class ...Targs>
struct Callable<Tinstance, Tret(Targs...)> {
	Tret operator()(Targs...);

	Tret (Tinstance::*callback)(Targs...);
	Tinstance *instance;
};

///
/// @brief Mock structure
///
struct Mock {
};

template <typename Tsignature, typename Tret, typename ...Targs>
class CallableBase {
	// Wrapper over CallbackImpl::Callback instance
protected:
	using Fn = Rr::Trait::Fn<CallableImpl::Mock, Tsignature>;

	struct {
		typename Fn::CallbackType callback;  /// Instead of Tret(Tinstance::*)(Targs...), we use Tret(Mock::*)(Targs...), because the underlying call procedure is the same regardless of a class implementation
		typename Fn::InstanceType *instance;  /// Instead of a pointer to a particular instance, we use Mock*, because the underlying call procedure is the same regardless of a class implementation
	} callable;

	// Memory to store CallbackImpl::Callback instance
protected:
	static constexpr auto kMemSize = Rr::Trait::MaxSizeof<typename CallableImpl::Callable<CallableImpl::Mock, Tsignature>,
		typename CallableImpl::Callable<Tsignature>>::value;  // TODO: Some compilers may differ in their implementation regarding whether or not member function pointer is a pointer to a virtual function
	char memory[kMemSize];  /// Storage for whatever instance we create

	CallableBase() = default;

public:
	static constexpr bool kConst = Fn::kConst;
	Tret operator()(Targs...);

	CallableBase(Tret(*)(Targs...));
};

}  // namespace CallableImpl

template<class ...>
struct Callable;

template<class Tret, class ...Targs>
struct Callable<Tret(Targs...)> : CallableImpl::CallableBase<Tret(Targs...), Tret, Targs...> {
	using CallableImpl::CallableBase<Tret(Targs...), Tret, Targs...>::operator();
	using CallableImpl::CallableBase<Tret(Targs...), Tret, Targs...>::CallableBase;

	template <typename Tinstance>
	Callable(Tret(Tinstance::*)(Targs...), Tinstance*);
};

template<class Tret, class ...Targs>
struct Callable<Tret(Targs...) const> : CallableImpl::CallableBase<Tret(Targs...), Tret, Targs...> {
	using CallableImpl::CallableBase<Tret(Targs...), Tret, Targs...>::operator();
	using CallableImpl::CallableBase<Tret(Targs...), Tret, Targs...>::CallableBase;

	template <typename Tinstance>
	Callable(Tret(Tinstance::*)(Targs...) const, const Tinstance*);
};

}  // namespace Util
}  // namespace Rr

#include "Callable.impl"

#endif // RR_UTIL_CALLABLE_HPP
