//
// NoMember.hpp
//
// Created on: 2022-01-15
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_REFL_NOMEMBER_HPP)
#define RR_REFL_NOMEMBER_HPP

namespace Rr {
namespace Refl {

///
/// @brief Used as a fallback type for various SFINAE inferences
///
struct NoMember{
};

}  // namespace Refl
}  // namespace Rr

#endif // RR_REFL_NOMEMBER_HPP
