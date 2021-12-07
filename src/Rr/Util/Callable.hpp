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

struct MockSizeof {
	struct Virtual {
		virtual void mock() = 0;  // Virtualness may increase "sizeof" of method pointers in some compilers
	};
	void (Virtual::*callback)();
	Virtual *instance;
};

}  // namespace CallableImpl

struct Mock {};

template <class...>
struct Callable;

// non-const Callable

template <class Tr, class ...Ta>
class Callable<Tr(Ta...)> {
	char memory[sizeof(CallableImpl::MockSizeof)];
	Mock *instance;
	Tr (Mock::*callback)(Ta...);

public:
	Tr operator()(Ta ...aArgs)
	{
		return (instance->*callback)(aArgs...);
	}

	Callable(Tr (*aCallback)(Ta...))
	{
		instance = reinterpret_cast<decltype(instance)>(new (memory) typename Rr::Trait::Fn<Tr(Ta...)>::CallableType{aCallback});
		callback = reinterpret_cast<decltype(callback)>(&Rr::Trait::Fn<Tr(Ta...)>::CallableType::operator());
	}

	template <class Ti>
	Callable(Tr (Ti::*aCallback)(Ta...), Ti *aInstance)
	{
		// The type a method pointer pertains to is not important
		instance = reinterpret_cast<decltype(instance)>(new (memory) typename Rr::Trait::Fn<Ti, Tr(Ta...)>::CallableType{aCallback, aInstance});
		callback = reinterpret_cast<decltype(callback)>(&Rr::Trait::Fn<Ti, Tr(Ta...)>::CallableType::operator());
	}
};

// const Callable

template <class Tr, class ...Ta>
class Callable<Tr(Ta...) const> {
	char memory[sizeof(CallableImpl::MockSizeof)];
	const Mock *instance;
	Tr (Mock::*callback)(Ta...)const;

public:
	Tr operator()(Ta ...aArgs)
	{
		return (instance->*callback)(aArgs...);
	}

	Callable(Tr (*aCallback)(Ta...))
	{
		instance = reinterpret_cast<decltype(instance)>(new (memory) typename Rr::Trait::Fn<Tr(Ta...)const>::CallableType{aCallback});
		callback = reinterpret_cast<decltype(callback)>(&Rr::Trait::Fn<Tr(Ta...)>::CallableType::operator());
	}

	template <class Ti>
	Callable(Tr (Ti::*aCallback)(Ta...)const, const Ti *aInstance)
	{
		// The type a method pointer pertains to is not important
		instance = reinterpret_cast<decltype(instance)>(new (memory) typename Rr::Trait::Fn<const Ti, Tr(Ta...) const>::CallableType{aCallback, aInstance});
		callback = reinterpret_cast<decltype(callback)>(&Rr::Trait::Fn<const Ti, Tr(Ta...) const>::CallableType::operator());
	}
};

}  // namespace Util
}  // namespace Rrq

#endif // RR_UTIL_CALLABLE_HPP
