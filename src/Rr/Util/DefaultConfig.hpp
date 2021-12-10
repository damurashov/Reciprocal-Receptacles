//
// DefaultConfig.hpp
//
// Author: Dmitry Murashov
//   mail: dmtr D murashov A gmail D com (Obfuscated to protect from spambots. Replace "D" with ".", "A" with "@")
//
// Provides default configuration for the library for cases when for some reason
// it should be configured w\o the use of CMake, i.e. manually.
//

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
