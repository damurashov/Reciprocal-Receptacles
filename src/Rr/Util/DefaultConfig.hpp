//
// DefaultConfig.hpp
//
// Author: Dmitry Murashov 
//   mail: dmtr D murashov A gmail D com (Obfuscated to protect from spambots. Replace "D" with ".", "A" with "@")
//
// Provides default configuration for the library for cases when for some reason
// it should be configured w\o the use of CMake, i.e. manually.
//


// Use C++ STL
#if !defined(RRO_STL_USED)
#define RRO_STL_USED 1
#endif // RRO_STL_USED
