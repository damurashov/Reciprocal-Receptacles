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
/// @brief Used to determine the synchronization type. Like `GetPolicyType`, it uses `Tfictitious` template parameter
/// for specialization, if, of course, such specialization is required.
///
/// @tparam Ipolicy         Policy being used (special or generic. See `GetPolicyType`)
/// @tparam TsyncTrait      Synchronization trait storing sync configuration info (same principle as w/ `GetPolicyType`)
/// @tparam Tspecializatoin Specialization type.
///
template <Policy::Type Ipolicy, class TsyncTrait, class Tspecialization=GetPrimitiveTypeDefault>
struct GetPrimitiveType;

template <class Tst, class Tfi>
struct GetPrimitiveType<Policy::Type::None, Tst, Tfi> : Trait::StoreType<Util::GenericMock> {
};

template <class Tst, class Tfi>
struct GetPrimitiveType<Policy::Type::Mutex, Tst, Tfi> : Trait::StoreType<typename Tst::Mutex> {
};

template <Policy::Type Ipolicy, class Tst, class Tfi=GetPrimitiveTypeDefault>
using GetPrimitiveTypeTp = typename GetPrimitiveType<Ipolicy, Tst>::Type;

}  // namespace Policy
}  // namespace Sync
}  // namespace Rr

#endif // RR_SYNC_POLICY_GETPRIMITIVETYPE_HPP
