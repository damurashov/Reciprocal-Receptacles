//
// NoMember.hpp
//
// Created on: 2022-01-15
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_SYNC_POLICY_NOMEMBER_HPP)
#define RR_SYNC_POLICY_NOMEMBER_HPP

namespace Rr {
namespace Sync {
namespace Policy {

///
/// @brief Used as a fallback type for various SFINAE inferences
///
struct NoMember{
};

}  // namespace Policy
}  // namespace Sync
}  // namespace Rr

#endif // RR_SYNC_POLICY_NOMEMBER_HPP
