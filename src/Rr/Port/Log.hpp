//
// Log.hpp
//
// Created on: Jul 27, 2023
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL> <DOT> <COM>)
//

#ifndef RR_PORT_LOG_HPP
#define RR_PORT_LOG_HPP

namespace Rr {
namespace Port {

/// Defines logging level
enum class LogLevel {
	Error = 0,
	Warning,
	Info,
	Debug,
};

using PrintIntegralCallable = void(*)(LogLevel, char *aPrefix, int aValue);

namespace LogApiImpl {

/// Enables platform-dependent logging functionality.
template <class T>
class LogApi {
public:
	/// Sets the static callback
	static void setPrintIntegralCallable(PrintIntegralCallable aPrintIntegralCallable)
	{
		printIntegralCallable = aPrintIntegralCallable;
	}

	/// Invokes the static callback, if set
	static void tryPrintIntegralCallable(LogLevel aLogLevel, char *aPrefix, int aValue)
	{
		if (printIntegralCallable != nullptr) {
			printIntegralCallable(aLogLevel, aPrefix, aValue);
		}
	}

private:
	static PrintIntegralCallable printIntegralCallable;
};

template <class T>
PrintIntegralCallable LogApi<T>::printIntegralCallable{nullptr};

}  // LogApiImpl

using Log = typename LogApiImpl::LogApi<void>;

}  // Port
}  // Rr

#endif // RR_PORT_LOG_HPP
