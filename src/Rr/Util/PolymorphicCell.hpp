//
// PolymorphicCell.hpp
//
// Created: 2021-12-03
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_POLYMORPHIC_CELL)
#define RR_UTIL_POLYMORPHIC_CELL

#include <Rr/Trait/Max.hpp>

namespace Rr {
namespace Util {

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

// TODO: PolymorphicCell<N>

}  // namespace Util
}  // namespace Rr

#include "PolymorphicCell.impl"

#endif // RR_UTIL_POLYMORPHIC_CELL
