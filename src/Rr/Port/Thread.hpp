//
// Thread.hpp
//
// Created on: Jul 27, 2023
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL> <DOT> <COM>)
//

#ifndef RR_PORT_THREAD_HPP
#define RR_PORT_THREAD_HPP

namespace Rr {
namespace Port {

/// Callable for putting a current thread into delay for a minimum amount of
/// time.
using ThreadSleepCallable = void(*)();

namespace ThreadApiImpl {

template <class T>
class ThreadApi {
public:
	static void setThreadSleepCallable(ThreadSleepCallable);

	/// Invokes sleep (yield control) function, if set
	static void trySleep()
	{
		if (threadSleepCallable != nullptr) {
			threadSleepCallable();
		}
	}

private:
	static ThreadSleepCallable threadSleepCallable;
};

template <class T>
ThreadSleepCallable ThreadApi<T>::threadSleepCallable{nullptr};

}  // namespace ThreadApiImpl

using Thread = typename ThreadApiImpl::ThreadApi<void>;

}  // Port
}  // Rr

#endif // RR_PORT_THREAD_HPP
