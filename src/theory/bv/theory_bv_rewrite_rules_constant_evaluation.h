/******************************************************************************
 * Top contributors (to current version):
 *   Liana Hadarean, Clark Barrett, Aina Niemetz
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2025 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Constant evaluation rewrite rules of the BV theory.
 *
 */

#include "cvc5_private.h"

#ifndef CVC5__THEORY__BV__THEORY_BV_REWRITE_RULES_CONSTANT_EVALUATION_H
#define CVC5__THEORY__BV__THEORY_BV_REWRITE_RULES_CONSTANT_EVALUATION_H

#include "theory/bv/theory_bv_rewrite_rules.h"
#include "theory/bv/theory_bv_utils.h"
#include "util/bitvector.h"
#include "util/rational.h"

namespace cvc5::internal {
namespace theory {
namespace bv {

template<> inline
bool RewriteRule<EvalAnd>::applies(TNode node) {
  Unreachable();
  return (node.getKind() == Kind::BITVECTOR_AND && node.getNumChildren() == 2
          && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalAnd>::apply(TNode node) {
  Unreachable();
  Trace("bv-rewrite") << "RewriteRule<EvalAnd>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();
  BitVector res = a & b;

  return utils::mkConst(node.getNodeManager(), res);
}

template<> inline
bool RewriteRule<EvalOr>::applies(TNode node) {
  Unreachable();
  return (node.getKind() == Kind::BITVECTOR_OR && node.getNumChildren() == 2
          && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalOr>::apply(TNode node) {
  Unreachable();
  Trace("bv-rewrite") << "RewriteRule<EvalOr>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();
  BitVector res = a | b;

  return utils::mkConst(node.getNodeManager(), res);
}

template<> inline
bool RewriteRule<EvalXor>::applies(TNode node) {
  Unreachable();
  return (node.getKind() == Kind::BITVECTOR_XOR && node.getNumChildren() == 2
          && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalXor>::apply(TNode node) {
  Unreachable();
  Trace("bv-rewrite") << "RewriteRule<EvalXor>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();
  BitVector res = a ^ b;

  return utils::mkConst(node.getNodeManager(), res);
}

// template<> inline
// bool RewriteRule<EvalXnor>::applies(TNode node) {
//   return (node.getKind() == Kind::BITVECTOR_XNOR &&
//           utils::isBvConstTerm(node));
// }

// template<> inline
// Node RewriteRule<EvalXnor>::apply(TNode node) {
//   Trace("bv-rewrite") << "RewriteRule<EvalXnor>(" << node << ")" << std::endl;
//   BitVector a = node[0].getConst<BitVector>();
//   BitVector b = node[1].getConst<BitVector>();
//   BitVector res = ~ (a ^ b);
  
//   return utils::mkConst(res);
// }
template<> inline
bool RewriteRule<EvalNot>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_NOT && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalNot>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalNot>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector res = ~ a;
  return utils::mkConst(node.getNodeManager(), res);
}

// template<> inline
// bool RewriteRule<EvalComp>::applies(TNode node) {
//   return (node.getKind() == Kind::BITVECTOR_COMP &&
//           utils::isBvConstTerm(node));
// }

// template<> inline
// Node RewriteRule<EvalComp>::apply(TNode node) {
//   Trace("bv-rewrite") << "RewriteRule<EvalComp>(" << node << ")" << std::endl;
//   BitVector a = node[0].getConst<BitVector>();
//   BitVector b = node[1].getConst<BitVector>();
//   BitVector res;
//   if (a == b) {
//     res = BitVector(1, Integer(1));
//   } else {
//     res = BitVector(1, Integer(0)); 
//   }
  
//   return utils::mkConst(res);
// }

template<> inline
bool RewriteRule<EvalMult>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_MULT && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalMult>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalMult>(" << node << ")" << std::endl;
  TNode::iterator child_it = node.begin();
  BitVector res = (*child_it).getConst<BitVector>();
  for(++child_it; child_it != node.end(); ++child_it) {
    res = res * (*child_it).getConst<BitVector>();
  }
  return utils::mkConst(node.getNodeManager(), res);
}

template <>
inline bool RewriteRule<EvalAdd>::applies(TNode node)
{
  return (node.getKind() == Kind::BITVECTOR_ADD && utils::isBvConstTerm(node));
}

template <>
inline Node RewriteRule<EvalAdd>::apply(TNode node)
{
  Trace("bv-rewrite") << "RewriteRule<EvalAdd>(" << node << ")" << std::endl;
  TNode::iterator child_it = node.begin();
  BitVector res = (*child_it).getConst<BitVector>();
  for(++child_it; child_it != node.end(); ++child_it) {
    res = res + (*child_it).getConst<BitVector>();
  }
  return utils::mkConst(node.getNodeManager(), res);
}

// template<> inline
// bool RewriteRule<EvalSub>::applies(TNode node) {
//   return (node.getKind() == Kind::BITVECTOR_SUB &&
//           utils::isBvConstTerm(node));
// }

// template<> inline
// Node RewriteRule<EvalSub>::apply(TNode node) {
//   Trace("bv-rewrite") << "RewriteRule<EvalSub>(" << node << ")" << std::endl;
//   BitVector a = node[0].getConst<BitVector>();
//   BitVector b = node[1].getConst<BitVector>();
//   BitVector res = a - b;
  
//   return utils::mkConst(res);
// }
template<> inline
bool RewriteRule<EvalNeg>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_NEG && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalNeg>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalNeg>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector res = - a;

  return utils::mkConst(node.getNodeManager(), res);
}
template<> inline
bool RewriteRule<EvalUdiv>::applies(TNode node) {
  return utils::isBvConstTerm(node) && node.getKind() == Kind::BITVECTOR_UDIV;
}

template<> inline
Node RewriteRule<EvalUdiv>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalUdiv>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();
  BitVector res = a.unsignedDivTotal(b);

  return utils::mkConst(node.getNodeManager(), res);
}
template<> inline
bool RewriteRule<EvalUrem>::applies(TNode node) {
  return utils::isBvConstTerm(node) && node.getKind() == Kind::BITVECTOR_UREM;
}

template<> inline
Node RewriteRule<EvalUrem>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalUrem>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();
  BitVector res = a.unsignedRemTotal(b);

  return utils::mkConst(node.getNodeManager(), res);
}

template<> inline
bool RewriteRule<EvalShl>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_SHL && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalShl>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalShl>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();

  BitVector res = a.leftShift(b);
  return utils::mkConst(node.getNodeManager(), res);
}

template<> inline
bool RewriteRule<EvalLshr>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_LSHR && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalLshr>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalLshr>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();
  
