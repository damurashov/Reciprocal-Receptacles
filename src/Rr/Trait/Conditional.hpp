//
// Conditional.hpp
//
// Created: 2021-12-07
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_CONDITIONAL_HPP)
#define RR_TRAIT_CONDITIONAL_HPP

namespace Rr {
namespace Trait {

template <bool, class A, class B>
struct Conditional {
	using type = A;
	using Type = A;
};

template <class A, class B>
struct Conditional<false, A, B> {
	using type = B;
	using Type = B;
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_CONDITIONAL_HPP
