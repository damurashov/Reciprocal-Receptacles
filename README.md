# RR is a ...

- C++11-compatible;
- Configurable;
- Dependency-free;
- Generic;
- Header-only;
- Lightweight;
- Malloc-free;
- Portable;
- Standard-compliant;
- Thread-safe (if that's what you need it to be);

... event framework for embedded applications. When you need to pass something
b/w different parts of your embedded (and beyond that)  code, and your compiler
supports C++11, RR is the way to go.

# How it works

It all boils down to type erasure:

1. RR stores callbacks to either class methods, or regular functions -- the
very same way STL or ETL does so -- in an instance of `Callable<...>` type;
2. The storage (which type you choose yourself) stores pointers to `Callable`s
   synchronized with a shared mutex;
3. The shared mutex is implemented as a wrapper over a regular mutex (which
   type, once again, you choose through providing RR with a trait specifying
   its type via `typedef/using`). And, of course, you can omit synchronization
   altogether.

And that's pretty much it!

# Why use it

It greatly reduces the amount of boilerplate you have to write for implementing
a subscription mechanism. No need to hatch through your codebase to tie things
together.

# What else can I use it for?

At the end of the day, it's just a slightly obscure (due to the use of generic
programming) subscription mechanism, so you can use it for pretty much
anything.

But don't use it for anything else, as, most likely, you will create messy
code. And don't ask me why I know that.

# Example

```c++

#include <Rr/Util/Event.hpp>

// At first, let's create a trait containing all the necessary fields.
struct SyncTrait {
	using MutexType = std::mutex;

	// Let's store our callables (pointers to those, anyway) in the list
	template <class ...Ts>
	using CallableContainerType = std::list<Ts...>;
};

struct MarkerToDistinguishTopics;

// Define a topic
using Event = typename Rr::Util::Event<void(int &), SyncTrait, MarkerToDistinguishTopics /* optional! */>;

// Here is how you activate a subscription
struct MethodCallable {
	void callable(int &aValue)
	{
		aValue += 1;
	}

	Event event;

	MethodCallable():
		event{&MethodCallable::callable, this}
	{
	}

	~MethodCallable()
	{
		event.setEnabled(false);  // optional. it will be called on destruction of `Event`
	}
};

void callback(int &aValue);

int main(void)
{
	MethodCallable methodCallable{};
	Event event{callback};
	int value = 40;
	Event::notify(aValue);  // value == 42;
}

```
