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

template <int Ibase, int Icurrent, class T, class ...Ta>
struct IntToTypeBase {
	using Type = typename IntToTypeBase<Ibase, Icurrent + 1, Ta...>::Type;
};

template <int Icurrent, class T, class ...Ta>
struct IntToTypeBase<Icurrent, Icurrent, T, Ta...> {
	using Type = T;
};

template <class Tintegral, Tintegral Ival, class ...Ta>
struct IntegralToType {
	using Type = typename IntToTypeBase<(int)Ival, 0, Ta...>::Type;
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_IDTOTYPENAME_HPP
