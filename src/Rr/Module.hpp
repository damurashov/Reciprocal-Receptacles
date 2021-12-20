//
// Module.hpp
//
// Created: 2021-12-14
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_MODULE_HPP)
#define RR_MODULE_HPP

#include <Rr/Key.hpp>
#include <Rr/Trait/Move.hpp>

namespace Rr {

template <class Tsignature, class Ttopic, template <class...> class Tstorage, class Tsync>
class Module : public Rr::Key<Tsignature, Ttopic, Tstorage, Tsync> {
private:
	using KeyType = Rr::Key<Tsignature, Ttopic, Tstorage, Tsync>;

	using TableIterator = decltype (KeyType::WrapperTableType::asSharedLockWrap().getInstance().begin());
public:

	///
	/// @brief A wrapper over the iterator pertaining to the static table
	/// storing callable instances. Using LockWrap<...> instances can be rather
	/// cumbersome. This iterator encapsulates all the complexities that go
	/// along. Not thread safe.
	///
	/// The principle is that when the underlying instance is referenced by`->`
	/// or `*`, the lock gets acquired, which is expressed in the form of
	/// allocating a new Rr::Util::LockWrap<...> instance. When the wrapped
	/// iterator is increased, the lock gets released. So is it on object
	/// destruction phase.
	///
	class Iterator {
	private:
		TableIterator it;
		TableIterator itEnd;

		using LockWrapType = typename Rr::Trait::RemoveReference<decltype(it->asLockWrap())>::Type;
		using InstanceType = typename Rr::Trait::RemoveReference<decltype(it->asLockWrap().getInstance())>::Type;

		LockWrapType *callableLockWrap;

	private:
		void acquireLock();
		void releaseLock();

		bool isLock()
		{
			return callableLockWrap != nullptr;
		}

		Iterator() = delete;

	public:
		Iterator(TableIterator aIt, TableIterator aItEnd): it{aIt}, itEnd{aItEnd}, callableLockWrap{nullptr}
		{
		}

		~Iterator()
		{
			releaseLock();
		}

		Iterator(const Iterator &aIterator): it{aIterator.it}, itEnd{aIterator.itEnd}, callableLockWrap{nullptr}
		{
			const_cast<Iterator *>(&aIterator)->releaseLock();  // In case when a unique lock is used instead of read (shared) lock, so we won't acquire the same mutex twice
		}

		Iterator(Iterator &&aIterator): it{Rr::Trait::move(aIterator.it)}, itEnd{Rr::Trait::move(aIterator.itEnd)}, callableLockWrap{aIterator.callableLockWrap}
		{
			aIterator.callableLockWrap = nullptr;
		}

		Iterator &operator=(Iterator &&aIterator);
		Iterator &operator++();
		InstanceType &operator*();
		InstanceType *operator->();

		bool operator==(const Iterator &aIterator)
		{
			return it == aIterator.it;
		}

		bool operator!=(const Iterator &aIterator)
		{
			return it != aIterator.it;
		}
	};

	///
	/// @brief Iterable wrapper. \see getIterators()
	///
	///
	struct Iterators {
		Iterator itBegin;
		Iterator itEnd;

		Iterator &begin()
		{
			return itBegin;
		}

		Iterator &end()
		{
			return itEnd;
		}
	};

	///
	/// @brief A shortcut enabling us to get ourselves a wrapper suitable for
	/// use in range-based `for` loop.
	///
	/// The reason why we make is so much compicated is that it must be certain
	/// that "begin" and "end" iterators are acquired atomically. After that,
	/// the list of callables will only grow w/o relocating the previously
	/// stored instances into a new memory. Therefore, the iterators will remain
	/// valid, and there is no need to keep the table locked.
	///
	/// @return Iterators A pair of iterators
	///
	static Iterators getIterators()
	{
		auto tableLockWrap = KeyType::WrapperTableType::asSharedLockWrap();
		return Iterators{{tableLockWrap.getInstance().begin(), tableLockWrap.getInstance().end()},
			{tableLockWrap.getInstance().end(), tableLockWrap.getInstance().end()}};
	}

	using KeyType::KeyType;

	using KeyType::setEnabled;
	using KeyType::notify;
};

}  // namespace Rr

#include "Module.impl"

#endif // RR_MODULE_HPP
