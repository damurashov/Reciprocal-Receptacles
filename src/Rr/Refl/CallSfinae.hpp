//
// CallSfinae.hpp
//
// Created on: 2022-01-08
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_REFL_LOCKSFINAE_HPP)
#define RR_REFL_LOCKSFINAE_HPP

#include <Rr/Refl/NoMember.hpp>

namespace Rr {
namespace Refl {

struct LockSfinae {
	static constexpr NoMember lock(...)
	{
		return {};
	}
	template <class T>
	static auto lock(T& a) -> decltype(a.lock())
	{
		return a.lock();
	}
};

struct TryLockSfinae {
	static constexpr NoMember tryLock(...)
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
};

struct UnlockSfinae {
	static constexpr NoMember unlock(...)
	{
		return {};
	}

	template <class T>
	static auto unlock(T& a) -> decltype(a.unlock())
	{
		return a.unlock();
	}
};

struct CallSfinae : LockSfinae, UnlockSfinae, TryLockSfinae {
};

}  // namespace Refl
}  // namespace Rr

#endif // RR_REFL_LOCKSFINAE_HPP
