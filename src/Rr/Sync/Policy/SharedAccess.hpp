//
// SharedAccess.hpp
//
// Created on: 2022-01-08
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_SYNC_POLICY_SHAREDACCESS_HP)
#define RR_SYNC_POLICY_SHAREDACCESS_HP

#include <Rr/Refl/Introspection.hpp>
#include <Rr/Sync/Policy/Type.hpp>
#include <Rr/Sync/Policy/GetPrimitiveType.hpp>
#include <Rr/Sync/Policy/GetPolicyType.hpp>
#include <Rr/Refl/CallSfinae.hpp>
#include <Rr/Trait/IntegralConstant.hpp>
#include <Rr/Trait/IsSame.hpp>
#include <Rr/Trait/IntegralConstant.hpp>

namespace Rr {
namespace Sync {
namespace Policy {
namespace SharedAccessImpl {

struct CallSharedAccessPolicy {
	template <class T>
	static auto call() -> decltype(T::kSharedAccessPolicy)
	{
		return T::kSharedAccessPolicy;
	}
};

}  // namespace SharedAccessImpl

template <class TsyncTrait>
struct SharedAccess {
	static constexpr auto kPolicy = getPolicyType<SharedAccessImpl::CallSharedAccessPolicy, CallPolicy>();
	using PrimitiveType = GetPrimitiveTypeTp<kPolicy, TsyncTrait>;
};

}  // namespace Policy
}  // namespace Sync
}  // namespace Rr

#endif // RR_SYNC_POLICY_SHAREDACCESS_HP
