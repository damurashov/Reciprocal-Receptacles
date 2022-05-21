//
// IsConstructible.hpp
//
// Created on:  May 21, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#if !defined(RR_TRAIT_ISCONSTRUCTIBLE_HPP_)
#define RR_TRAIT_ISCONSTRUCTIBLE_HPP_

#include <Rr/Trait/CallFamily.hpp>
#include <Rr/Trait/NoMember.hpp>
#include <Rr/Trait/IntegralConstant.hpp>
#include <Rr/Trait/Declval.hpp>

namespace Rr {
namespace Trait {
namespace IsConstructibleImpl {

struct CtorSfinae {
	template <class T, class ...Ts>
	auto call(int) -> decltype(T{Rr::Trait::declval<Ts>()...});

	template <class T, class ...Ts>
	auto call(...) -> Trait::NoMember;
};

template <class T, class ...Ts>
struct IsConstructible : Rr::Trait::IntegralConstant<bool,
	!Rr::Trait::IsSame<Trait::NoMember, decltype(CtorSfinae::call<T, Ts...>)>::value> {
};

}  // IsConstructibleImpl

using IsConstructibleImpl::IsConstructible;

}  // Trait
}  // Rr

#endif  // RR_TRAIT_ISCONSTRUCTIBLE_HPP_
