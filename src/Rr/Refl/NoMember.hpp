//
// NoMember.hpp
//
// Created on: 2022-01-15
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_REFL_NOMEMBER_HPP)
#define RR_REFL_NOMEMBER_HPP

#include <Rr/Trait/IntegralConstant.hpp>
#include <Rr/Trait/IsSame.hpp>
#include <Rr/Trait/RemoveReference.hpp>

namespace Rr {
namespace Refl {

///
/// @brief Used as a fallback type for various SFINAE inferences
///
struct NoMember {
};

template <class T>
struct IsNoMember :
    Trait::IntegralConstant<bool, Rr::Trait::IsSame<Rr::Trait::StripTp<T>, NoMember>::value>
{
};

}  // namespace Refl
}  // namespace Rr

#endif // RR_REFL_NOMEMBER_HPP
