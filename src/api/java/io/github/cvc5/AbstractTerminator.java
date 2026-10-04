/******************************************************************************
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2026 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * The cvc5 java API.
 */

package io.github.cvc5;

/**
 * A termination callback.
 *
 * A terminator is connected to a solver via
 * {@link Solver#setTerminator(AbstractTerminator)}. While the solver is
 * running (e.g., during a call to {@link Solver#checkSat()}), it periodically
 * calls {@link #terminate()} to determine whether the current call should be
 * terminated. If {@link #terminate()} returns {@code true}, the solver
 * interrupts the current call as soon as possible and returns an unknown
 * result with explanation {@link UnknownExplanation#INTERRUPTED}. The solver
 * remains usable afterwards.
 *
 * Note that {@link #terminate()} is always invoked from the thread that runs
 * the solver. Since it is called frequently, it should be cheap to compute.
 * A typical implementation reads a flag that is set from another thread, or
 * checks whether a deadline has passed.
 */
public abstract class AbstractTerminator
{
  /**
   * Create terminator instance.
   */
  public AbstractTerminator() {}

  /**
   * Determine whether the associated solver should be terminated.
   *
   * This method is called periodically while the solver is running.
   *
   * @return True if the current call of the associated solver should be
   *         terminated.
   */
  public abstract boolean terminate();
}
