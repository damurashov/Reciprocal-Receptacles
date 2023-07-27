//
// PointerStorage.hpp
//
// Created on: Jul 27, 2023
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL> <DOT> <COM>)
//

#ifndef RR_CONT_POINTERSTORAGE_HPP
#define RR_CONT_POINTERSTORAGE_HPP

#include <Rr/Port/Log.hpp>

namespace Rr {
namespace Cont {

/// Stores raw `void *` pointers
/// \tparam TContainer - a generic container, must define the following methods:
///  - back()
///  - push_back()
///  - pop_back()
///  - begin()
///  - end()
/// \details
/// The following limitations must be considered:
///  - Removing from / adding to the container is slow
template <class TContainer>
class PointerStorage {
private:
	TContainer container;

public:
	bool tryAdd(void *aPointer)
	{
		for (void *pointer : container) {
			if (pointer == aPointer) {
				Rr::Port::Log::tryPrint(Rr::Port::LogLevel::Warning,
					"`PointerStorage`: an attempt to add an already existing element, ignoring");

				return false;
			}
		}

		container.push_back(aPointer);

		return true;
	}

	bool tryRemove(void *aPointer)
	{
		for (void *&pointer : container) {
			if (pointer == aPointer) {
				// Swap w/ the latest element, and remove
				pointer = container.back();
				container.pop_back();

				return true;
			}
		}

 		Rr::Port::Log::tryPrint(Rr::Port::LogLevel::Warning,
			"`PointerStorage`: an attempt to remove a non-existing element, ignoring");

		return false;
	}

	auto begin() -> decltype(container.begin())
	{
		return container.begin();
	}

	auto end() -> decltype(container.end())
	{
		return container.end();
	}
};

}  // Cont
}  // Rr

#endif // RR_CONT_POINTERSTORAGE_HPP
