//
// Key.hpp
//
// Created: 2021-12-07
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_KEY_HPP)
#define RR_KEY_HPP

#include <Rr/Util/CallableTable.hpp>

namespace Rr {

template <class Tsignature, class Ttopic, template <class...> class Tstorage, class Tsync>
class Key :
	protected Rr::Util::Callable<Tsignature>,
	protected Rr::Util::SyncedCallableWrapper<Tsignature, Tsync>
{
private:
	using WrapperTableType = typename Rr::Util::SyncedCallableWrapperStaticTable<Tsignature, Ttopic, Tstorage, Tsync>;

protected:
	static void registerTable(Key &aInstance)
	{
		WrapperTableType::asUniqueLockWrap().getInstance().push_back(aInstance);
	}

public:
	~Key() {
		setEnabled(false);  // Safely disable the instance, so no attempts to invoke it will be made
	}

	void setEnabled(bool aEnabled)
	{
		typename Rr::Trait::UniqueLockType<Tsync>::Type lock{this->getSyncPrimitive()};
		Rr::Util::SyncedCallableWrapper<Tsignature, Tsync>::setEnabled(aEnabled);
	}

	Key() = delete;

	template <class ...Ta>
	Key(Ta &&...aArgs):
		Key(true, static_cast<Ta &&>(aArgs)...)
	{
	}

	template <class ...Ta>
	Key(bool aEnabled, Ta &&...aArgs):
		Rr::Util::Callable<Tsignature>(static_cast<Ta &&>(aArgs)...),
		Rr::Util::SyncedCallableWrapper<Tsignature, Tsync>(aEnabled, *this)
	{
		registerTable(*this);
	}

	template <class ...Ta>
	static void notify(Ta &&...aArgs)
	{
		using IteratorType = decltype(WrapperTableType::asSharedLockWrap().getInstance().begin());

		IteratorType itBegin;
		IteratorType itEnd;

		// The iterator boundaries will remain valid. Hence the short duration of table lock.
		{
			auto tableLock = WrapperTableType::asSharedLockWrap();

			itBegin = tableLock.getInstance().begin();
			itEnd = tableLock.getInstance().end();
		}

		for (auto it = itBegin; it != itEnd; ++it) {
			auto lockWrap = it->asLockWrap();
			auto &instance = lockWrap.getInstance();

			if (instance.isEnabled()) {
				instance(static_cast<Ta &&>(aArgs)...);
			}
		}
	}
};

}  // namespace Rr

#include "Key.impl"

#endif // RR_KEY_HPP
