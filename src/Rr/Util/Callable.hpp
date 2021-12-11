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
private:

#if !RRO_STATIC_CAST_FN_CONVERSION
	struct Virtual {
		virtual void v() {}
		void (Virtual::*callback)();
		Virtual *instance;
	};
#endif

protected:
	union VariantCallable {

// To make sure that Fn<...>::Callable will fit. This is because
// virtual member callbacks may differ in sizes from non-virtual ones.
// static_cast guarantees that everything is smooth, which is not the
// case with reinterpret_cast'ing of member pointers which is used
// when RRO_STATIC_CAST_FN_CONVERSION = 0
#if !RRO_STATIC_CAST_FN_CONVERSION
		Virtual v;
#endif

		typename Rr::Trait::Fn<Trr, Tsignature> member;  // Callable encapsulating instance pointer and method pointer
		typename Rr::Trait::Fn<Tsignature> nonmember;  // Callable encapsulating static function pointer
		VariantCallable() {
		}
	} callable;
	TrrCb callback;

public:
	Tret operator()(Ta ...aArgs)
	{
		return (static_cast<Trr *>(&callable.member)->*callback)(aArgs...);
	}

	Tret operator()(Ta ...aArgs) const
	{
		return (static_cast<Trr *>(&callable.member)->*callback)(aArgs...);
	}

	CallableImpl(Tret (*aCallback)(Ta...))
	{
		new (&callable.nonmember) typename Rr::Trait::Fn<Tsignature>{aCallback};
		// CallableType has RrObject as its base, hence use of static_cast
		callback = static_cast<decltype(callback)>(&Rr::Trait::Fn<Tsignature>::operator());
	}

	template <class Tc, class Ti>
	CallableImpl(Tc aCallback, Ti *aInstance)
	{
		new (&callable.member) typename Rr::Trait::Fn<Trr, Tsignature>{aCallback, aInstance};
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
}  // namespace Rr

#endif // RR_UTIL_CALLABLE_HPP
