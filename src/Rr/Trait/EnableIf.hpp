//
// EnableIf.hpp
//
// Created: 2021-12-06
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_ENABLEIF_HPP)
#define RR_TRAIT_ENABLEIF_HPP

namespace Rr {
namespace Trait {

template <bool N, class T = void>
struct EnableIf;

template <class T>
struct EnableIf<true, T> {
	using type = T;
	using Type = T;
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_ENABLEIF_HPP
