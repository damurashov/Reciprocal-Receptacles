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

struct Virtual {
	virtual void v() = 0;
	char mem[sizeof(&Virtual::v) + sizeof(Virtual *)];  // The struct we adjust to will contain a member function pointer, plus an instance pointer
};

// A shortcut for determining max sizeof() of stored types, which are Fn<SIGNATURE>::CallableType
template<class TrrObject, class Tsignature>
struct MaxSizeof {
	static constexpr auto value = Rr::Trait::MaxSizeof<
		Virtual,  // Some compilers may use a different size of pointers to virtual function members
		typename Rr::Trait::Fn<TrrObject, Tsignature>,  // Callable encapsulating instance pointer and method pointer
		typename Rr::Trait::Fn<Tsignature>>::value;  // Callable encapsulating static function pointer
};

///
/// @brief Polymorphic wrapper obfuscating underlying representation which may be either
///        static function pointer encapsulation, or that of member pointer
///
/// @tparam Tret Return type
/// @tparam Trr `RrObject` with cv-qualifiers
/// @tparam TrrCb `Tret(RrObject::*)(Ta...) with cv- and ref-qualifiers
/// @tparam Tsignature `void(int)`, `void(char, std::string) const &&`, etc.
/// @tparam Ta variadic parameters to the underlying callable
///
template <class Tret, class Trr, class TrrCb, class Tsignature, class ...Ta>
class CallableImpl {
protected:
	char memory[MaxSizeof<Trr, Tsignature>::value];
	Trr *instance;
	TrrCb callback;

public:
	Tret operator()(Ta ...aArgs)
	{
		return (instance->*callback)(aArgs...);
	}

	Tret operator()(Ta ...aArgs) const
	{
		return (instance->*callback)(aArgs...);
	}

	CallableImpl(Tret (*aCallback)(Ta...))
	{
		auto mem = new (memory) typename Rr::Trait::Fn<Tsignature>{aCallback};
		// CallableType has RrObject as its base, hence use of static_cast
		instance = static_cast<decltype(instance)>(mem);
		callback = static_cast<decltype(callback)>(&Rr::Trait::Fn<Tsignature>::operator());
	}

	template <class Tc, class Ti>
	CallableImpl(Tc aCallback, Ti *aInstance)
	{
		auto mem = new (memory) typename Rr::Trait::Fn<Trr, Tsignature>{aCallback, aInstance};
		instance = static_cast<decltype(instance)>(mem);
		callback = static_cast<decltype(callback)>(&Rr::Trait::Fn<Trr, Tsignature>::operator());
	}
};

}  // namespace CallableImpl

// non-const Callable

template<class Tsignature>
class Callable;

template <class Tret, class ...Ta>
class Callable<Tret(Ta...)> :
	public CallableImpl::CallableImpl<Tret, RrObject, Tret(RrObject::*)(Ta...), Tret(Ta...), Ta...>
{
public:
	using BaseType = typename CallableImpl::CallableImpl<Tret, RrObject, Tret(RrObject::*)(Ta...), Tret(Ta...), Ta...>;
	using BaseType::BaseType;
	using BaseType::operator();

	// To facilitate template arg. deduction
	template <class Ti>
	Callable(Tret(Ti::*aCallback)(Ta...), Ti *aInstance): BaseType(aCallback, aInstance) {}
};

// const Callable

template <class Tret, class ...Ta>
class Callable<Tret(Ta...)const> :
	public CallableImpl::CallableImpl<Tret, const RrObject, Tret(RrObject::*)(Ta...)const, Tret(Ta...)const, Ta...>
{
public:
	using BaseType = typename CallableImpl::CallableImpl<Tret, const RrObject, Tret(RrObject::*)(Ta...)const, Tret(Ta...)const, Ta...>;
	using BaseType::BaseType;
	using BaseType::operator();

	// To facilitate template arg. deduction
	template <class Ti>
	Callable(Tret(Ti::*aCallback)(Ta...)const, const Ti *aInstance): BaseType(aCallback, aInstance) {}
};

}  // namespace Util
}  // namespace Rrq

#endif // RR_UTIL_CALLABLE_HPP