  BitVector res = a.logicalRightShift(b);
  return utils::mkConst(node.getNodeManager(), res);
}

template<> inline
bool RewriteRule<EvalAshr>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_ASHR && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalAshr>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalAshr>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();

  BitVector res = a.arithRightShift(b);
  return utils::mkConst(node.getNodeManager(), res);
}

template<> inline
bool RewriteRule<EvalUlt>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_ULT && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalUlt>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalUlt>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();

  NodeManager* nm = node.getNodeManager();

  if (a.unsignedLessThan(b)) {
    return utils::mkTrue(nm);
  }
  return utils::mkFalse(nm);
}

template<> inline
bool RewriteRule<EvalUltBv>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_ULTBV
          && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalUltBv>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalUltBv>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();

  NodeManager* nm = node.getNodeManager();
  if (a.unsignedLessThan(b)) {
    return utils::mkConst(nm, 1, 1);
  }
  return utils::mkConst(nm, 1, 0);
}

template<> inline
bool RewriteRule<EvalSlt>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_SLT && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalSlt>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalSlt>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();

  NodeManager* nm = node.getNodeManager();

  if (a.signedLessThan(b)) {
    return utils::mkTrue(nm);
  }
  return utils::mkFalse(nm);
}

template<> inline
bool RewriteRule<EvalSltBv>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_SLTBV
          && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalSltBv>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalSltBv>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();

  NodeManager* nm = node.getNodeManager();
  if (a.signedLessThan(b)) {
    return utils::mkConst(nm, 1, 1);
  }
  return utils::mkConst(nm, 1, 0);
}

