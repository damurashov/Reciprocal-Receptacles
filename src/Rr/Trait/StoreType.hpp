//
// StoreType.hpp
//
// Created: 2021-12-21
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_STORETYPE_HPP)
#define RR_TRAIT_STORETYPE_HPP

namespace Rr {
namespace Trait {

template <class T>
struct StoreType {
	using Type = T;
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_STORETYPE_HPP
