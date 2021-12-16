//
// IntegralIn.hpp
//
// Created: 2021-12-16
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_INTEGRALIN_HPP)
#define RR_TRAIT_INTEGRALIN_HPP

#include <Rr/Trait/IntegralConstant.hpp>

namespace Rr {
namespace Trait {

namespace IntegralInImpl {
constexpr int kMarkerInt = 0xffffffff;
}  // namespace IntegralInImpl

template <int Ibase, unsigned Ipos, int Icurrent, int... Ia>
struct IntToPosBase : IntegralConstant<int, IntToPosBase<Ibase, Ipos + 1, Ia...>::value> {
};

template <int Ibase, unsigned Ipos, int ...Ia>
struct IntToPosBase<Ibase, Ipos, Ibase, Ia...> : IntegralConstant<int, Ipos> {
};

template <int Ibase, unsigned Ipos>
struct IntToPosBase<Ibase, Ipos, IntegralInImpl::kMarkerInt>: IntegralConstant<int, -1> {
};

template <int Ibase, int ...Ia>
struct IntToPos : IntToPosBase<Ibase, 0, Ia..., IntegralInImpl::kMarkerInt> {
};

template <class Tintegral, Tintegral ...Ia>
struct IntegralToPos : IntegralConstant<Tintegral, (Tintegral)IntToPos<Ia...>::value> {
};

template <int Ibase, int ...Ia>
struct IntIn : IntegralConstant<bool, IntToPos<Ibase, Ia...>::value != -1> {
};

template <class Tintegral, Tintegral Ibase, Tintegral ...Ia>
struct IntegralIn : IntegralConstant<bool, IntIn<(int)Ibase, ((int)Ia)...>::value> {
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_INTEGRALIN_HPP
