//
// Callback.hpp
//
// Created: 2021-12-03
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_CALLBACK_HPP)
#define RR_UTIL_CALLBACK_HPP

#include <Rr/Util/Callable.hpp>
#include <Rr/Trait/Conditional.hpp>
#include <Rr/Trait/Sync.hpp>
#include <Rr/Util/Sync.hpp>

namespace Rr {
namespace Util {

template <typename Tsignature, template <class ...> class Tcontainer>
using CallbackTable = Tcontainer<Rr::Util::Callable<Tsignature>>;

template <class Tsignature, class Tsync>
struct Observer {
	static constexpr bool kConst = Rr::Trait::Fn<Tsignature>::kConst;

	bool enabled;
	typename Rr::Trait::Conditional<Tsync::kGroup == 0, typename Rr::Util::Sync::MockSharedMutex, typename Tsync::Type>::Type mutex;
	Rr::Util::Callable<Tsignature> &callable;
};

///
/// @brief GrowingCallbackTable
///
template <class Tsignature, template<class...> class Tcontainer, class Tsync = Rr::Trait::TsyncMut<0>>
class ObserverTable : protected Tcontainer<Observer<Tsignature, Tsync>> {
protected:
	using BaseType = Tcontainer<Observer<Tsignature, Tsync>>;

	using BaseType::begin;
	using BaseType::end;
	CallbackTable<Tsignature, Tcontainer> callbackTable;
public:
	using BaseType::Tcontainer;
	using BaseType::back;

	template <class ...Ta>
	Observer<Tsignature, Tsync> &construct(Ta &&...aArgs)
	{
		return construct(true, static_cast<Ta &&>(aArgs)...);
	}

	template <class ...Ta>
	Observer<Tsignature, Tsync> &construct(bool aEnabled, Ta &&...aArgs)
	{
		callbackTable.emplace_back(static_cast<Ta &&>(aArgs)...);
		BaseType::emplace_back(Observer<Tsignature, Tsync>{aEnabled, {}, callbackTable.back()});
		return BaseType::back();
	}
};

}  // namespace Util
}  // namespace Rr

#endif // RR_UTIL_CALLBACK_HPP
