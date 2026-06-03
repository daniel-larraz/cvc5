/******************************************************************************
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2026 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Singleton CoCoA global manager.
 *
 * A CoCoA global manager must be created before most CoCoA operations are
 * performed. It must be created **exactly once** (per process, we believe);
 * creating it multiple times raises an exception. It is never directly used by
 * our code: no component of our code accesses it.
 *
 * We store it in a function-local static, so it is constructed lazily on first
 * use and shared across the entire process (and in particular, all cvc5
 * solvers). C++11 guarantees this initialization happens exactly once, even if
 * multiple threads race to trigger it.
 *
 * Rather than expose the manager (or a free "init" function) and rely on every
 * CoCoA entry point remembering to call it, we make initialization a side
 * effect of *construction*: see CocoaInitializer below.
 */

#include "cvc5_public.h"

#ifdef CVC5_USE_COCOA

#ifndef CVC5__UTIL__COCOA_GLOBALS_H
#define CVC5__UTIL__COCOA_GLOBALS_H

namespace cvc5::internal {

/**
 * Guard that guarantees the singleton CoCoA global manager exists.
 *
 * Constructing a CocoaInitializer initializes the manager (exactly once per
 * process, thread-safely). It does not own the manager: the manager lives for
 * the lifetime of the process, so destroying the guard is a no-op.
 *
 * The intent is that every class that constructs or manipulates CoCoA objects
 * derives (privately) from this guard. Because base classes are constructed
 * before the derived object and its members, the manager is guaranteed to exist
 * before any CoCoA object the derived class owns. This makes "using CoCoA" and
 * "initializing CoCoA" the same act: one cannot construct a CoCoA-using object
 * without first running this constructor, so no entry point has to remember to
 * initialize anything.
 *
 * Test code that builds CoCoA objects by hand (rather than through one of those
 * classes) may construct a CocoaInitializer directly as a local guard.
 */
class CocoaInitializer
{
 public:
  CocoaInitializer();
};

}  // namespace cvc5::internal

#endif /* CVC5__UTIL__COCOA_GLOBALS_H */

#endif /* CVC5_USE_COCOA */
