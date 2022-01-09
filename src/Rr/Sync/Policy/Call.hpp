//
// Call.hpp
//
// Created on: 2022-01-02
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <Rr/Sync/Policy/Introspection.hpp>
#include <Rr/Sync/SyncMock.hpp>
#include <Rr/Sync/Policy/Type.hpp>
#include <Rr/Sync/Policy/LockSfinae.hpp>
#include <Rr/Sync/Policy/GetPrimitiveType.hpp>
#include <Rr/Trait/Conditional.hpp>
#include <Rr/Trait/IntegralConstant.hpp>
#include <Rr/Trait/Switch.hpp>
#include <Rr/Util/GenericMock.hpp>

#if !defined(RR_SYNC_POLICY_CALL_HPP)
#define RR_SYNC_POLICY_CALL_HPP

namespace Rr {
namespace Sync {
namespace Policy {
namespace CallImpl {

struct CallGetPrimitiveType;

// Get policy

template <bool Fcp, class T>
struct GetPolicyImpl : Trait::IntegralConstant<Policy::Type, T::kCallPolicy> {
};

template <class T>
struct GetPolicyImpl<false, T> : Trait::IntegralConstant<Policy::Type, T::kPolicy> {
};

template <class T>
using GetPolicy = GetPolicyImpl<definesCallPolicy<T>(), T>;

// Lock - specializations of Rr::Sync::Policy::Lock

}  // namespace CallImpl

///
/// @brief Defines RAII lock type for a callable object
///
/// @tparam T Sync policy trait, subject to introspection
///
template <class T>
class Call {
private:
	static_assert(definesCallPolicy<T>() || definesPolicy<T>(), "At least, generic sync policy has to be defined");
	static constexpr auto kPolicy = CallImpl::GetPolicy<T>::value;
	static_assert(!(kPolicy == Type::Mutex) || definesMutex<T>(), "For mutex-based lock. policy, `Mutex` type should be defined");

public:
	using Primitive = GetPrimitiveTypeTp<kPolicy, T, CallImpl::CallGetPrimitiveType>;
	using Lock = Util::GenericMock;

	static void lock(Primitive &a)
	{
		Policy::LockSfinae::lock(a);
	}

	static void unlock(Primitive &a)
	{
		LockSfinae::unlock(a);
	}
};

}  // namespace Policy
}  // namespace Sync
}  // namespace Rr

#endif // RR_SYNC_POLICY_CALL_HPP
