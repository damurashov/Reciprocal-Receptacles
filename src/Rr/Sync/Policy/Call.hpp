//
// Call.hpp
//
// Created on: 2022-01-02
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <Rr/Sync/Introspection.hpp>
#include <Rr/Sync/SyncMock.hpp>
#include <Rr/Sync/Policy/Type.hpp>
#include <Rr/Trait/Conditional.hpp>
#include <Rr/Trait/IntegralConstant.hpp>
#include <Rr/Trait/Switch.hpp>
#include <Rr/Util/GenericMock.hpp>

namespace Rr {
namespace Sync {
namespace Policy {
namespace CallImpl {

// Get policy

template <bool Fcp, class T>
struct GetPolicy : Trait::IntegralConstant<Policy::Type, T::kCallPolicy> {
};

template <class T>
struct GetPolicy<false, T> : Trait::IntegralConstant<Policy::Type, T::kPolicy> {
};

// StoreTypes

template <class Tprimitive, class TlockBefore>
struct StoreTypes {
	using Primitive = Tprimitive;
	using RaiiBefore = TlockBefore;
};

// GetTypes

template <Policy::Type, class T>
struct GetTypes;

using Mock = Util::GenericMock;

template <class T>
struct GetTypes<Policy::Type::None, T> : StoreTypes<Mock, Mock> {
};

template <class T>
struct GetTypes<Policy::Type::Mutex, T> : StoreTypes<typename T::Mutex, typename T::Lock> {
};

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

	static constexpr auto kPolicy = CallImpl::template GetPolicy<definesCallPolicy<T>(), T>::value;
	static_assert(!(kPolicy == Type::Mutex) || definesMutex<T>(), "For mutex-based lock. policy, `Mutex` type should be defined");
	static_assert(!(kPolicy == Type::Mutex) || definesLock<T>(), "For mutex-based lock. policy, `Lock` type should be defined");

	using Types = typename CallImpl::template GetTypes<kPolicy, T>;

public:
	using Primitive = typename Types::Primitive;
	using RaiiBefore = typename Types::RaiiBefore;
};

}  // namespace Policy
}  // namespace Sync
}  // namespace Rr