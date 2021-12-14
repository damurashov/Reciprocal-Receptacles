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

template <class T1, class T2>
bool operator==(const T1&, const T2&);

template <class T1, class T2>
bool operator!=(const T1&, const T2&);

namespace Rr {

template <class Tsignature, class Ttopic, template <class...> class Tstorage, class Tsync>
class Module : public Rr::Key<Tsignature, Ttopic, Tstorage, Tsync> {
private:
	using KeyType = Rr::Key<Tsignature, Ttopic, Tstorage, Tsync>;

	using TableIterator = decltype (KeyType::WrapperTableType::asSharedLockWrap().getInstance().begin());
public:

	class Iterator {
		TableIterator it;
		decltype(it->asLockWrap()) callableLockWrap;
	public:
		Iterator(TableIterator aIt): it{aIt}, callableLockWrap{it->asLockWrap()}
		{
		}

		Iterator(Iterator &&aIterator): it{aIterator.it}, callableLockWrap{aIterator.callableLockWrap}
		{
		}

		Iterator &operator=(Iterator &&aIterator)
		{
			it = Rr::Trait::move(aIterator.it);
			callableLockWrap = Rr::Trait::move(aIterator.callableLockWrap);
			return *this;
		}

		Iterator &operator++()
		{
			++it;
			return *this;
		}

		decltype(callableLockWrap.getInstance()) &operator*()
		{
			return callableLockWrap.getInstance();
		}

		decltype(&callableLockWrap.getInstance()) operator->()
		{
			return &callableLockWrap.getInstance();
		}

		friend bool operator==<>(const Iterator &aLhs, const Iterator &aRhs);
		friend bool operator!=<>(const Iterator &aLhs, const Iterator &aRhs);
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

// impl

template <class Tsignature, class Ttopic, template <class...> class Tstorage, class Tsync>
bool operator==(const typename Rr::Module<Tsignature, Ttopic, Tstorage, Tsync>::Iterator &aLhs,
	const typename Rr::Module<Tsignature, Ttopic, Tstorage, Tsync>::Iterator &aRhs)
{
	return aLhs.it == aRhs.it;
}

template <class Tsignature, class Ttopic, template <class...> class Tstorage, class Tsync>
bool operator!=(const typename Rr::Module<Tsignature, Ttopic, Tstorage, Tsync>::Iterator &aLhs,
	const typename Rr::Module<Tsignature, Ttopic, Tstorage, Tsync>::Iterator &aRhs)
{
	return aLhs.it != aRhs.it;
}

#endif // RR_MODULE_HPP
