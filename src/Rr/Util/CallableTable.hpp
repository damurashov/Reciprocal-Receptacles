//
// CallableTable.hpp
//
// Created: 2021-12-03
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_CALLBACK_HPP)
#define RR_UTIL_CALLBACK_HPP

#include <Rr/Util/SyncedCallable.hpp>

namespace Rr {
namespace Util {

template <typename Tsignature, template <class ...> class Tcontainer>
using CallableTable = Tcontainer<Rr::Util::Callable<Tsignature>>;

template <class Tsignature, template<class...> class Tcontainer, class Tsync>
using SyncedCallableTable = Tcontainer<typename Rr::Util::SyncedCallableType<Tsignature, Tsync>::Type>;

}  // namespace Util
}  // namespace Rr

#endif // RR_UTIL_CALLBACK_HPP
