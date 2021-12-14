//
// RemoveReference.hpp
//
// Created: 2021-12-14
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_REMOVEREFERENCE_HPP)
#define RR_TRAIT_REMOVEREFERENCE_HPP

namespace Rr {
namespace Trait {

template <class T>
struct RemoveReference {
	using Type = T;
};

template <class T>
struct RemoveReference<T&> {
	using Type = T;
};

template <class T>
struct RemoveReference<T&&> {
	using Type = T;
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_REMOVEREFERENCE_HPP
