//
// Module.cpp
//
// Created on: Jun 21, 2022
//     Author: Dmitry Murashov (dmtr DOT murashov AT gmail DOT com)
//

#include <cassert>
#include <doctest/doctest.h>
#include <Rr/Util/Module.hpp>
#include <mutex>
#include <list>

struct MutexLockTrait {
	static constexpr auto kStoragePolicy = Rr::Sync::Policy::Type::Mutex;
	static constexpr auto kSharedAccessPolicy = Rr::Sync::Policy::Type::None;
	using Mutex = std::mutex;
};

struct Serial : Rr::Util::ModuleRegistry<Serial, MutexLockTrait, std::list> {
	std::size_t read(void *aBuffer, std::size_t aBufferSize)
	{
		(void)aBuffer;
		(void)aBufferSize;

		return 1;
	}
};

struct Uart : Serial, Rr::Util::MakeModule<Serial, Uart> {

	std::size_t deviceId() const
	{
		return 1;
	}
};

struct Usb : Serial {
};

TEST_CASE("Module, const") {
}

TEST_CASE("Module, basic usage")
{
	static constexpr auto knInstances = 3;
	Uart uarts[knInstances] = {{}};

	for (auto &u : uarts) {
		u.setEnabled(true);
		u.asLockWrap();
	}

	SUBCASE("Iterate")
	{
		std::size_t n = 0;

		Uart::visitAll(
			[&n](Serial &aUart)
			{
				(void)aUart;
				++n;
			});
		CHECK(knInstances == n);
	}

	SUBCASE("iterate disable")
	{
		uarts[1].setEnabled(false);
		std::size_t n = 0;

		Uart::visitAll(
			[&n](Serial &aUart)
			{
				(void)aUart;
				++n;
			});
		CHECK(knInstances - 1 == n);

		uarts[1].setEnabled(true);
		n = 0;
		Uart::visitAll(
			[&n](Serial &aUart)
			{
				(void)aUart;
				++n;
			});
		CHECK(knInstances == n);
	}

	SUBCASE("Iterate over heterogeneous instances")
	{
		Usb usb;
		Serial::reg(usb, true);
		unsigned n = 0;

		Uart::visitAll(
			[&n](Serial &aSerial)
			{
				(void)aSerial;
				++n;
			}
		);

		CHECK(knInstances + 1 == n);
	}

}
