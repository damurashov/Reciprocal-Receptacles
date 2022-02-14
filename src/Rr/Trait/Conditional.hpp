//
// Conditional.hpp
//
// Created: 2021-12-07
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_CONDITIONAL_HPP)
#define RR_TRAIT_CONDITIONAL_HPP

#include <Rr/Trait/StoreType.hpp>

namespace Rr {
namespace Trait {

template <bool, class A, class B>
struct Conditional : StoreType<A> {
};

template <class A, class B>
struct Conditional<false, A, B> : StoreType<B> {
};

template <bool F, class Ta, class Tb>
using ConditionalTp = typename Conditional<F, Ta, Tb>::Type;

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_CONDITIONAL_HPP
