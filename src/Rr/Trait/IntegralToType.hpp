//
// IntegralToType.hpp
//
// Created: 2021-12-15
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_IDTOTYPENAME_HPP)
#define RR_TRAIT_IDTOTYPENAME_HPP

#include <Rr/Trait/IntegralConstant.hpp>

namespace Rr {
namespace Trait {

template <class Tintegral, Tintegral Ibase, Tintegral Icurrent, class T, class ...Ta>
struct IntegralToTypeImpl {
	using Type = typename IntegralToTypeImpl<Tintegral, Ibase, Icurrent + 1, Ta...>::Type;
};

template <class Tintegral, Tintegral Icurrent, class T, class ...Ta>
struct IntegralToTypeImpl<Tintegral, Icurrent, Icurrent, T, Ta...> {
	using Type = T;
};

template <class Tintegral, Tintegral Ival, class ...Ta>
struct IntegralToType {
	using Type = typename IntegralToTypeImpl<Tintegral, Ival, 0, Ta...>::Type;
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_IDTOTYPENAME_HPP
