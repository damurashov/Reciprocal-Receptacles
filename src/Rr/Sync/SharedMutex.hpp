//
// SharedMutex.hpp
//
// Created on: Jul 27, 2023
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL> <DOT> <COM>)
//

#ifndef RR_SYNC_SHAREDMUTEX_HPP
#define RR_SYNC_SHAREDMUTEX_HPP

#include <Rr/Port/Thread.hpp>
#include <Rr/Port/Log.hpp>

namespace Rr {
namespace Sync {

/// Can either be locked in unique, or shared mode. Implements shared lock
/// functionality using plain mutex.
///
/// \details Frequent shared locks, and infrequent unique ones are expected.
/// Unique locks have a priority.
///
/// \tparam Plain mutex type. Must provide `lock()` and `unlock()` methods
template <class M>
class SharedMutex final {
private:
	/// Maximum number of attempts before a warning log message is issued
	static constexpr int kMaxAttempts = 30;

public:
	SharedMutex():
		uniqueLockRequestCounter{0},
		sharedLockRequestCounter{0},
		mutex{}
	{
	}

	/// Unique lock. Waits for shared locks and preceding turns to finish, then
	/// yields control
	/// \warning The method does not check whether the mutex has already been
	/// locked. The turns are organized as LIFO.
	void lock()
	{
		int uniqueLockAttemptId = 0;

		mutex.lock();

		// Register unique lock attempt
		uniqueLockRequestCounter += 1;
		uniqueLockAttemptId = uniqueLockRequestCounter;

		// Release mutex
		mutex.unlock();

		// Wait until all the shared locks are out, and it this instance's turn to use the resource
		int attemptCounter = kMaxAttempts;

		while (true) {
			mutex.lock();

			// Check if it is this instance's turn
			if (sharedLockRequestCounter == 0 && uniqueLockRequestCounter == uniqueLockAttemptId) {
				mutex.unlock();

				break;
			} else {
				mutex.unlock();
			}

			if (attemptCounter > 0) {
				attemptCounter -= 1;
			} else {
				Rr::Port::Log::tryPrintIntegral(Rr::Port::LogLevel::Warning,
					"`SharedMutex::lock()` may be stuck, maximum number of attempts has been exceeded:",
					kMaxAttempts);
			}

			Rr::Port::Thread::trySleep();
		}
	}

	void unlock()
	{
		mutex.lock();
		uniqueLockRequestCounter -= 1;
		mutex.unlock();
	}

	void lockShared()
	{
		int attemptCounter = kMaxAttempts;

		while (true) {
			mutex.lock();

			if (uniqueLockRequestCounter != 0) {
				mutex.unlock();
				Rr::Port::Thread::trySleep();

				if (attemptCounter > 0) {
					attemptCounter -= 1;
				} else {
					Rr::Port::Log::tryPrintIntegral(Rr::Port::LogLevel::Warning,
						"`SharedMutex::lockShared()` may be stuck, maximum number of attempts has been exceeded:",
						kMaxAttempts);
				}

			} else {
				sharedLockRequestCounter += 1;
				mutex.unlock();

				break;
			}
		}
	}

	void unlockShared()
	{
		mutex.lock();
		sharedLockRequestCounter -= 1;
		mutex.unlock();
	}

private:
	int uniqueLockRequestCounter;
	int sharedLockRequestCounter;
	bool uniqueLocked;
	bool sharedLocked;
	M mutex;
};

}  // Sync
}  // Rr

#endif // RR_SYNC_SHAREDMUTEX_HPP
