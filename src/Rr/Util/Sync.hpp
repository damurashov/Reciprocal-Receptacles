//
// Sync.hpp
// 
// Author: Dmitry Murashov
//  Email: dmtr D murashov A gmail D com (Obfuscated to protect from spambots. Replace "D" with ".", "A" with "@")
//

#if !defined(RR_UTIL_SYNC_HPP_)
#define RR_UTIL_SYNC_HPP_

namespace Rr {
namespace Util {
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

}  // namespace Sync
}  // namespace Util
}  // namespace Rr

#endif // RR_UTIL_SYNC_HPP_

