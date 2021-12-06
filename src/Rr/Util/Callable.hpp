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

}  // namespace CallableImpl

template <typename ...T>
struct Callable;

template <typename Tret, typename ...Targs>
class Callable<Tret(Targs...)> {
	// Wrapper over CallbackImpl::Callback instance
private:
	using Fn = Rr::Trait::Fn<CallableImpl::Mock, Tret(Targs...)>;

	struct {
		typename Fn::CallbackType callback;  /// Instead of Tret(Tinstance::*)(Targs...), we use Tret(Mock::*)(Targs...), because the underlying call procedure is the same regardless of a class implementation
		typename Fn::InstanceType *instance;  /// Instead of a pointer to a particular instance, we use Mock*, because the underlying call procedure is the same regardless of a class implementation
	} callable;

	// Memory to store CallbackImpl::Callback instance
private:
	static constexpr auto kMemSize = Rr::Trait::MaxSizeof<typename CallableImpl::Callable<CallableImpl::Mock, Tret(Targs...)>,
		typename CallableImpl::Callable<Tret(Targs...)>>::value;  // TODO: Some compilers may differ in their implementation regarding whether or not member function pointer is a pointer to a virtual function
	char memory[kMemSize];  /// Storage for whatever instance we create

public:
	Callable(Tret(*)(Targs...));

	template <typename Tinstance>
	Callable(Tret(Tinstance::*)(Targs...), Tinstance*);

	Tret operator()(Targs...);
};

}  // namespace Util
}  // namespace Rr

#include "Callable.impl"

#endif // RR_UTIL_CALLABLE_HPP
