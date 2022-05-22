//
// IsFunction.hpp
//
// Created on:  May 22, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#if !defined(RR_TRAIT_ISFUNCTION_HPP_)
#define RR_TRAIT_ISFUNCTION_HPP_

#include <Rr/Trait/IntegralConstant.hpp>
#include <Rr/Trait/MemberDecay.hpp>
#include <Rr/Trait/EnableIf.hpp>
#include <Rr/Refl/CallFamily.hpp>

namespace Rr {
namespace Trait {
namespace IsFunctionImpl {

struct MethodSfinae {
	template <class T, bool F = Trait::MemberDecay<T>::kIsPointerForm>
	static typename Trait::EnableIf<!F>::Type call();
};

template <class T>
constexpr bool checkIsFunction()
{
	return Refl::CanCallFamily<MethodSfinae>::template check<T>();
}

template <class T>
struct IsFunction : Rr::Trait::IntegralConstant<bool, checkIsFunction<T>()> {
};

}  // IsFunctionImpl

using IsFunctionImpl::IsFunction;

}  // Trait
}  // Rr

#endif  // RR_TRAIT_ISFUNCTION_HPP_
