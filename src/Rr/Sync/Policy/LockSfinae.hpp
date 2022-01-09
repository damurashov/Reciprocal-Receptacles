//
// LockSfinae.hpp
//
// Created on: 2022-01-08
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_SYNC_POLICY_LOCKSFINAE_HPP)
#define RR_SYNC_POLICY_LOCKSFINAE_HPP

namespace Rr {
namespace Sync {
namespace Policy {

struct LockSfinae {
	struct Fallback {};

	static constexpr Fallback tryLock(...)
	{
		return {};
	}

	static constexpr Fallback lock(...)
	{
		return {};
	}

	template <class T>
	static auto tryLock(T& a) -> decltype(a.try_lock())
	{
		return a.try_lock();
	}

	template <class T>
	static auto tryLock(T& a) -> decltype(a.tryLock())
	{
		return a.tryLock();
	}

	template <class T>
	static auto lock(T& a) -> decltype(a.lock())
	{
		return a.lock();
	}

	template <class T>
	static auto unlock(T& a) -> decltype(a.unlock())
	{
		return a.unlock();
	}
};

}  // namespace Policy
}  // namespace Sync
}  // namespace Rr

#endif // RR_SYNC_POLICY_LOCKSFINAE_HPP
