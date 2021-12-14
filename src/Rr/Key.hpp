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
	public Rr::Util::SyncedCallableWrapper<Tsignature, Tsync>
{
private:
	using CallableType = typename Rr::Util::Callable<Tsignature>;
	using WrapperType = typename Rr::Util::SyncedCallableWrapper<Tsignature, Tsync>;
	using WrapperTableType = typename Rr::Util::SyncedCallableWrapperStaticTable<Tsignature, Ttopic, Tstorage, Tsync>;

protected:
	static void registerTable(Key &aInstance)
	{
		WrapperTableType::asUniqueLockWrap().getInstance().push_back(aInstance);
	}

public:
	using Rr::Util::SyncedCallableWrapper<Tsignature, Tsync>::setEnabled;
	using Rr::Util::SyncedCallableWrapper<Tsignature, Tsync>::asLockWrap;

	~Key() {
		setEnabled(false);  // Safely disable the instance, so no attempts to invoke it will be made
	}

	Key() = delete;

	template <class ...Ta>
	Key(Ta &&...aArgs): Key(true, static_cast<Ta &&>(aArgs)...)
	{
	}

	template <class ...Ta>
	Key(bool aEnabled, Ta &&...aArgs): CallableType(static_cast<Ta &&>(aArgs)...), WrapperType(aEnabled, *this)
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
			it->asLockWrap().getInstance()(static_cast<Ta &&>(aArgs)...);
		}
	}
};

}  // namespace Rr

#include "Key.impl"

#endif // RR_KEY_HPP
