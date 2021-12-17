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

///
/// @brief Underlying engine for IntToPos<...>
///
/// @tparam Isizeof    Initial sizeof of arguments
/// @tparam ImaxSizeof Isizeof + 1 - marker
/// @tparam Ibase      The integer which position should be found
/// @tparam Ipos       Current position
/// @tparam Icurrent   Current integer
/// @tparam Ia         Integers, including 2 fictitious elements
///
template <unsigned Isizeof, unsigned ImaxSizeof, int Ibase, unsigned Ipos, int Icurrent, int... Ia>
struct IntToPosBase : IntegralConstant<int, IntToPosBase<Isizeof, ImaxSizeof, Ibase, Ipos + 1, Ia...>::value> {
};

template <unsigned Isizeof, unsigned ImaxSizeof, int Ibase, unsigned Ipos, int ...Ia>
struct IntToPosBase<Isizeof, ImaxSizeof, Ibase, Ipos, Ibase, Ia...> : IntegralConstant<int, Ipos> {
};

template <unsigned Isizeof, unsigned ImaxSizeof, int Ibase, int Icurrent>
struct IntToPosBase<Isizeof, ImaxSizeof, Ibase, ImaxSizeof, Icurrent> : IntegralConstant<int, -1> {
};

///
/// @brief Find one int among integers. IntToPos<>::value = -1, if not found
///
/// @tparam Ibase The element to find
/// @tparam Ia The heap in which the search is conducted
///
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
