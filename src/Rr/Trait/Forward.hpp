//
// Forward.hpp
//
// Created: 2021-12-21
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_FORWARD_HPP)
#define RR_TRAIT_FORWARD_HPP

#include <Rr/Trait/RemoveReference.hpp>

namespace Rr {
namespace Trait {

template <class T>
constexpr T&& forward(Rr::Trait::RemoveReference<T> &a)
{
	return static_cast<T &&>(a);
}

template <class T>
constexpr T&& forward(Rr::Trait::RemoveReference<T> &&a)
{
	return static_cast<T &&>(a);
}

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_FORWARD_HPP
