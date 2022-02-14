//
// GetPrimitiveType.hpp
//
// Created on: 2022-01-08
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

// Enables one to select a primitive type based on policy type. May be specified through use of `Tfictitious` template
// parameter

#if !defined(RR_SYNC_POLICY_GETPRIMITIVETYPE_HPP)
#define RR_SYNC_POLICY_GETPRIMITIVETYPE_HPP

#include <Rr/Sync/Policy/Type.hpp>

namespace Rr {
namespace Sync {
namespace Policy {

struct GetPrimitiveTypeDefault;

///
/// @brief Used to determine the synchronization type.
///
/// @tparam Ipolicy         Policy being used (special or generic. See `GetPolicyType`)
/// @tparam TsyncTrait      Synchronization trait storing sync configuration info
///
template <Policy::Type Ipolicy, class TsyncTrait>
struct GetPrimitiveType;

template <class Tst>
struct GetPrimitiveType<Policy::Type::None, Tst> : Trait::StoreType<Util::GenericMock> {
};

template <class Tst>
struct GetPrimitiveType<Policy::Type::Mutex, Tst> : Trait::StoreType<typename Tst::Mutex> {
};

template <Policy::Type Ipolicy, class Tst>
using GetPrimitiveTypeTp = typename GetPrimitiveType<Ipolicy, Tst>::Type;

}  // namespace Policy
}  // namespace Sync
}  // namespace Rr

#endif // RR_SYNC_POLICY_GETPRIMITIVETYPE_HPP
