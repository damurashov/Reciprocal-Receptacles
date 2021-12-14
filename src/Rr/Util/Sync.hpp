//
// Sync.hpp
//
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_SYNC_HPP_)
#define RR_UTIL_SYNC_HPP_

#include <Rr/Util/SyncMock.hpp>

namespace Rr {
namespace Util {

// GroupSync

namespace GroupSyncImpl {

///
/// @brief Static storage for whatever sync primitive is being used
///
/// @tparam Tsync  See Trait/Sync
/// @tparam Igroup If `Igroup != 0`, common (group) lock instance is used
///
template <class Tsync, unsigned Igroup>
struct SyncInstance {
	static typename Tsync::Type value;
};

template <class Tsync, unsigned Igroup>
typename Tsync::Type SyncInstance<Tsync, Igroup>::value;

}  // namespace GroupSyncImpl

///
/// @brief Group lock variant
///
template <class Tsync, unsigned Igroup>
class GroupSync {
	typename Tsync::Type &syncPrimitive;
protected:
	void deleteSyncPrimitive()
	{
	}
public:
	GroupSync(): syncPrimitive(GroupSyncImpl::SyncInstance<Tsync, Igroup>::value) {}
	typename Tsync::Type &getSyncPrimitive()
	{
		return syncPrimitive;
	}
};

///
/// @brief Individual lock variant
///
template <class Tsync>
class GroupSync<Tsync, 0> {
	typename Tsync::Type *syncPrimitive;
protected:
	void deleteSyncPrimitive()
	{
		delete syncPrimitive;
	}
public:
	GroupSync(): syncPrimitive{new typename Tsync::Type()}
	{
	}
	typename Tsync::Type &getSyncPrimitive()
	{
		return *syncPrimitive;
	}
};

}  // namespace Util
}  // namespace Rr

#endif // RR_UTIL_SYNC_HPP_

