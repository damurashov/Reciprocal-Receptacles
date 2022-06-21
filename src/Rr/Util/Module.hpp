//
// Module.hpp
//
// Created on: Jun 21, 2022
//     Author: Dmitry Murashov (dmtr DOT murashov AT gmail DOT com)
//

#if !defined(RR_UTIL_MODULE_HPP_)
#define RR_UTIL_MODULE_HPP_

#include <Rr/Sync/Storage.hpp>

namespace Rr {
namespace Util {
namespace ModuleImpl {

/// \brief Encapsulation of shared access static storage
/// \tparam Tmod  - module
/// \tparam Tsytr - sync traits
/// \tparam Tcont - container type
/// \tparam Ttop  - topic (for separating storages storing same types)
///
template <class Tmod, class Tsytr, template <class...> class Tcont, class Ttop = void>
struct ModuleRegistry {
private:
	static Rr::Sync::Storage<Tmod, Tsytr, Tcont> storage;

public:

	using ValueType = Tmod;
	using SyncTrait = Tsytr;

	static auto getIterators() -> decltype(storage.getIterators())
	{
		return storage.getIterators();
	}

	static auto reg(Tmod &aMod, bool aEnabled = true) -> decltype(storage.reg(aMod, aEnabled))
	{
		return storage.reg(aMod, aEnabled);
	}

	/// \brief Iterate over registered modules
	/// \tparam Tcb - callable type
	///
	template <class Tcb>
	static void visitAll(Tcb &&aCb)
	{
		for (auto &mod : storage.getIterators()) {
			aCb(mod);
		}
	}

	/// \brief Iterate over N registered modules
	/// \tparam Tcb - callable type
	///
	template <class Tcb>
	static void visitN(Tcb &&aCb, Rr::usize aN)
	{
		for (auto &mod : storage.getIterators()) {
			if (aN == 0) {
				break;
			}

			aCb(mod);
			--aN;
		}
	}
};

template <class Tmod, class Tsytr, template <class...> class Tcont, class Ttop>
Rr::Sync::Storage<Tmod, Tsytr, Tcont> ModuleRegistry<Tmod, Tsytr, Tcont, Ttop>::storage {};

/// \brief Provides coupling w/ an instance of SharedAccess<...>. The instance may be added into instance registry
/// either way, therefore, using MakeModule as a base class is not necessary.
/// \tparam Treg  - registry type
/// \tparam Thint - hint for the actual type
///
template <class Treg, class Thint = typename Treg::ValueType>
struct MakeModule {
private:
	typename Rr::Sync::SharedAccess<typename Treg::ValueType, typename Treg::SyncTrait> &sharedAccess;

public:
	void setEnabled(bool aEnabled)
	{
		// TODO: move `setEnabled` method into `SharedAccess<...>`
		Rr::Sync::Policy::SharedAccess<typename Treg::SyncTrait>::PrimitiveOps::lock(sharedAccess.primitive);
		sharedAccess.stored = (aEnabled ? static_cast<Thint *>(this) : nullptr);
		Rr::Sync::Policy::SharedAccess<typename Treg::SyncTrait>::PrimitiveOps::unlock(sharedAccess.primitive);
	}

	MakeModule(bool aEnabled = true) : sharedAccess{Treg::reg(*static_cast<Thint *>(this), aEnabled)}
	{
	}

	~MakeModule()
	{
		setEnabled(false);
	}
};

}  // namespace ModuleImpl

using ModuleImpl::MakeModule;
using ModuleImpl::ModuleRegistry;

}  // namespace Util
}  // namespace Rr

#endif // RR_UTIL_MODULE_HPP_
