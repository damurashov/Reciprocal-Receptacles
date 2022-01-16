//
// Declval.hpp
//
// Created on: 2022-01-16
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_DECLVAL_HPP)
#define RR_TRAIT_DECLVAL_HPP

namespace Rr {
namespace Trait {
namespace DeclvalImpl {

constexpr const char *kDummy = "";

}  // namespace DeclvalImpl

template <class T>
constexpr T &declval()
{
    return *const_cast<T *>(reinterpret_cast<T const *const>(DeclvalImpl::kDummy));
}

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_DECLVAL_HPP
