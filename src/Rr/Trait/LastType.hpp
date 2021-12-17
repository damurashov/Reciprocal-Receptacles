//
// LastType.hpp
//
// Created: 2021-12-16
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_LASTTYPE_HPP)
#define RR_UTIL_LASTTYPE_HPP

namespace Rr {
namespace Trait {

namespace LastTypeImpl {
struct Stop;

template <class T1, class T2, class ...Ta>
struct LastType {
	using Type = typename LastType<T2, Ta...>::Type;
};

template <class T1>
struct LastType<T1, Stop> {
	using Type = T1;
};

}  // namespace LastTypeImpl

template <class T, class ...Ta>
struct LastType : LastTypeImpl::LastType<T, Ta..., typename LastTypeImpl::Stop> {
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_UTIL_LASTTYPE_HPP
