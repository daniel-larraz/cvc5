/******************************************************************************
 * Top contributors (to current version):
 *   Alex Ozdemir, Aina Niemetz
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2025 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Black box testing of the finite-field related API methods.
 *
 * In its own file because it requires cvc5 to be built with --cocoa.
 */

package tests;

import static io.github.cvc5.Kind.*;
import static io.github.cvc5.RoundingMode.*;
import static org.junit.jupiter.api.Assertions.*;

import io.github.cvc5.*;
import io.github.cvc5.modes.BlockModelsMode;
import io.github.cvc5.modes.LearnedLitType;
import io.github.cvc5.modes.ProofComponent;
import java.math.BigInteger;
import java.util.*;
import java.util.concurrent.atomic.AtomicReference;
import org.junit.jupiter.api.*;
import org.junit.jupiter.api.function.Executable;

class FiniteFieldTest
{
  private TermManager d_tm;
  private Solver d_solver;

  @BeforeEach
  void setUp()
  {
    d_tm = new TermManager();
    d_solver = new Solver(d_tm);
  }

  @AfterEach
  void tearDown()
  {
    Context.deletePointers();
  }

  @Test
  void finiteFieldTest() throws CVC5ApiException
  {
    d_solver.setLogic("QF_FF"); // Set the logic

    Sort f5 = d_tm.mkFiniteFieldSort("5", 10);
    Term a = d_tm.mkConst(f5, "a");
    Term b = d_tm.mkConst(f5, "b");
    Term z = d_tm.mkFiniteFieldElem("0", f5, 10);

    assertEquals(f5.isFiniteField(), true);
    assertEquals(f5.getFiniteFieldSize(), "5");
    assertEquals(z.isFiniteFieldValue(), true);
    assertEquals(z.getFiniteFieldValue(), "0");

    Term inv = d_tm.mkTerm(Kind.EQUAL,
        d_tm.mkTerm(Kind.FINITE_FIELD_ADD,
            d_tm.mkTerm(Kind.FINITE_FIELD_MULT, a, b),
            d_tm.mkFiniteFieldElem("-1", f5, 10)),
        z);

    Term aIsTwo = d_tm.mkTerm(
        Kind.EQUAL, d_tm.mkTerm(Kind.FINITE_FIELD_ADD, a, d_tm.mkFiniteFieldElem("-2", f5, 10)), z);

    d_solver.assertFormula(inv);
    d_solver.assertFormula(aIsTwo);

    Result r = d_solver.checkSat();
    assertEquals(r.isSat(), true);

    Term bIsTwo = d_tm.mkTerm(
        Kind.EQUAL, d_tm.mkTerm(Kind.FINITE_FIELD_ADD, b, d_tm.mkFiniteFieldElem("-2", f5, 10)), z);

    d_solver.assertFormula(bIsTwo);
    r = d_solver.checkSat();
    assertEquals(r.isSat(), false);
  }
}
