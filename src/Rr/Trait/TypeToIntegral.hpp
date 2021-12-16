//
// TypeToIntegral.hpp
//
// Created: 2021-12-16
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_TYPETOINTEGRAL_HPP)
#define RR_TRAIT_TYPETOINTEGRAL_HPP

#include <Rr/Trait/IntegralConstant.hpp>
#include <Rr/Trait/IsSame.hpp>

namespace Rr {
namespace Trait {

namespace TypeToIntegralImpl {
struct Stop;
}  // namespace TypeToIntegralImpl

template <int Icurrent, class Tbase, class T, class ...Ta>
struct TypeToIntBase : IntegralConstant<int, TypeToIntBase<Icurrent + 1, Tbase, Ta...>::value> {
};

template <int Icurrent, class Tbase, class ...Ta>
struct TypeToIntBase<Icurrent, Tbase, Tbase, Ta...> : IntegralConstant<int, Icurrent> {
};

template <int Icurrent, class Tbase>
struct TypeToIntBase<Icurrent, Tbase, TypeToIntegralImpl::Stop> : IntegralConstant<int, -1> {
};

///
/// @brief Get id of a type
///
///  value - id of the type. -1, if not found
///
template <class Tbase, class ...Ta>
struct TypeToInt : TypeToIntBase<0, Tbase, Ta..., TypeToIntegralImpl::Stop> {
};

template <class Tintegral, class Tbase, class ...Ta>
struct TypeToIntegral : IntegralConstant<Tintegral, (Tintegral)TypeToInt<Tbase, Ta...>::value> {
};

///
/// @brief Helper. Checks whether a type is present among types
///
template <class Tbase, class ...Ta>
struct TypeIn : IntegralConstant<bool, TypeToInt<Tbase, Ta...>::value != -1> {
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_TYPETOINTEGRAL_HPP
