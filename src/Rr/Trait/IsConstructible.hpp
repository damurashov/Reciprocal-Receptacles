//
// IsConstructible.hpp
//
// Created on:  May 21, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#if !defined(RR_TRAIT_ISCONSTRUCTIBLE_HPP_)
#define RR_TRAIT_ISCONSTRUCTIBLE_HPP_

#include <Rr/Refl/CallFamily.hpp>
#include <Rr/Refl/NoMember.hpp>
#include <Rr/Trait/IntegralConstant.hpp>
#include <Rr/Trait/Declval.hpp>

namespace Rr {
namespace Trait {
namespace IsConstructibleImpl {

struct CtorSfinae {
	template <class T, class ...Ts>
	static auto call(int) -> decltype(T{Rr::Trait::declval<Ts>()...});

	template <class T, class ...Ts>
	static auto call(...) -> Refl::NoMember;
};

template <class T, class ...Ts>
struct IsConstructible : Rr::Trait::IntegralConstant<bool,
	!Rr::Trait::IsSame<Refl::NoMember, decltype(CtorSfinae::call<T, Ts...>(0))>::value> {
};

}  // IsConstructibleImpl

using IsConstructibleImpl::IsConstructible;

}  // Trait
}  // Rr

#endif  // RR_TRAIT_ISCONSTRUCTIBLE_HPP_
