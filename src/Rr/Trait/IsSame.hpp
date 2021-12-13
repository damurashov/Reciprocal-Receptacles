//
// IsSame.hpp
//
// Created: 2021-12-13
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_ISSAME_HPP)
#define RR_TRAIT_ISSAME_HPP

#include <Rr/Trait/IntegralConstant.hpp>

namespace Rr {
namespace Trait {

template <class T1, class T2>
struct IsSame : Rr::Trait::IntegralConstant<bool, false>
{
};

template <class T>
struct IsSame<T, T> : Rr::Trait::IntegralConstant<bool, true>
{
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_ISSAME_HPP
