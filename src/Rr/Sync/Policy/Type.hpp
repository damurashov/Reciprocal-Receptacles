//
// Type.hpp
//
// Created on: 2022-01-02
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_SYNC_POLICY_TYPE_HPP)
#define RR_SYNC_POLICY_TYPE_HPP

namespace Rr {
namespace Sync {
namespace Policy {

enum class Type {
	None,
	Mutex,
};

}  // namespace Policy
}  // namespace Sync
}  // namespace Rr

#endif // RR_SYNC_POLICY_TYPE_HPP