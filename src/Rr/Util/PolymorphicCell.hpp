//
// PolymorphicCell.hpp
//
// Created: 2021-12-03
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_POLYMORPHIC_CELL)
#define RR_UTIL_POLYMORPHIC_CELL

namespace Rr {
namespace Util {

template <unsigned N1, unsigned N2, bool Ngt = (N1 > N2)>
struct Max2;

template <unsigned N1, unsigned N2>
struct Max2<N1, N2, true> {
	static constexpr auto value = N1;
};

template <unsigned N1, unsigned N2>
struct Max2<N1, N2, false> {
	static constexpr auto value = N2;
};

template <unsigned N, unsigned ...Nsizeofs>
struct MaxVar {
	static constexpr auto value = Max2<N, MaxVar<(Nsizeofs)...>::value>::value;
};

template <unsigned N>
struct MaxVar<N> {
	static constexpr auto value = N;
};

template <class T, class ...Targs>
struct MaxSizeof {
	static constexpr auto value = MaxVar<sizeof(T), sizeof(Targs)...>::value;
};

template <class Tbase, class ...Tderived>
struct PolymorphicCell {
	static constexpr unsigned size();

	///
	/// @brief  Helper method to create pointer hints for the constructor
	///
	template <class T>
	static constexpr T *ptr();

	operator Tbase&();

	///
	/// @brief Construct a new Polymorphic Cell object
	///
	/// @tparam T Hint for auto type deduction to set the exact type that should be constructed
	/// @tparam Targs Arguments for T constructor
	///
	template <class T, class ...Targs>
	PolymorphicCell(T *, Targs &&...);
private:
	char memory[size()];  ///< Storage for anything that will be constructed
};

}  // namespace Util
}  // namespace Rr

#include "PolymorphicCell.impl"

#endif // RR_UTIL_POLYMORPHIC_CELL
