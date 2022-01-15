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
#include <Rr/Refl/LockSfinae.hpp>
#include <Rr/Trait/IntegralConstant.hpp>
#include <Rr/Trait/IsSame.hpp>
#include <Rr/Trait/IntegralConstant.hpp>

namespace Rr {
namespace Sync {
namespace Policy {

struct SharedAccessSpecialization;  // Compile-time specialization "token". For extension.

///
/// @brief Specialization. See <GetPolicyType.hpp>
///
template<bool F, class TsyncTrait>
struct GetPolicyType<F, TsyncTrait, SharedAccessSpecialization> : Trait::IntegralConstant<Policy::Type, TsyncTrait::kSharedAccessPolicy> {
};

///
/// @brief
///
/// @tparam T Synchronization trait
///
template <class T>
class SharedAccess {
public:
	static constexpr auto kPolicy = GetPolicyType<Refl::definesSharedAccessPolicy<T>(), T, SharedAccessSpecialization>;
	using Primitive = GetPrimitiveTypeTp<kPolicy, T, SharedAccessSpecialization>;

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
