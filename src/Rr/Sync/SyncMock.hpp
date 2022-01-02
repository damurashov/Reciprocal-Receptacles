//
// SyncMock.hpp
//
// Created: 2021-12-14
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_SYNCMOCK_HPP)
#define RR_UTIL_SYNCMOCK_HPP

namespace Rr {
namespace Util {

// TODO: redundant nested namespace

namespace Sync {

///
/// @brief Mocks STL's lock_guard: https://en.cppreference.com/w/cpp/thread/lock_guard
///
/// @tparam Mutex
///
template <typename Mutex>
struct LockGuard final {
public:
	LockGuard(Mutex &aMutex) : mutex(aMutex)
	{
		mutex.lock();
	}
	~LockGuard()
	{
		mutex.unlock();
	}
private:
	Mutex &mutex;
};

///
/// @brief STL BasicLockable trait https://en.cppreference.com/w/cpp/named_req/BasicLockable
///
struct MockBasicLockable final {
	inline void lock()
	{
	}

	inline void unlock()
	{
	}
};

///
/// @brief STL SharedMutex trait https://en.cppreference.com/w/cpp/named_req/SharedMutex
///
struct MockSharedMutex final {
	inline void lock()
	{
	}

	inline void unlock()
	{
	}

	constexpr bool try_lock()
	{
		return true;
	}

	inline void lock_shared()
	{
	}

	constexpr bool try_lock_shared()
	{
		return true;
	}

	inline void unlock_shared()
	{
	}
};

}  // namespace Sync

}  // namespace Util
}  // namespace Rr

#endif // RR_UTIL_SYNCMOCK_HPP
