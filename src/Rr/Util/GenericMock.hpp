//
// GenericMock.hpp
//
// Created: 2021-12-16
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_GENERICMOCK_HPP)
#define RR_UTIL_GENERICMOCK_HPP

namespace Rr {
namespace Util {

struct GenericMock {

	template <class ...Ts>
	constexpr inline GenericMock(Ts&&...)
	{
	}

	constexpr inline GenericMock()
	{
	}

	constexpr inline GenericMock getSyncPrimitive()
	{
		return {};
	}
};

}  // namespace Util
}  // namespace Rr

#endif // RR_UTIL_GENERICMOCK_HPP
