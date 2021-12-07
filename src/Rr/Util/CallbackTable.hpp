//
// Callback.hpp
//
// Created: 2021-12-03
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_CALLBACK_HPP)
#define RR_UTIL_CALLBACK_HPP

#include <Rr/Util/Callable.hpp>

namespace Rr {
namespace Util {

template <class Tsignature, template<class...> class Tcontainer>
using CallbackTable = Tcontainer<Rr::Util::Callable<Tsignature>>;

}  // namespace Util
}  // namespace Rr

#endif // RR_UTIL_CALLBACK_HPP
