//
// GetPolicyType.hpp
//
// Created on: 2022-01-09
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_SYNC_POLICY_GETPOLICY_HPP)
#define RR_SYNC_POLICY_GETPOLICY_HPP

#include <Rr/Trait/IntegralConstant.hpp>
#include <Rr/Sync/Policy/Type.hpp>
#include <Rr/Refl/Introspection.hpp>
#include <Rr/Refl/CallFamily.hpp>
#include <Rr/Trait/IsSame.hpp>
#include <Rr/Trait/RemoveReference.hpp>

namespace Rr {
namespace Sync {
namespace Policy {

struct GetPolicyTypeDefault;

///
/// @brief Helper to a compile-time policy inference code. Used to determine whether or not a `SyncTrait` specifies any
/// particular policy type for a module, and if so, extract its enum value. In essence, it boils down to the following
/// logic:
///
/// @code{.c}
/// has_element(SyncTrait::k<Special>Policy)  ?  return SyncTrait::k<Special>Policy  :  return SyncTrait::kPolicy
/// @endcode
///
/// This code is extended through specialization mechanism (see `Tspecialization` template argument)
/// To see the complete list of policies available, please refer to <Rr/Sync/Policy/Type.hpp>
///
/// @tparam Tspecialization  Extension point
/// @tparam FdefinesSpecial  Defines special type of policy for this particular component. e.g. `kCallPolicy`. See <Introspection.hpp>.
/// @tparam TsyncTrait       Sync trait that encapsulates the necessary info on policies
///
template <bool FdefinesSpecial, class TsyncTrait, class Tspecialization=GetPolicyTypeDefault>
struct GetPolicyType;

template <class T, class TsyncTrait>
struct GetPolicyType<false, TsyncTrait, T>  {
	static_assert(Refl::definesPolicy<TsyncTrait>(), "In an absence of some special synchronization policies, a synchronization trait should provide ::kPolicy member");
	static constexpr auto value = TsyncTrait::kPolicy;
};

struct CallPolicy {
	template <class T>
	static constexpr auto call() -> decltype(T::kPolicy)
	{
		return T::kPolicy;
	}
};

///
/// @brief Returns constexpr-specified policy type.
/// @tparam T     Synchronization trait
///
/// @tparam Tcall Inference helpers, similar to `CallGetPolicy`. The `CallGetPolicy` itself may be used as a "last
/// resort", i.e. when specific sync. policies were not defined
///
template <class T, class ...Tcall>
inline constexpr Rr::Sync::Policy::Type getPolicyType()
{
	static_assert(Rr::Refl::CanCallFamily<Tcall...>::template check<T>(), "Policy type cannot be inferred");
	static_assert(Trait::IsSame<Trait::StripTp<decltype(Rr::Refl::ConstexprCallFamily<Tcall...>::template call<T>())>, Policy::Type>::value, "The inferred policy is not of type `Rr::Sync::Policy::Type`");
	return Rr::Refl::ConstexprCallFamily<Tcall...>::template call<T>();
}

}  // namespace Policy
}  // namespace Sync
}  // namespace Rr

#endif // RR_SYNC_POLICY_GETPOLICY_HPP
