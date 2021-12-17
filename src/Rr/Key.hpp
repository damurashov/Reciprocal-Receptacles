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

///
/// @brief  No-return notifications. Essentially, a callback table
///
/// @tparam Tsignature Signature of a method in form `void(int) [const]`
/// @tparam Ttopic     Mock structure to provide a distinction
/// @tparam Tstorage   Generic std::list-like container. std::list will fit perfectly
/// @tparam Tsync      Synchronization traits. TODO: provide exhaustive description
///
template <class Tsignature, class Ttopic, template <class...> class Tstorage, class Tsync>
class Key :
	protected Rr::Util::Callable<Tsignature>,
	protected Rr::Util::SyncedCallableWrapper<Tsignature, Tsync>
{
private:

	///
	/// @brief Puts the pointer to an instance to a callable table. The pointer
	/// is stored statically, so constructing an instance does bear a little
	/// overhead.
	///
	/// @param aInstance
	///
	static void registerTable(Key &aInstance)
	{
		WrapperTableType::asUniqueLockWrap().getInstance().push_back(aInstance);
	}
protected:
	using WrapperTableType = typename Rr::Util::SyncedCallableWrapperStaticTable<Tsignature, Ttopic, Tstorage, Tsync>;
	using Rr::Util::SyncedCallableWrapper<Tsignature, Tsync>::SetEnabledLockType;  ///< Different lock approaches are used for different actions

public:
	~Key() {
		setEnabled(false);  // Safely disable the instance, so no attempts to invoke it will be made
	}

	///
	/// @brief When disabled, the instance will stop receiving any
	/// notifications. Automatically disabled on destruction; however, it gives
	/// no guarantees, so it is better to disable it manually. Providing that the
	/// Tsync trait is formed correctly, the method is thread-safe.
	///
	/// @param aEnabled
	///
	void setEnabled(bool aEnabled);

	Key() = delete;

	///
	/// @brief Construct a new Key object
	///
	/// @tparam Ta
	///
	/// @param  aArgs (1) function pointer / (1) method pointer, (2) instance
	/// pointer. NOTE: for overloaded methods (i.e. with reference /
	/// cv-quailifiers) the method pointer should be explicitly casted to an
	/// appropriate form.
	///
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
	static void notify(Ta &&...aArgs);
};

}  // namespace Rr

#include "Key.impl"

#endif // RR_KEY_HPP