template<> inline
bool RewriteRule<EvalITEBv>::applies(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalITEBv>::applies(" << node << ")" << std::endl;
  return (node.getKind() == Kind::BITVECTOR_ITE && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalITEBv>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalITEBv>(" << node << ")" << std::endl;
  BitVector cond = node[0].getConst<BitVector>();

  NodeManager* nm = node.getNodeManager();
  if (node[0] == utils::mkConst(nm, 1, 1))
  {
    return node[1];
  }
  else
  {
    Assert(node[0] == utils::mkConst(nm, 1, 0));
    return node[2];
  }
}

template<> inline
bool RewriteRule<EvalUle>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_ULE && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalUle>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalUle>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();

  NodeManager* nm = node.getNodeManager();

  if (a.unsignedLessThanEq(b)) {
    return utils::mkTrue(nm);
  }
  return utils::mkFalse(nm);
}

template<> inline
bool RewriteRule<EvalSle>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_SLE && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalSle>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalSle>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();

  NodeManager* nm = node.getNodeManager();
  if (a.signedLessThanEq(b)) {
    return utils::mkTrue(nm);
  }
  return utils::mkFalse(nm);
}

template<> inline
bool RewriteRule<EvalExtract>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_EXTRACT
          && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalExtract>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalExtract>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  unsigned lo = utils::getExtractLow(node);
  unsigned hi = utils::getExtractHigh(node);

  BitVector res = a.extract(hi, lo);
  return utils::mkConst(node.getNodeManager(), res);
}


template<> inline
bool RewriteRule<EvalConcat>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_CONCAT
          && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalConcat>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalConcat>(" << node << ")" << std::endl;
  unsigned num = node.getNumChildren();
  BitVector res = node[0].getConst<BitVector>();
  for(unsigned i = 1; i < num; ++i ) {  
    BitVector a = node[i].getConst<BitVector>();
    res = res.concat(a); 
  }
  return utils::mkConst(node.getNodeManager(), res);
}

template<> inline
bool RewriteRule<EvalSignExtend>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_SIGN_EXTEND
          && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalSignExtend>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalSignExtend>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  unsigned amount =
      node.getOperator().getConst<BitVectorSignExtend>().d_signExtendAmount;
  BitVector res = a.signExtend(amount);

  return utils::mkConst(node.getNodeManager(), res);
}

template<> inline
bool RewriteRule<EvalEquals>::applies(TNode node) {
  return (node.getKind() == Kind::EQUAL && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalEquals>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalEquals>(" << node << ")" << std::endl;
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();
  NodeManager* nm = node.getNodeManager();
  if (a == b) {
    return utils::mkTrue(nm);
  }
  return utils::mkFalse(nm);
}

template<> inline
bool RewriteRule<EvalComp>::applies(TNode node) {
  return (node.getKind() == Kind::BITVECTOR_COMP && utils::isBvConstTerm(node));
}

template<> inline
Node RewriteRule<EvalComp>::apply(TNode node) {
  Trace("bv-rewrite") << "RewriteRule<EvalComp>(" << node << ")" << std::endl;
  NodeManager* nm = node.getNodeManager();
  BitVector a = node[0].getConst<BitVector>();
  BitVector b = node[1].getConst<BitVector>();
  if (a == b) {
    return utils::mkConst(nm, 1, 1);
  }
  return utils::mkConst(nm, 1, 0);
}

template <>
inline bool RewriteRule<EvalConstBvSym>::applies(TNode node)
{
  // second argument must be positive and fit unsigned int
  return (node.getKind() == Kind::CONST_BITVECTOR_SYMBOLIC && node[0].isConst()
          && node[1].isConst() && node[1].getConst<Rational>().sgn() == 1
          && node[1].getConst<Rational>().getNumerator().fitsUnsignedInt());
}

template <>
inline Node RewriteRule<EvalConstBvSym>::apply(TNode node)
{
  Trace("bv-rewrite") << "RewriteRule<EvalConstBvSym>(" << node << ")"
                      << std::endl;
  Integer a = node[0].getConst<Rational>().getNumerator();
  Integer b = node[1].getConst<Rational>().getNumerator();
  return utils::mkConst(node.getNodeManager(), b.toUnsignedInt(), a);
}

template <>
inline bool RewriteRule<EvalEagerAtom>::applies(TNode node)
{
  return (node.getKind() == Kind::BITVECTOR_EAGER_ATOM && node[0].isConst());
}

template <>
inline Node RewriteRule<EvalEagerAtom>::apply(TNode node)
{
  Trace("bv-rewrite") << "RewriteRule<EvalEagerAtom>(" << node << ")" << std::endl;
  return node[0];
}
}  // namespace bv
}  // namespace theory
}  // namespace cvc5::internal
#endif
