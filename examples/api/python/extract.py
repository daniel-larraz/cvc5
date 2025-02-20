#!/usr/bin/env python
###############################################################################
# Top contributors (to current version):
#   Aina Niemetz, Makai Mann, Andrew Reynolds
#
# This file is part of the cvc5 project.
#
# Copyright (c) 2009-2025 by the authors listed in the file AUTHORS
# in the top-level source directory and their institutional affiliations.
# All rights reserved.  See the file COPYING in the top-level source
# directory for licensing information.
# #############################################################################
#
# A simple demonstration of the solving capabilities of the cvc5 bit-vector
# solver through the Python API. This is a direct translation of
# extract-new.cpp.
##

from cvc5 import TermManager, Solver, Kind

if __name__ == "__main__":
    tm = TermManager()
    slv = Solver(tm)
    slv.setLogic("QF_BV")

    bitvector32 = tm.mkBitVectorSort(32)

    x = tm.mkConst(bitvector32, "a")

    ext_31_1 = tm.mkOp(Kind.BITVECTOR_EXTRACT, 31, 1)
    x_31_1 = tm.mkTerm(ext_31_1, x)

    ext_30_0 = tm.mkOp(Kind.BITVECTOR_EXTRACT, 30, 0)
    x_30_0 = tm.mkTerm(ext_30_0, x)

    ext_31_31 = tm.mkOp(Kind.BITVECTOR_EXTRACT, 31, 31)
    x_31_31 = tm.mkTerm(ext_31_31, x)

    ext_0_0 = tm.mkOp(Kind.BITVECTOR_EXTRACT, 0, 0)
    x_0_0 = tm.mkTerm(ext_0_0, x)

    eq = tm.mkTerm(Kind.EQUAL, x_31_1, x_30_0)
    print("Asserting:", eq)
    slv.assertFormula(eq)

    eq2 = tm.mkTerm(Kind.EQUAL, x_31_31, x_0_0)
    print("Check sat assuming:", eq2.notTerm())
    print("Expect UNSAT")
    print("cvc5:", slv.checkSatAssuming(eq2.notTerm()))
