/******************************************************************************
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2026 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Singleton CoCoA global manager
 */

#ifdef CVC5_USE_COCOA

#include "util/cocoa_globals.h"

#include <CoCoA/GlobalManager.H>

namespace cvc5::internal {

namespace {

/**
 * Returns the singleton CoCoA global manager, constructing it on the first
 * call. The initialization is thread-safe and happens exactly once per process
 * (C++11 function-local static semantics). Kept file-private so that the only
 * way to initialize CoCoA is to construct a CocoaInitializer.
 */
CoCoA::GlobalManager& cocoaGlobalManager()
{
  static CoCoA::GlobalManager s_cocoaGlobalManager;
  return s_cocoaGlobalManager;
}

}  // namespace

CocoaInitializer::CocoaInitializer() { cocoaGlobalManager(); }

}  // namespace cvc5::internal

#endif /* CVC5_USE_COCOA */
