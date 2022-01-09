//
// SharedAccess.hpp
//
// Created on: 2022-01-08
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_SYNC_POLICY_SHAREDACCESS_HP)
#define RR_SYNC_POLICY_SHAREDACCESS_HP

#include <Rr/Sync/Policy/Introspection.hpp>
#include <Rr/Sync/Policy/Type.hpp>
#include <Rr/Sync/Policy/GetPrimitiveType.hpp>
#include <Rr/Sync/Policy/LockSfinae.hpp>
#include <Rr/Trait/IntegralConstant.hpp>
#include <Rr/Trait/IsSame.hpp>

namespace Rr {
namespace Sync {
namespace Policy {
namespace SharedAccessImpl {

struct SharedAccessGetPrimitiveType;

template <class TsyncTrait>
struct GetPolicy {
	template <bool F>
	struct Impl : Trait::IntegralConstant<Policy::Type, TsyncTrait::kSharedAccessPolicy> {
	};

	template <>
	struct Impl<false> : Trait::IntegralConstant<Policy::Type, TsyncTrait::kPolicy> {
	};

	static constexpr auto value = Impl<definesSharedAccessPolicy<TsyncTrait>()>::value;
};

}  // namespace SharedAccessImpl

template <class TsyncTrait>
class SharedAccess {
public:
	using Primitive = GetPrimitiveTypeTp<SharedAccessImpl::GetPolicy<TsyncTrait>::value,
		TsyncTrait,
		SharedACcessImpl::SharedAccessGetPrimitiveType>;

	static void lock(Primitive &a)
	{
		LockSfinae::lock(a);
	}

	static void unlock(Primitive &a)
	{
		LockSfinae::lock(a);
	}
};

}  // namespace Policy
}  // namespace Sync
}  // namespace Rr

#endif // RR_SYNC_POLICY_SHAREDACCESS_HP
