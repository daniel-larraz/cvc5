/******************************************************************************
 * Top contributors (to current version):
 *   Aina Niemetz
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2025 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Test for project issue #383.
 */
#include <cvc5/cvc5.h>

#include <cassert>

using namespace cvc5;

int main(void)
{
  TermManager tm;
  Solver solver(tm);

  solver.setOption("produce-models", "true");

  DatatypeConstructorDecl ctordecl = tm.mkDatatypeConstructorDecl("_x5");
  DatatypeDecl dtdecl = tm.mkDatatypeDecl("_x0");
  dtdecl.addConstructor(ctordecl);
  ctordecl = tm.mkDatatypeConstructorDecl("_x23");
  ctordecl.addSelectorSelf("_x21");
  dtdecl = tm.mkDatatypeDecl("_x12");
  dtdecl.addConstructor(ctordecl);
  try
  {
    tm.mkDatatypeSort(dtdecl);
  }
  catch (CVC5ApiException& e)
  {
    return 0;
  }
  assert(false);
}
