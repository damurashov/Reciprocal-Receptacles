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

	class Iterator {
	private:
		TableIterator it;

		using LockWrapType = typename Rr::Trait::RemoveReference<decltype(it->asLockWrap())>::Type;
		using InstanceType = typename Rr::Trait::RemoveReference<decltype(it->asLockWrap().getInstance())>::Type;

		LockWrapType *callableLockWrap;

	private:
		void acquireLock()
		{
			if (!callableLockWrap) {
				callableLockWrap = new LockWrapType{it->asLockWrap()};
			}
		}

		void releaseLock()
		{
			if (callableLockWrap) {
				delete callableLockWrap;
				callableLockWrap = nullptr;
			}
		}

	public:
		Iterator(TableIterator aIt): it{aIt}, callableLockWrap{nullptr}
		{
		}

		Iterator(const Iterator &aIterator): it{aIterator.it}, callableLockWrap{nullptr}
		{
			const_cast<Iterator *>(&aIterator)->releaseLock();
		}

		Iterator(Iterator &&aIterator): it{aIterator.it}, callableLockWrap{aIterator.callableLockWrap}
		{
			aIterator.callableLockWrap = nullptr;
		}

		Iterator &operator=(Iterator &&aIterator)
		{
			it = Rr::Trait::move(aIterator.it);
			callableLockWrap = aIterator.callableLockWrap;
			aIterator.callableLockWrap = nullptr;

			return *this;
		}

		Iterator &operator++()
		{
			releaseLock();
			++it;
			return *this;
		}

		InstanceType &operator*()
		{
			acquireLock();
			return callableLockWrap->getInstance();
		}

		InstanceType *operator->()
		{
			acquireLock();
			return &callableLockWrap->getInstance();
		}

		bool operator==(const Iterator &aIterator)
		{
			return it == aIterator.it;
		}

		bool operator!=(const Iterator &aIterator)
		{
			return it != aIterator.it;
		}
	};

	class Iterators {
		Iterator itBegin;
		Iterator itEnd;

	public:
		Iterators(TableIterator aBegin, TableIterator aEnd): itBegin{aBegin}, itEnd{aEnd}
		{
		}

		Iterator &begin()
		{
			return itBegin;
		}

		Iterator &end()
		{
			return itEnd;
		}
	};

	static Iterators getIterators()
	{
		auto tableLockWrap = KeyType::WrapperTableType::asSharedLockWrap();
		return Iterators(tableLockWrap.getInstance().begin(), tableLockWrap.getInstance().end());
	}

	using KeyType::KeyType;

	using KeyType::setEnabled;
	using KeyType::notify;
};

}  // namespace Rr

#endif // RR_MODULE_HPP
