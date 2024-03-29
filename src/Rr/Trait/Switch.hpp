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
#include <Rr/Trait/Conditional.hpp>
#include <Rr/Trait/LastType.hpp>

namespace Rr {
namespace Trait {

namespace SwitchImpl {

template <bool Ifound, int Ipos, class ...Ta>
struct GetType {
	using Type = typename Rr::Trait::LastType<Ta...>::Type;
};

template <int Ipos, class ...Ta>
struct GetType<true, Ipos, Ta...> {
	using Type = typename IntToType<(int)Ipos, Ta...>::Type;
};

}  // namespace SwitchImpl

template <int, class T1, class T2>
struct SwitchInt;

template <int Ival, int ...Ia, template <int ...> class TiList, class ...Ta, template <class...> class TtList>
struct SwitchInt<Ival, TiList<Ia...>, TtList<Ta...>> {
	static constexpr auto kPos = IntToPos<Ival, Ia...>::value;

	// static_assert(kPos != -1, "");
	static_assert((sizeof...(Ta) - sizeof...(Ia) <= 1) && (sizeof...(Ta) - sizeof...(Ia) >= 0),
		"Wrong number of arguments for Switch. Expected (nTypes - nInts = {0; 1})");

	using Type = typename SwitchImpl::GetType<kPos >= 0, kPos, Ta...>::Type;
};

template <int Ival, int ...Ia, template <int ...> class TiList, class ...Ta, template <class...> class TtList>
struct SwitchInt<Ival, TtList<Ta...>, TiList<Ia...>> : SwitchInt<Ival, TiList<Ia...>, TtList<Ta...>>  {
};

template <class Ti, Ti, class, class>
struct Switch;

template <class Tintegral, Tintegral Ival, Tintegral ...Ia, template <Tintegral ...> class TiList, class ...Ta, template <class...> class TtList>
struct Switch<Tintegral, Ival, TtList<Ta...>, TiList<Ia...>> {
	static constexpr auto kPos = IntegralToPos<Tintegral, Ival, Ia...>::value;

	// static_assert(kPos != -1, "");
	static_assert((sizeof...(Ta) - sizeof...(Ia) <= 1) && (sizeof...(Ta) - sizeof...(Ia) >= 0),
		"Wrong number of arguments for Switch. Expected (nTypes - nInts = {0; 1})");

	using Type = typename SwitchImpl::GetType<kPos >= 0, kPos, Ta...>::Type;
};

template <class Tintegral, Tintegral Ival, Tintegral ...Ia, template <Tintegral ...> class TiList, class ...Ta, template <class...> class TtList>
struct Switch<Tintegral, Ival, TiList<Ia...>, TtList<Ta...>> : Switch<Tintegral, Ival, TtList<Ta...>, TiList<Ia...>> {
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_SWITCH_HPP
