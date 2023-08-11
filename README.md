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
   very same way STL or ETL does so in an instance of `Callable<...>` type;
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

Under the hood, it's just a slightly obscure (due to the use of generic
programming) subscription mechanism, so you can use it for pretty much
anything.

But don't use it for anything else, as, most likely, you will create messy
code. And don't ask me why I know that.

# Examples
