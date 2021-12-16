//
// Switch.hpp
//
// Created: 2021-12-16
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_SWITCH_HPP)
#define RR_TRAIT_SWITCH_HPP

#include <Rr/Trait/IntegralToType.hpp>
#include <Rr/Trait/IntegralIn.hpp>

namespace Rr {
namespace Trait {

namespace SwitchImpl {

template <int, class>
struct TypePack;

template <int Ipos, class Tmock, class ...Ta>
struct TypePack<Ipos, Tmock(Ta...)> {
	using Type = typename IntToType<Ipos, Ta...>::Type;
};

}  // namespace SwitchImpl

template <int Ival, class TtypePack, int ...Ia>
class SwitchInt {
	static constexpr auto kIntPos = IntToPos<Ival, Ia...>::value;
public:
	using Type = typename SwitchImpl::TypePack<kIntPos, TtypePack>::Type;
};

template <class Tintegral, Tintegral Ival, class TtypePack, Tintegral ...Ia>
struct Switch : SwitchInt<(int)Ival, TtypePack, ((int)Ia)...> {
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_SWITCH_HPP
