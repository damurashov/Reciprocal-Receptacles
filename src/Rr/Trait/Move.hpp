//
// Move.hpp
//
// Created: 2021-12-14
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_MOVE_HPP)
#define RR_TRAIT_MOVE_HPP

#include <Rr/Trait/RemoveReference.hpp>

namespace Rr {
namespace Trait {

template <class T>
typename Rr::Trait::RemoveReference<T>::Type&& move(T &&aArg)
{
	return static_cast<typename Rr::Trait::RemoveReference<T>::Type&&>(aArg);
}

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_MOVE_HPP
