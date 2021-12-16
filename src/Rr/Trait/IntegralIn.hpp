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

template <unsigned Isizeof, unsigned ImaxSizeof, int Ibase, unsigned Ipos, int Icurrent, int... Ia>
struct IntToPosBase : IntegralConstant<int, IntToPosBase<Isizeof, ImaxSizeof, Ibase, Ipos + 1, Ia...>::value> {
};

template <unsigned Isizeof, unsigned ImaxSizeof, int Ibase, unsigned Ipos, int ...Ia>
struct IntToPosBase<Isizeof, ImaxSizeof, Ibase, Ipos, Ibase, Ia...> : IntegralConstant<int, Ipos> {
};

template <unsigned Isizeof, unsigned ImaxSizeof, int Ibase, int Icurrent>
struct IntToPosBase<Isizeof, ImaxSizeof, Ibase, ImaxSizeof, Icurrent> : IntegralConstant<int, -1> {
};

template <int Ibase, int ...Ia>
struct IntToPos : IntToPosBase<sizeof...(Ia), sizeof...(Ia) + 1, Ibase, 0, Ia..., Ibase - 1, Ibase - 1> {
};

template <class Tintegral, Tintegral ...Ia>
struct IntegralToPos : IntegralConstant<int, IntToPos<((int)Ia)...>::value> {
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
