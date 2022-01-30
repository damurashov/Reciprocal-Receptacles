//
// Primitive.hpp
//
// Created on: 2022-01-29
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

// Unified access to synchronization primitives' functionality

#if !defined(RR_SYNC_POLICY_PRIMITIVE_HPP)
#define RR_SYNC_POLICY_PRIMITIVE_HPP

#include <Rr/Sync/Policy/Type.hpp>
#include <Rr/Refl/CallFamily.hpp>
#include <Rr/Trait/Declval.hpp>
#include <Rr/Trait/EnableIf.hpp>

namespace Rr {
namespace Sync {
namespace Policy {
namespace PrimitiveImpl {

struct CallLock {
	template <class T>
	static inline auto call(T &a) -> decltype(a.lock())
	{
		return a.lock();
	}
};

struct CallUnlock {
	template <class T>
	static inline auto call(T &a) -> decltype(a.unlock())
	{
		return a.unlock();
	}
};

struct CallTryLock {
	template <class T>
	static inline auto call(T &a) -> decltype((bool)a.try_lock())
	{
		return a.try_lock();
	}

	template <class T>
	static inline auto call(T &a) -> decltype((bool)a.tryLock())
	{
		return a.tryLock();
	}
};

}  // namespace PrimitiveImpl

///
/// @brief Performs family-calls upon the provided type `T`. See test cases for `CallFamily` for more info.
///
/// @tparam I    Synchronization policy type
/// @tparam T    Synchronization primitive
/// @tparam Ftry Force the provided method to have `try_lock` or similar calls
///
template <Policy::Type I, class T, bool Ftry>
struct Primitive;

template <class T, bool Ftry>
struct Primitive<Policy::Type::None, T, Ftry> {
	static inline constexpr void lock(...)
	{
	}

	static inline constexpr void unlock(...)
	{
	}

	static inline constexpr bool tryLock()
	{
		return true;
	}
};

template <class T, bool Ftry>
class Primitive<Policy::Type::Mutex, T, Ftry> {
	static constexpr T& kObjStub = Rr::Trait::declval<T>();

	static_assert(Rr::Refl::CanCallFamily<PrimitiveImpl::CallLock>::check(kObjStub),
		"Mutex type must define method ::lock(0)");
	static_assert(Rr::Refl::CanCallFamily<PrimitiveImpl::CallUnlock>::check(kObjStub),
		"Mutex type must define method ::unlock(0)");

	static constexpr auto kDefinesTryLock = Rr::Refl::CanCallFamily<PrimitiveImpl::CallTryLock>::check(kObjStub);
	static_assert(!Ftry || kDefinesTryLock, "Mutex type must define method ::try_lock(0), or ::tryLock(0)");  // if try-lock functionality is required by whatever instance calls this, make sure the method is present

	template <bool F>
	static inline Rr::Trait::EnableIf<F, bool> tryLockImpl(T &aT)
	{
		return Rr::Refl::CallFamily<PrimitiveImpl::CallTryLock>::call(aT);
	}

	template <bool F>
	static inline Rr::Trait::EnableIf<!F, bool> tryLockImpl(T &aT)
	{
		Rr::Refl::CallFamily<PrimitiveImpl::CallLock>::call(aT);
		return true;
	}

public:
	static inline void lock(T &aT)
	{
		return Rr::Refl::CallFamily<PrimitiveImpl::CallLock>::call(aT);
	}

	static inline void unlock(T &aT)
	{
		return Rr::Refl::CallFamily<PrimitiveImpl::CallUnlock>::call(aT);
	}

	static inline bool tryLock(T &aT)
	{
		return tryLockImpl<kDefinesTryLock>();
	}
};

}  // namespace Policy
}  // namespace Sync
}  // namespace Rr


#endif // RR_SYNC_POLICY_PRIMITIVE_HPP
