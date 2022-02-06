//
// Storage.hpp
//
// Created on: 2022-01-15
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_SYNC_POLICY_STORAGE_HPP)
#define RR_SYNC_POLICY_STORAGE_HPP

#include <Rr/Sync/Policy/GetPolicyType.hpp>
#include <Rr/Sync/Policy/GetPrimitiveType.hpp>
#include <Rr/Sync/Policy/Primitive.hpp>

namespace Rr {
namespace Sync {
namespace Policy {
namespace StorageImpl {

struct CallStoragePolicy {
	template <class T>
	static constexpr auto call() -> decltype(T::kStoragePolicy)
	{
		return T::kStoragePolicy;
	}
};

}  // namespace StorageImpl

template <class TsyncTrait>
struct Storage {
	static constexpr auto kPolicy = getPolicyType<TsyncTrait, StorageImpl::CallStoragePolicy, CallPolicy>();
	using PrimitiveType = GetPrimitiveTypeTp<kPolicy, TsyncTrait>;
	using PrimitiveOps = Primitive<kPolicy, PrimitiveType, false>;
};

}  // namespace Policy
}  // namespace Sync
}  // namespace Rr

#endif // RR_SYNC_POLICY_STORAGE_HPP
