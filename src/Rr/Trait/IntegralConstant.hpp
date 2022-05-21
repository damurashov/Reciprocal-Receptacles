//
// IntegralConstant.hpp
//
// Created: 2021-12-13
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_INTEGRALCONSTANT_HPP)
#define RR_TRAIT_INTEGRALCONSTANT_HPP

namespace Rr {
namespace Trait {

template<class T, T Vval>
struct IntegralConstant {
	static constexpr T value = Vval;
};

struct FalseType : IntegralConstant<bool, false> {
};

struct TrueType : IntegralConstant<bool, true> {
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_INTEGRALCONSTANT_HPP
