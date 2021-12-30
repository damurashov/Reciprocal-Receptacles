//
// DefaultConfig.hpp
//
// Author: Dmitry Murashov
//   mail: dmtr D murashov A gmail D com (Obfuscated to protect from spambots. Replace "D" with ".", "A" with "@")
//
// Provides default configuration for the library for cases when for some reason
// it should be configured w\o the use of CMake, i.e. manually.
//

#if !defined(RR_UTIL_DEFAULTCONFIG_HPP)
#define RR_UTIL_DEFAULTCONFIG_HPP

#if __cplusplus < 201103L
# error " RR requires at least C++11"
#endif

// Use C++ STL

#if !defined(RRO_STL_USED)
#define RRO_STL_USED 1
#include <type_traits>  // Test
#endif // RRO_STL_USED

// Use static_cast for callable member casting.

// For example, modern gcc allows one to convert, say, void(D::*)(int) to
// void(Z::*)() and USE the latter although the types are not related in any
// way. With this flag in place, one would be able to use the library in the
// same way, but with deriving the callable objects from Rr::RrObject
// https://stackoverflow.com/questions/37662100/calling-derived-class-through-base-class-function-pointer
#if !defined(RRO_STATIC_CAST_FN_CONVERSION)
# define RRO_STATIC_CAST_FN_CONVERSION 0
#endif

// Enable <cassert>

#if !defined(RRO_ENABLE_CASSERT)
# define RRO_ENABLE_CASSERT 1
#endif

#if RRO_ENABLE_CASSERT
# include <cassert>
# define rr_assert assert
#else
# define rr_assert(...)
#endif

// Enable debug

#if !defined RRO_ENABLE_DEBUG
# define RRO_ENABLE_DEBUG 0
#endif



#if RRO_ENABLE_DEBUG
# if !RRO_STL_USED
#  error "STL is required for debug output"
# endif
# include <iostream>
namespace Rr {
namespace Util {
template <class ...Ta>
inline void debug(Ta &&...aArgs)
{
	using VtList = int[];
	(void)VtList{(void(std::cout << static_cast<Ta &&>(aArgs)), 0)...};
	std::cout << std::endl;
}
}  // namespace Util
}  // namespace Rr
#else
namespace Rr {
namespace Util {
template <class ...Ta>
inline void debug(Ta...)
{
}
}  // namespace Util
}  // namespace Rr
#endif

namespace Rr {
struct Object {};
}  // Rr

#if RRO_STATIC_CAST_FN_CONVERSION
# define rr_fn_cast static_cast
#else
# define rr_fn_cast reinterpret_cast
#endif


#endif // RR_UTIL_DEFAULTCONFIG_HPP
