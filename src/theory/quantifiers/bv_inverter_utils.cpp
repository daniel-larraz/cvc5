/******************************************************************************
 * Top contributors (to current version):
 *   Aina Niemetz, Mathias Preiner, Daniel Larraz
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2025 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Inverse rules for bit-vector operators.
 */

#include "theory/quantifiers/bv_inverter_utils.h"
#include "theory/bv/theory_bv_utils.h"

using namespace cvc5::internal::kind;

namespace cvc5::internal {
namespace theory {
namespace quantifiers {
namespace utils {

Node getICBvUltUgt(bool pol, Kind k, Node x, Node t)
{
  Assert(k == Kind::BITVECTOR_ULT || k == Kind::BITVECTOR_UGT);
  NodeManager* nm = t.getNodeManager();

  unsigned w = bv::utils::getSize(t);
  Node ic;

  if (k == Kind::BITVECTOR_ULT)
  {
    if (pol == true)
    {
      /* x < t
       * with invertibility condition:
       * (distinct t z)
       * where
       * z = 0 with getSize(z) = w  */
      Node scl =
          NodeManager::mkNode(Kind::DISTINCT, t, bv::utils::mkZero(nm, w));
      Node scr = NodeManager::mkNode(k, x, t);
      ic = NodeManager::mkNode(Kind::IMPLIES, scl, scr);
    }
    else
    {
      /* x >= t
       * with invertibility condition:
       * true (no invertibility condition)  */
      ic = NodeManager::mkNode(Kind::NOT, NodeManager::mkNode(k, x, t));
    }
  }
  else
  {
    Assert(k == Kind::BITVECTOR_UGT);
    if (pol == true)
    {
      /* x > t
       * with invertibility condition:
       * (distinct t ones)
       * where
       * ones = ~0 with getSize(ones) = w  */
      Node scl =
          NodeManager::mkNode(Kind::DISTINCT, t, bv::utils::mkOnes(nm, w));
      Node scr = NodeManager::mkNode(k, x, t);
      ic = NodeManager::mkNode(Kind::IMPLIES, scl, scr);
    }
    else
    {
      /* x <= t
       * with invertibility condition:
       * true (no invertibility condition)  */
      ic = NodeManager::mkNode(Kind::NOT, NodeManager::mkNode(k, x, t));
    }
  }
  Trace("bv-invert") << "Add SC_" << k << "(" << x << "): " << ic << std::endl;
  return ic;
}

Node getICBvSltSgt(bool pol, Kind k, Node x, Node t)
{
  Assert(k == Kind::BITVECTOR_SLT || k == Kind::BITVECTOR_SGT);

  unsigned w = bv::utils::getSize(t);
  Node ic;

  NodeManager* nm = t.getNodeManager();
  if (k == Kind::BITVECTOR_SLT)
  {
    if (pol == true)
    {
      /* x < t
       * with invertibility condition:
       * (distinct t min)
       * where
       * min is the minimum signed value with getSize(min) = w  */
      Node min = bv::utils::mkMinSigned(nm, w);
      Node scl = NodeManager::mkNode(Kind::DISTINCT, min, t);
      Node scr = NodeManager::mkNode(k, x, t);
      ic = NodeManager::mkNode(Kind::IMPLIES, scl, scr);
    }
    else
    {
      /* x >= t
       * with invertibility condition:
       * true (no invertibility condition)  */
      ic = NodeManager::mkNode(Kind::NOT, NodeManager::mkNode(k, x, t));
    }
  }
  else
  {
    Assert(k == Kind::BITVECTOR_SGT);
    if (pol == true)
    {
      /* x > t
       * with invertibility condition:
       * (distinct t max)
       * where
       * max is the signed maximum value with getSize(max) = w  */
      Node max = bv::utils::mkMaxSigned(nm, w);
      Node scl = NodeManager::mkNode(Kind::DISTINCT, t, max);
      Node scr = NodeManager::mkNode(k, x, t);
      ic = NodeManager::mkNode(Kind::IMPLIES, scl, scr);
    }
    else
    {
      /* x <= t
       * with invertibility condition:
       * true (no invertibility condition)  */
      ic = NodeManager::mkNode(Kind::NOT, NodeManager::mkNode(k, x, t));
    }
  }
  Trace("bv-invert") << "Add SC_" << k << "(" << x << "): " << ic << std::endl;
  return ic;
}

Node getICBvMult(
    bool pol, Kind litk, Kind k, unsigned idx, Node x, Node s, Node t)
{
  Assert(k == Kind::BITVECTOR_MULT);
  Assert(litk == Kind::EQUAL || litk == Kind::BITVECTOR_ULT
         || litk == Kind::BITVECTOR_SLT || litk == Kind::BITVECTOR_UGT
         || litk == Kind::BITVECTOR_SGT);

  NodeManager* nm = x.getNodeManager();
  Node scl;
  unsigned w = bv::utils::getSize(s);
  Assert(w == bv::utils::getSize(t));

  if (litk == Kind::EQUAL)
  {
    Node z = bv::utils::mkZero(nm, w);

    if (pol)
    {
      /* x * s = t
       * with invertibility condition (synthesized):
       * (= (bvand (bvor (bvneg s) s) t) t)
       *
       * is equivalent to:
       * ctz(t) >= ctz(s)
       * ->
       * (or
       *   (= t z)
       *   (and
       *     (bvuge (bvand t (bvneg t)) (bvand s (bvneg s)))
       *     (distinct s z)))
       * where
       * z = 0 with getSize(z) = w  */
      Node o =
          nm->mkNode(Kind::BITVECTOR_OR, nm->mkNode(Kind::BITVECTOR_NEG, s), s);
      scl = nm->mkNode(Kind::EQUAL, nm->mkNode(Kind::BITVECTOR_AND, o, t), t);
    }
    else
    {
      /* x * s != t
       * with invertibility condition:
       * (or (distinct t z) (distinct s z))
       * where
       * z = 0 with getSize(z) = w  */
      scl = nm->mkNode(Kind::OR, t.eqNode(z).notNode(), s.eqNode(z).notNode());
    }
  }
  else if (litk == Kind::BITVECTOR_ULT)
  {
    if (pol)
    {
      /* x * s < t
       * with invertibility condition (synthesized):
       * (distinct t z)
       * where
       * z = 0 with getSize(z) = w  */
      Node z = bv::utils::mkZero(nm, w);
      scl = nm->mkNode(Kind::DISTINCT, t, z);
    }
    else
    {
      /* x * s >= t
       * with invertibility condition (synthesized):
       * (bvuge (bvor (bvneg s) s) t)  */
      Node o =
          nm->mkNode(Kind::BITVECTOR_OR, nm->mkNode(Kind::BITVECTOR_NEG, s), s);
      scl = nm->mkNode(Kind::BITVECTOR_UGE, o, t);
    }
  }
  else if (litk == Kind::BITVECTOR_UGT)
  {
    if (pol)
    {
      /* x * s > t
       * with invertibility condition (synthesized):
       * (bvult t (bvor (bvneg s) s))  */
      Node o =
          nm->mkNode(Kind::BITVECTOR_OR, nm->mkNode(Kind::BITVECTOR_NEG, s), s);
      scl = nm->mkNode(Kind::BITVECTOR_ULT, t, o);
    }
    else
    {
      /* x * s <= t
       * true (no invertibility condition)  */
      scl = nm->mkConst<bool>(true);
    }
  }
  else if (litk == Kind::BITVECTOR_SLT)
  {
    if (pol)
    {
      /* x * s < t
       * with invertibility condition (synthesized):
       * (bvslt (bvand (bvnot (bvneg t)) (bvor (bvneg s) s)) t)  */
      Node a1 =
          nm->mkNode(Kind::BITVECTOR_NOT, nm->mkNode(Kind::BITVECTOR_NEG, t));
      Node a2 =
          nm->mkNode(Kind::BITVECTOR_OR, nm->mkNode(Kind::BITVECTOR_NEG, s), s);
      scl = nm->mkNode(
          Kind::BITVECTOR_SLT, nm->mkNode(Kind::BITVECTOR_AND, a1, a2), t);
    }
    else
    {
      /* x * s >= t
       * with invertibility condition (synthesized):
       * (bvsge (bvand (bvor (bvneg s) s) max) t)
       * where
       * max is the signed maximum value with getSize(max) = w  */
      Node max = bv::utils::mkMaxSigned(nm, w);
      Node o =
          nm->mkNode(Kind::BITVECTOR_OR, nm->mkNode(Kind::BITVECTOR_NEG, s), s);
      Node a = nm->mkNode(Kind::BITVECTOR_AND, o, max);
      scl = nm->mkNode(Kind::BITVECTOR_SGE, a, t);
    }
  }
  else
  {
    Assert(litk == Kind::BITVECTOR_SGT);
    if (pol)
    {
      /* x * s > t
       * with invertibility condition (synthesized):
       * (bvslt t (bvsub t (bvor (bvor s t) (bvneg s))))  */
      Node o = nm->mkNode(Kind::BITVECTOR_OR,
                          nm->mkNode(Kind::BITVECTOR_OR, s, t),
                          nm->mkNode(Kind::BITVECTOR_NEG, s));
      Node sub = nm->mkNode(Kind::BITVECTOR_SUB, t, o);
      scl = nm->mkNode(Kind::BITVECTOR_SLT, t, sub);
    }
    else
    {
      /* x * s <= t
       * with invertibility condition (synthesized):
       * (not (and (= s z) (bvslt t s)))
       * where
       * z = 0 with getSize(z) = w  */
      Node z = bv::utils::mkZero(nm, w);
      scl = nm->mkNode(
          Kind::AND, s.eqNode(z), nm->mkNode(Kind::BITVECTOR_SLT, t, s));
      scl = scl.notNode();
    }
  }

  Node scr =
      nm->mkNode(litk, idx == 0 ? nm->mkNode(k, x, s) : nm->mkNode(k, s, x), t);
  Node ic = nm->mkNode(Kind::IMPLIES, scl, pol ? scr : scr.notNode());
  Trace("bv-invert") << "Add SC_" << k << "(" << x << "): " << ic << std::endl;
  return ic;
}

Node getICBvUrem(
    bool pol, Kind litk, Kind k, unsigned idx, Node x, Node s, Node t)
{
  Assert(k == Kind::BITVECTOR_UREM);
  Assert(litk == Kind::EQUAL || litk == Kind::BITVECTOR_ULT
         || litk == Kind::BITVECTOR_SLT || litk == Kind::BITVECTOR_UGT
         || litk == Kind::BITVECTOR_SGT);

  NodeManager* nm = x.getNodeManager();
  Node scl;
  unsigned w = bv::utils::getSize(s);
  Assert(w == bv::utils::getSize(t));

  if (litk == Kind::EQUAL)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x % s = t
         * with invertibility condition (synthesized):
         * (bvuge (bvnot (bvneg s)) t)  */
        Node neg = nm->mkNode(Kind::BITVECTOR_NEG, s);
        scl = nm->mkNode(
            Kind::BITVECTOR_UGE, nm->mkNode(Kind::BITVECTOR_NOT, neg), t);
      }
      else
      {
        /* x % s != t
         * with invertibility condition:
         * (or (distinct s (_ bv1 w)) (distinct t z))
         * where
         * z = 0 with getSize(z) = w  */
        Node z = bv::utils::mkZero(nm, w);
        scl = nm->mkNode(Kind::OR,
                         s.eqNode(bv::utils::mkOne(nm, w)).notNode(),
                         t.eqNode(z).notNode());
      }
    }
    else
    {
      if (pol)
      {
        /* s % x = t
         * with invertibility condition (synthesized):
         * (bvuge (bvand (bvsub (bvadd t t) s) s) t)
         *
         * is equivalent to:
         * (or (= s t)
         *     (and (bvugt s t)
         *          (bvugt (bvsub s t) t)
         *          (or (= t z) (distinct (bvsub s (_ bv1 w)) t))))
         * where
         * z = 0 with getSize(z) = w  */
        Node add = nm->mkNode(Kind::BITVECTOR_ADD, t, t);
        Node sub = nm->mkNode(Kind::BITVECTOR_SUB, add, s);
        Node a = nm->mkNode(Kind::BITVECTOR_AND, sub, s);
        scl = nm->mkNode(Kind::BITVECTOR_UGE, a, t);
      }
      else
      {
        /* s % x != t
         * with invertibility condition:
         * (or (distinct s z) (distinct t z))
         * where
         * z = 0 with getSize(z) = w  */
        Node z = bv::utils::mkZero(nm, w);
        scl =
            nm->mkNode(Kind::OR, s.eqNode(z).notNode(), t.eqNode(z).notNode());
      }
    }
  }
  else if (litk == Kind::BITVECTOR_ULT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x % s < t
         * with invertibility condition:
         * (distinct t z)
         * where
         * z = 0 with getSize(z) = w  */
        Node z = bv::utils::mkZero(nm, w);
        scl = t.eqNode(z).notNode();
      }
      else
      {
        /* x % s >= t
         * with invertibility condition (synthesized):
         * (bvuge (bvnot (bvneg s)) t)  */
        Node neg = nm->mkNode(Kind::BITVECTOR_NEG, s);
        scl = nm->mkNode(
            Kind::BITVECTOR_UGE, nm->mkNode(Kind::BITVECTOR_NOT, neg), t);
      }
    }
    else
    {
      if (pol)
      {
        /* s % x < t
         * with invertibility condition:
         * (distinct t z)
         * where
         * z = 0 with getSize(z) = w  */
        Node z = bv::utils::mkZero(nm, w);
        scl = t.eqNode(z).notNode();
      }
      else
      {
        /* s % x >= t
         * with invertibility condition (combination of = and >):
         * (or
         *   (bvuge (bvand (bvsub (bvadd t t) s) s) t)  ; eq, synthesized
         *   (bvult t s))                               ; ugt, synthesized  */
        Node add = nm->mkNode(Kind::BITVECTOR_ADD, t, t);
        Node sub = nm->mkNode(Kind::BITVECTOR_SUB, add, s);
        Node a = nm->mkNode(Kind::BITVECTOR_AND, sub, s);
        Node sceq = nm->mkNode(Kind::BITVECTOR_UGE, a, t);
        Node scugt = nm->mkNode(Kind::BITVECTOR_ULT, t, s);
        scl = nm->mkNode(Kind::OR, sceq, scugt);
      }
    }
  }
  else if (litk == Kind::BITVECTOR_UGT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x % s > t
         * with invertibility condition (synthesized):
         * (bvult t (bvnot (bvneg s)))  */
        Node nt =
            nm->mkNode(Kind::BITVECTOR_NOT, nm->mkNode(Kind::BITVECTOR_NEG, s));
        scl = nm->mkNode(Kind::BITVECTOR_ULT, t, nt);
      }
      else
      {
        /* x % s <= t
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
    }
    else
    {
      if (pol)
      {
        /* s % x > t
         * with invertibility condition (synthesized):
         * (bvult t s)  */
        scl = nm->mkNode(Kind::BITVECTOR_ULT, t, s);
      }
      else
      {
        /* s % x <= t
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
    }
  }
  else if (litk == Kind::BITVECTOR_SLT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x % s < t
         * with invertibility condition (synthesized):
         * (bvslt (bvnot t) (bvor (bvneg s) (bvneg t)))  */
        Node o1 = nm->mkNode(Kind::BITVECTOR_NEG, s);
        Node o2 = nm->mkNode(Kind::BITVECTOR_NEG, t);
        Node o = nm->mkNode(Kind::BITVECTOR_OR, o1, o2);
        scl = nm->mkNode(
            Kind::BITVECTOR_SLT, nm->mkNode(Kind::BITVECTOR_NOT, t), o);
      }
      else
      {
        /* x % s >= t
         * with invertibility condition (synthesized):
         * (or (bvslt t s) (bvsge z s))
         * where
         * z = 0 with getSize(z) = w  */
        Node z = bv::utils::mkZero(nm, w);
        Node s1 = nm->mkNode(Kind::BITVECTOR_SLT, t, s);
        Node s2 = nm->mkNode(Kind::BITVECTOR_SGE, z, s);
        scl = nm->mkNode(Kind::OR, s1, s2);
      }
    }
    else
    {
      Node z = bv::utils::mkZero(nm, w);

      if (pol)
      {
        /* s % x < t
         * with invertibility condition (synthesized):
         * (or (bvslt s t) (bvslt z t))
         * where
         * z = 0 with getSize(z) = w  */
        Node slt1 = nm->mkNode(Kind::BITVECTOR_SLT, s, t);
        Node slt2 = nm->mkNode(Kind::BITVECTOR_SLT, z, t);
        scl = nm->mkNode(Kind::OR, slt1, slt2);
      }
      else
      {
        /* s % x >= t
         * with invertibility condition:
         * (and
         *   (=> (bvsge s z) (bvsge s t))
         *   (=> (and (bvslt s z) (bvsge t z)) (bvugt (bvsub s t) t)))
         * where
         * z = 0 with getSize(z) = w  */
        Node i1 = nm->mkNode(Kind::IMPLIES,
                             nm->mkNode(Kind::BITVECTOR_SGE, s, z),
                             nm->mkNode(Kind::BITVECTOR_SGE, s, t));
        Node i2 = nm->mkNode(
            Kind::IMPLIES,
            nm->mkNode(Kind::AND,
                       nm->mkNode(Kind::BITVECTOR_SLT, s, z),
                       nm->mkNode(Kind::BITVECTOR_SGE, t, z)),
            nm->mkNode(
                Kind::BITVECTOR_UGT, nm->mkNode(Kind::BITVECTOR_SUB, s, t), t));
        scl = nm->mkNode(Kind::AND, i1, i2);
      }
    }
  }
  else
  {
    Assert(litk == Kind::BITVECTOR_SGT);
    if (idx == 0)
    {
      Node z = bv::utils::mkZero(nm, w);

      if (pol)
      {
        /* x % s > t
         * with invertibility condition:
         *
         * (and
         *   (and
         *     (=> (bvsgt s z) (bvslt t (bvnot (bvneg s))))
         *     (=> (bvsle s z) (distinct t max)))
         *   (or (distinct t z) (distinct s (_ bv1 w))))
         * where
         * z = 0 with getSize(z) = w
         * and max is the maximum signed value with getSize(max) = w  */
        Node max = bv::utils::mkMaxSigned(nm, w);
        Node nt =
            nm->mkNode(Kind::BITVECTOR_NOT, nm->mkNode(Kind::BITVECTOR_NEG, s));
        Node i1 = nm->mkNode(Kind::IMPLIES,
                             nm->mkNode(Kind::BITVECTOR_SGT, s, z),
                             nm->mkNode(Kind::BITVECTOR_SLT, t, nt));
        Node i2 = nm->mkNode(Kind::IMPLIES,
                             nm->mkNode(Kind::BITVECTOR_SLE, s, z),
                             t.eqNode(max).notNode());
        Node a1 = nm->mkNode(Kind::AND, i1, i2);
        Node a2 = nm->mkNode(Kind::OR,
                             t.eqNode(z).notNode(),
                             s.eqNode(bv::utils::mkOne(nm, w)).notNode());
        scl = nm->mkNode(Kind::AND, a1, a2);
      }
      else
      {
        /* x % s <= t
         * with invertibility condition (synthesized):
         * (bvslt ones (bvand (bvneg s) t))
         * where
         * z = 0 with getSize(z) = w
         * and ones = ~0 with getSize(ones) = w  */
        Node a = nm->mkNode(
            Kind::BITVECTOR_AND, nm->mkNode(Kind::BITVECTOR_NEG, s), t);
        scl = nm->mkNode(Kind::BITVECTOR_SLT, bv::utils::mkOnes(nm, w), a);
      }
    }
    else
    {
      if (pol)
      {
        /* s % x > t
         * with invertibility condition:
         * (and
         *   (=> (bvsge s z) (bvsgt s t))
         *   (=> (bvslt s z)
         *    (bvsgt (bvlshr (bvsub s (_ bv1 w)) (_ bv1 w)) t)))
         * where
         * z = 0 with getSize(z) = w  */
        Node z = bv::utils::mkZero(nm, w);
        Node i1 = nm->mkNode(Kind::IMPLIES,
                             nm->mkNode(Kind::BITVECTOR_SGE, s, z),
                             nm->mkNode(Kind::BITVECTOR_SGT, s, t));
        Node shr = nm->mkNode(
            Kind::BITVECTOR_LSHR, bv::utils::mkDec(s), bv::utils::mkOne(nm, w));
        Node i2 = nm->mkNode(Kind::IMPLIES,
                             nm->mkNode(Kind::BITVECTOR_SLT, s, z),
                             nm->mkNode(Kind::BITVECTOR_SGT, shr, t));
        scl = nm->mkNode(Kind::AND, i1, i2);
      }
      else
      {
        /* s % x <= t
         * with invertibility condition (synthesized):
         * (or (bvult t min) (bvsge t s))
         * where
         * min is the minimum signed value with getSize(min) = w  */
        Node min = bv::utils::mkMinSigned(nm, w);
        Node o1 = nm->mkNode(Kind::BITVECTOR_ULT, t, min);
        Node o2 = nm->mkNode(Kind::BITVECTOR_SGE, t, s);
        scl = nm->mkNode(Kind::OR, o1, o2);
      }
    }
  }

  Node scr =
      nm->mkNode(litk, idx == 0 ? nm->mkNode(k, x, s) : nm->mkNode(k, s, x), t);
  Node ic = nm->mkNode(Kind::IMPLIES, scl, pol ? scr : scr.notNode());
  Trace("bv-invert") << "Add SC_" << k << "(" << x << "): " << ic << std::endl;
  return ic;
}

Node getICBvUdiv(
    bool pol, Kind litk, Kind k, unsigned idx, Node x, Node s, Node t)
{
  Assert(k == Kind::BITVECTOR_UDIV);
  Assert(litk == Kind::EQUAL || litk == Kind::BITVECTOR_ULT
         || litk == Kind::BITVECTOR_SLT || litk == Kind::BITVECTOR_UGT
         || litk == Kind::BITVECTOR_SGT);

  NodeManager* nm = x.getNodeManager();
  unsigned w = bv::utils::getSize(s);
  Assert(w == bv::utils::getSize(t));
  Node scl;
  Node z = bv::utils::mkZero(nm, w);

  if (litk == Kind::EQUAL)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x udiv s = t
         * with invertibility condition (synthesized):
         * (= (bvudiv (bvmul s t) s) t)
         *
         * is equivalent to:
         * (or
         *   (and (= t (bvnot z))
         *        (or (= s z) (= s (_ bv1 w))))
         *   (and (distinct t (bvnot z))
         *        (distinct s z)
         *        (not (umulo s t))))
         *
         * where
         * umulo(s, t) is true if s * t produces and overflow
         * and z = 0 with getSize(z) = w  */
        Node mul = nm->mkNode(Kind::BITVECTOR_MULT, s, t);
        Node div = nm->mkNode(Kind::BITVECTOR_UDIV, mul, s);
        scl = nm->mkNode(Kind::EQUAL, div, t);
      }
      else
      {
        /* x udiv s != t
         * with invertibility condition:
         * (or (distinct s z) (distinct t ones))
         * where
         * z = 0 with getSize(z) = w
         * and ones = ~0 with getSize(ones) = w  */
        Node ones = bv::utils::mkOnes(nm, w);
        scl = nm->mkNode(
            Kind::OR, s.eqNode(z).notNode(), t.eqNode(ones).notNode());
      }
    }
    else
    {
      if (pol)
      {
        /* s udiv x = t
         * with invertibility condition (synthesized):
         * (= (bvudiv s (bvudiv s t)) t)
         *
         * is equivalent to:
         * (or
         *   (= s t)
         *   (= t (bvnot z))
         *   (and
         *     (bvuge s t)
         *     (or
         *       (= (bvurem s t) z)
         *       (bvule (bvadd (bvudiv s (bvadd t (_ bv1 w))) (_ bv1 w))
         *              (bvudiv s t)))
         *     (=> (= s (bvnot (_ bv0 8))) (distinct t (_ bv0 8)))))
         *
         * where
         * z = 0 with getSize(z) = w  */
        Node div = nm->mkNode(Kind::BITVECTOR_UDIV, s, t);
        scl = nm->mkNode(
            Kind::EQUAL, nm->mkNode(Kind::BITVECTOR_UDIV, s, div), t);
      }
      else
      {
        /* s udiv x != t
         * with invertibility condition (w > 1):
         * true (no invertibility condition)
         *
         * with invertibility condition (w == 1):
         * (= (bvand s t) z)
         *
         * where
         * z = 0 with getSize(z) = w  */
        if (w > 1)
        {
          scl = nm->mkConst<bool>(true);
        }
        else
        {
          scl = nm->mkNode(Kind::BITVECTOR_AND, s, t).eqNode(z);
        }
      }
    }
  }
  else if (litk == Kind::BITVECTOR_ULT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x udiv s < t
         * with invertibility condition (synthesized):
         * (and (bvult z s) (bvult z t))
         * where
         * z = 0 with getSize(z) = w  */
        Node u1 = nm->mkNode(Kind::BITVECTOR_ULT, z, s);
        Node u2 = nm->mkNode(Kind::BITVECTOR_ULT, z, t);
        scl = nm->mkNode(Kind::AND, u1, u2);
      }
      else
      {
        /* x udiv s >= t
         * with invertibility condition (synthesized):
         * (= (bvand (bvudiv (bvmul s t) t) s) s)  */
        Node mul = nm->mkNode(Kind::BITVECTOR_MULT, s, t);
        Node div = nm->mkNode(Kind::BITVECTOR_UDIV, mul, t);
        scl =
            nm->mkNode(Kind::EQUAL, nm->mkNode(Kind::BITVECTOR_AND, div, s), s);
      }
    }
    else
    {
      if (pol)
      {
        /* s udiv x < t
         * with invertibility condition (synthesized):
         * (and (bvult z (bvnot (bvand (bvneg t) s))) (bvult z t))
         * where
         * z = 0 with getSize(z) = w  */
        Node a = nm->mkNode(
            Kind::BITVECTOR_AND, nm->mkNode(Kind::BITVECTOR_NEG, t), s);
        Node u1 = nm->mkNode(
            Kind::BITVECTOR_ULT, z, nm->mkNode(Kind::BITVECTOR_NOT, a));
        Node u2 = nm->mkNode(Kind::BITVECTOR_ULT, z, t);
        scl = nm->mkNode(Kind::AND, u1, u2);
      }
      else
      {
        /* s udiv x >= t
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
    }
  }
  else if (litk == Kind::BITVECTOR_UGT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x udiv s > t
         * with invertibility condition:
         * (bvugt (bvudiv ones s) t)
         * where
         * ones = ~0 with getSize(ones) = w  */
        Node ones = bv::utils::mkOnes(nm, w);
        Node div = nm->mkNode(Kind::BITVECTOR_UDIV, ones, s);
        scl = nm->mkNode(Kind::BITVECTOR_UGT, div, t);
      }
      else
      {
        /* x udiv s <= t
         * with invertibility condition (synthesized):
         * (bvuge (bvor s t) (bvnot (bvneg s)))  */
        Node u1 = nm->mkNode(Kind::BITVECTOR_OR, s, t);
        Node u2 =
            nm->mkNode(Kind::BITVECTOR_NOT, nm->mkNode(Kind::BITVECTOR_NEG, s));
        scl = nm->mkNode(Kind::BITVECTOR_UGE, u1, u2);
      }
    }
    else
    {
      if (pol)
      {
        /* s udiv x > t
         * with invertibility condition (synthesized):
         * (bvult t ones)
         * where
         * ones = ~0 with getSize(ones) = w  */
        Node ones = bv::utils::mkOnes(nm, w);
        scl = nm->mkNode(Kind::BITVECTOR_ULT, t, ones);
      }
      else
      {
        /* s udiv x <= t
         * with invertibility condition (synthesized):
         * (bvult z (bvor (bvnot s) t))
         * where
         * z = 0 with getSize(z) = w  */
        scl = nm->mkNode(
            Kind::BITVECTOR_ULT,
            z,
            nm->mkNode(
                Kind::BITVECTOR_OR, nm->mkNode(Kind::BITVECTOR_NOT, s), t));
      }
    }
  }
  else if (litk == Kind::BITVECTOR_SLT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x udiv s < t
         * with invertibility condition:
         * (=> (bvsle t z) (bvslt (bvudiv min s) t))
         * where
         * z = 0 with getSize(z) = w
         * and min is the minimum signed value with getSize(min) = w  */
        Node min = bv::utils::mkMinSigned(nm, w);
        Node sle = nm->mkNode(Kind::BITVECTOR_SLE, t, z);
        Node div = nm->mkNode(Kind::BITVECTOR_UDIV, min, s);
        Node slt = nm->mkNode(Kind::BITVECTOR_SLT, div, t);
        scl = nm->mkNode(Kind::IMPLIES, sle, slt);
      }
      else
      {
        /* x udiv s >= t
         * with invertibility condition:
         * (or
         *   (bvsge (bvudiv ones s) t)
         *   (bvsge (bvudiv max s) t))
         * where
         * ones = ~0 with getSize(ones) = w
         * and max is the maximum signed value with getSize(max) = w  */
        Node max = bv::utils::mkMaxSigned(nm, w);
        Node ones = bv::utils::mkOnes(nm, w);
        Node udiv1 = nm->mkNode(Kind::BITVECTOR_UDIV, ones, s);
        Node udiv2 = nm->mkNode(Kind::BITVECTOR_UDIV, max, s);
        Node sge1 = nm->mkNode(Kind::BITVECTOR_SGE, udiv1, t);
        Node sge2 = nm->mkNode(Kind::BITVECTOR_SGE, udiv2, t);
        scl = nm->mkNode(Kind::OR, sge1, sge2);
      }
    }
    else
    {
      if (pol)
      {
        /* s udiv x < t
         * with invertibility condition (synthesized):
         * (or (bvslt s t) (bvsge t z))
         * where
         * z = 0 with getSize(z) = w  */
        Node slt = nm->mkNode(Kind::BITVECTOR_SLT, s, t);
        Node sge = nm->mkNode(Kind::BITVECTOR_SGE, t, z);
        scl = nm->mkNode(Kind::OR, slt, sge);
      }
      else
      {
        /* s udiv x >= t
         * with invertibility condition (w > 1):
         * (and
         *   (=> (bvsge s z) (bvsge s t))
         *   (=> (bvslt s z) (bvsge (bvlshr s (_ bv1 w)) t)))
         *
         * with invertibility condition (w == 1):
         * (bvsge s t)
         *
         * where
         * z = 0 with getSize(z) = w  */

        if (w > 1)
        {
          Node div =
              nm->mkNode(Kind::BITVECTOR_LSHR, s, bv::utils::mkConst(nm, w, 1));
          Node i1 = nm->mkNode(Kind::IMPLIES,
                               nm->mkNode(Kind::BITVECTOR_SGE, s, z),
                               nm->mkNode(Kind::BITVECTOR_SGE, s, t));
          Node i2 = nm->mkNode(Kind::IMPLIES,
                               nm->mkNode(Kind::BITVECTOR_SLT, s, z),
                               nm->mkNode(Kind::BITVECTOR_SGE, div, t));
          scl = nm->mkNode(Kind::AND, i1, i2);
        }
        else
        {
          scl = nm->mkNode(Kind::BITVECTOR_SGE, s, t);
        }
      }
    }
  }
  else
  {
    Assert(litk == Kind::BITVECTOR_SGT);
    if (idx == 0)
    {
      if (pol)
      {
        /* x udiv s > t
         * with invertibility condition:
         * (or
         *   (bvsgt (bvudiv ones s) t)
         *   (bvsgt (bvudiv max s) t))
         * where
         * ones = ~0 with getSize(ones) = w
         * and max is the maximum signed value with getSize(max) = w  */
        Node max = bv::utils::mkMaxSigned(nm, w);
        Node ones = bv::utils::mkOnes(nm, w);
        Node div1 = nm->mkNode(Kind::BITVECTOR_UDIV, ones, s);
        Node sgt1 = nm->mkNode(Kind::BITVECTOR_SGT, div1, t);
        Node div2 = nm->mkNode(Kind::BITVECTOR_UDIV, max, s);
        Node sgt2 = nm->mkNode(Kind::BITVECTOR_SGT, div2, t);
        scl = nm->mkNode(Kind::OR, sgt1, sgt2);
      }
      else
      {
        /* x udiv s <= t
         * with invertibility condition (combination of = and <):
         * (or
         *   (= (bvudiv (bvmul s t) s) t)                ; eq, synthesized
         *   (=> (bvsle t z) (bvslt (bvudiv min s) t)))  ; slt
         * where
         * z = 0 with getSize(z) = w
         * and min is the minimum signed value with getSize(min) = w  */
        Node mul = nm->mkNode(Kind::BITVECTOR_MULT, s, t);
        Node div1 = nm->mkNode(Kind::BITVECTOR_UDIV, mul, s);
        Node o1 = nm->mkNode(Kind::EQUAL, div1, t);
        Node min = bv::utils::mkMinSigned(nm, w);
        Node sle = nm->mkNode(Kind::BITVECTOR_SLE, t, z);
        Node div2 = nm->mkNode(Kind::BITVECTOR_UDIV, min, s);
        Node slt = nm->mkNode(Kind::BITVECTOR_SLT, div2, t);
        Node o2 = nm->mkNode(Kind::IMPLIES, sle, slt);
        scl = nm->mkNode(Kind::OR, o1, o2);
      }
    }
    else
    {
      if (pol)
      {
        /* s udiv x > t
         * with invertibility condition (w > 1):
         * (and
         *   (=> (bvsge s z) (bvsgt s t))
         *   (=> (bvslt s z) (bvsgt (bvlshr s (_ bv1 w)) t)))
         *
         * with invertibility condition (w == 1):
         * (bvsgt s t)
         *
         * where
         * z = 0 with getSize(z) = w  */
        if (w > 1)
        {
          Node div =
              nm->mkNode(Kind::BITVECTOR_LSHR, s, bv::utils::mkConst(nm, w, 1));
          Node i1 = nm->mkNode(Kind::IMPLIES,
                               nm->mkNode(Kind::BITVECTOR_SGE, s, z),
                               nm->mkNode(Kind::BITVECTOR_SGT, s, t));
          Node i2 = nm->mkNode(Kind::IMPLIES,
                               nm->mkNode(Kind::BITVECTOR_SLT, s, z),
                               nm->mkNode(Kind::BITVECTOR_SGT, div, t));
          scl = nm->mkNode(Kind::AND, i1, i2);
        }
        else
        {
          scl = nm->mkNode(Kind::BITVECTOR_SGT, s, t);
        }
      }
      else
      {
        /* s udiv x <= t
         * with invertibility condition (synthesized):
         * (not (and (bvslt t (bvnot #x0)) (bvslt t s)))
         * <->
         * (or (bvsge t ones) (bvsge t s))
         * where
         * ones = ~0 with getSize(ones) = w  */
        Node ones = bv::utils::mkOnes(nm, w);
        Node sge1 = nm->mkNode(Kind::BITVECTOR_SGE, t, ones);
        Node sge2 = nm->mkNode(Kind::BITVECTOR_SGE, t, s);
        scl = nm->mkNode(Kind::OR, sge1, sge2);
      }
    }
  }

  Node scr =
      nm->mkNode(litk, idx == 0 ? nm->mkNode(k, x, s) : nm->mkNode(k, s, x), t);
  Node ic = nm->mkNode(Kind::IMPLIES, scl, pol ? scr : scr.notNode());
  Trace("bv-invert") << "Add SC_" << k << "(" << x << "): " << ic << std::endl;
  return ic;
}

Node getICBvAndOr(
    bool pol, Kind litk, Kind k, unsigned idx, Node x, Node s, Node t)
{
  Assert(k == Kind::BITVECTOR_AND || k == Kind::BITVECTOR_OR);
  Assert(litk == Kind::EQUAL || litk == Kind::BITVECTOR_ULT
         || litk == Kind::BITVECTOR_SLT || litk == Kind::BITVECTOR_UGT
         || litk == Kind::BITVECTOR_SGT);

  NodeManager* nm = x.getNodeManager();
  unsigned w = bv::utils::getSize(s);
  Assert(w == bv::utils::getSize(t));
  Node scl;

  if (litk == Kind::EQUAL)
  {
    if (pol)
    {
      /* x & s = t
       * x | s = t
       * with invertibility condition:
       * (= (bvand t s) t)
       * (= (bvor t s) t)  */
      scl = nm->mkNode(Kind::EQUAL, t, nm->mkNode(k, t, s));
    }
    else
    {
      if (k == Kind::BITVECTOR_AND)
      {
        /* x & s = t
         * with invertibility condition:
         * (or (distinct s z) (distinct t z))
         * where
         * z = 0 with getSize(z) = w  */
        Node z = bv::utils::mkZero(nm, w);
        scl =
            nm->mkNode(Kind::OR, s.eqNode(z).notNode(), t.eqNode(z).notNode());
      }
      else
      {
        /* x | s = t
         * with invertibility condition:
         * (or (distinct s ones) (distinct t ones))
         * where
         * ones = ~0 with getSize(ones) = w  */
        Node n = bv::utils::mkOnes(nm, w);
        scl =
            nm->mkNode(Kind::OR, s.eqNode(n).notNode(), t.eqNode(n).notNode());
      }
    }
  }
  else if (litk == Kind::BITVECTOR_ULT)
  {
    if (pol)
    {
      if (k == Kind::BITVECTOR_AND)
      {
        /* x & s < t
         * with invertibility condition (synthesized):
         * (distinct t z)
         * where
         * z = 0 with getSize(z) = 0  */
        Node z = bv::utils::mkZero(nm, w);
        scl = t.eqNode(z).notNode();
      }
      else
      {
        /* x | s < t
         * with invertibility condition (synthesized):
         * (bvult s t)  */
        scl = nm->mkNode(Kind::BITVECTOR_ULT, s, t);
      }
    }
    else
    {
      if (k == Kind::BITVECTOR_AND)
      {
        /* x & s >= t
         * with invertibility condition (synthesized):
         * (bvuge s t)  */
        scl = nm->mkNode(Kind::BITVECTOR_UGE, s, t);
      }
      else
      {
        /* x | s >= t
         * with invertibility condition (synthesized):
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
    }
  }
  else if (litk == Kind::BITVECTOR_UGT)
  {
    if (pol)
    {
      if (k == Kind::BITVECTOR_AND)
      {
        /* x & s > t
         * with invertibility condition (synthesized):
         * (bvult t s)  */
        scl = nm->mkNode(Kind::BITVECTOR_ULT, t, s);
      }
      else
      {
        /* x | s > t
         * with invertibility condition (synthesized):
         * (bvult t ones)
         * where
         * ones = ~0 with getSize(ones) = w  */
        scl = nm->mkNode(Kind::BITVECTOR_ULT, t, bv::utils::mkOnes(nm, w));
      }
    }
    else
    {
      if (k == Kind::BITVECTOR_AND)
      {
        /* x & s <= t
         * with invertibility condition (synthesized):
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
      else
      {
        /* x | s <= t
         * with invertibility condition (synthesized):
         * (bvuge t s)  */
        scl = nm->mkNode(Kind::BITVECTOR_UGE, t, s);
      }
    }
  }
  else if (litk == Kind::BITVECTOR_SLT)
  {
    if (pol)
    {
      if (k == Kind::BITVECTOR_AND)
      {
        /* x & s < t
         * with invertibility condition (synthesized):
         * (bvslt (bvand (bvnot (bvneg t)) s) t)  */
        Node nnt =
            nm->mkNode(Kind::BITVECTOR_NOT, nm->mkNode(Kind::BITVECTOR_NEG, t));
        scl = nm->mkNode(
            Kind::BITVECTOR_SLT, nm->mkNode(Kind::BITVECTOR_AND, nnt, s), t);
      }
      else
      {
        /* x | s < t
         * with invertibility condition (synthesized):
         * (bvslt (bvor (bvnot (bvsub s t)) s) t)  */
        Node st = nm->mkNode(Kind::BITVECTOR_NOT,
                             nm->mkNode(Kind::BITVECTOR_SUB, s, t));
        scl = nm->mkNode(
            Kind::BITVECTOR_SLT, nm->mkNode(Kind::BITVECTOR_OR, st, s), t);
      }
    }
    else
    {
      if (k == Kind::BITVECTOR_AND)
      {
        /* x & s >= t
         * with invertibility condition (case = combined with synthesized
         * bvsgt): (or
         *  (= (bvand s t) t)
         *  (bvslt t (bvand (bvsub t s) s)))  */
        Node sc_sgt = nm->mkNode(
            Kind::BITVECTOR_SLT,
            t,
            nm->mkNode(
                Kind::BITVECTOR_AND, nm->mkNode(Kind::BITVECTOR_SUB, t, s), s));
        Node sc_eq = nm->mkNode(Kind::BITVECTOR_AND, s, t).eqNode(t);
        scl = sc_eq.orNode(sc_sgt);
      }
      else
      {
        /* x | s >= t
         * with invertibility condition (synthesized):
         * (bvsge s (bvand s t))  */
        scl = nm->mkNode(
            Kind::BITVECTOR_SGE, s, nm->mkNode(Kind::BITVECTOR_AND, s, t));
      }
    }
  }
  else
  {
    Assert(litk == Kind::BITVECTOR_SGT);
    if (pol)
    {
      /* x & s > t
       * x | s > t
       * with invertibility condition (synthesized):
       * (bvslt t (bvand s max))
       * (bvslt t (bvor s max))
       * where
       * max is the signed maximum value with getSize(max) = w  */
      Node max = bv::utils::mkMaxSigned(nm, w);
      scl = nm->mkNode(Kind::BITVECTOR_SLT, t, nm->mkNode(k, s, max));
    }
    else
    {
      if (k == Kind::BITVECTOR_AND)
      {
        /* x & s <= t
         * with invertibility condition (synthesized):
         * (bvuge s (bvand t min))
         * where
         * min is the signed minimum value with getSize(min) = w  */
        Node min = bv::utils::mkMinSigned(nm, w);
        scl = nm->mkNode(
            Kind::BITVECTOR_UGE, s, nm->mkNode(Kind::BITVECTOR_AND, t, min));
      }
      else
      {
        /* x | s <= t
         * with invertibility condition (synthesized):
         * (bvsge t (bvor s min))
         * where
         * min is the signed minimum value with getSize(min) = w  */
        Node min = bv::utils::mkMinSigned(nm, w);
        scl = nm->mkNode(
            Kind::BITVECTOR_SGE, t, nm->mkNode(Kind::BITVECTOR_OR, s, min));
      }
    }
  }
  Node scr = nm->mkNode(litk, nm->mkNode(k, x, s), t);
  Node ic = nm->mkNode(Kind::IMPLIES, scl, pol ? scr : scr.notNode());
  Trace("bv-invert") << "Add SC_" << k << "(" << x << "): " << ic << std::endl;
  return ic;
}

namespace {
Node defaultShiftIC(Kind litk, Kind shk, Node s, Node t)
{
  unsigned w;
  NodeManager* nm = s.getNodeManager();
  NodeBuilder nb(nm, Kind::OR);

  w = bv::utils::getSize(s);
  Assert(w == bv::utils::getSize(t));

  nb << nm->mkNode(litk, s, t);
  for (unsigned i = 1; i <= w; i++)
  {
    Node sw = bv::utils::mkConst(nm, w, i);
    nb << nm->mkNode(litk, nm->mkNode(shk, s, sw), t);
  }
  if (nb.getNumChildren() == 1) return nb[0];
  return nb.constructNode();
}
}  // namespace

Node getICBvLshr(
    bool pol, Kind litk, Kind k, unsigned idx, Node x, Node s, Node t)
{
  Assert(k == Kind::BITVECTOR_LSHR);
  Assert(litk == Kind::EQUAL || litk == Kind::BITVECTOR_ULT
         || litk == Kind::BITVECTOR_SLT || litk == Kind::BITVECTOR_UGT
         || litk == Kind::BITVECTOR_SGT);

  NodeManager* nm = x.getNodeManager();
  Node scl;
  unsigned w = bv::utils::getSize(s);
  Assert(w == bv::utils::getSize(t));
  Node z = bv::utils::mkZero(nm, w);

  if (litk == Kind::EQUAL)
  {
    if (idx == 0)
    {
      Node ww = bv::utils::mkConst(nm, w, w);

      if (pol)
      {
        /* x >> s = t
         * with invertibility condition (synthesized):
         * (= (bvlshr (bvshl t s) s) t)  */
        Node shl = nm->mkNode(Kind::BITVECTOR_SHL, t, s);
        Node lshr = nm->mkNode(Kind::BITVECTOR_LSHR, shl, s);
        scl = lshr.eqNode(t);
      }
      else
      {
        /* x >> s != t
         * with invertibility condition:
         * (or (distinct t z) (bvult s w))
         * where
         * z = 0 with getSize(z) = w
         * and w = getSize(s) = getSize(t)  */
        scl = nm->mkNode(Kind::OR,
                         t.eqNode(z).notNode(),
                         nm->mkNode(Kind::BITVECTOR_ULT, s, ww));
      }
    }
    else
    {
      if (pol)
      {
        /* s >> x = t
         * with invertibility condition:
         * (or (= (bvlshr s i) t) ...)
         * for i in 0..w  */
        scl = defaultShiftIC(Kind::EQUAL, Kind::BITVECTOR_LSHR, s, t);
      }
      else
      {
        /* s >> x != t
         * with invertibility condition:
         * (or (distinct s z) (distinct t z))
         * where
         * z = 0 with getSize(z) = w  */
        scl =
            nm->mkNode(Kind::OR, s.eqNode(z).notNode(), t.eqNode(z).notNode());
      }
    }
  }
  else if (litk == Kind::BITVECTOR_ULT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x >> s < t
         * with invertibility condition (synthesized):
         * (distinct t z)
         * where
         * z = 0 with getSize(z) = w  */
        scl = t.eqNode(z).notNode();
      }
      else
      {
        /* x >> s >= t
         * with invertibility condition (synthesized):
         * (= (bvlshr (bvshl t s) s) t)  */
        Node ts = nm->mkNode(Kind::BITVECTOR_SHL, t, s);
        scl = nm->mkNode(Kind::BITVECTOR_LSHR, ts, s).eqNode(t);
      }
    }
    else
    {
      if (pol)
      {
        /* s >> x < t
         * with invertibility condition (synthesized):
         * (distinct t z)
         * where
         * z = 0 with getSize(z) = w  */
        scl = t.eqNode(z).notNode();
      }
      else
      {
        /* s >> x >= t
         * with invertibility condition (synthesized):
         * (bvuge s t)  */
        scl = nm->mkNode(Kind::BITVECTOR_UGE, s, t);
      }
    }
  }
  else if (litk == Kind::BITVECTOR_UGT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x >> s > t
         * with invertibility condition (synthesized):
         * (bvult t (bvlshr (bvnot s) s))  */
        Node lshr = nm->mkNode(
            Kind::BITVECTOR_LSHR, nm->mkNode(Kind::BITVECTOR_NOT, s), s);
        scl = nm->mkNode(Kind::BITVECTOR_ULT, t, lshr);
      }
      else
      {
        /* x >> s <= t
         * with invertibility condition:
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
    }
    else
    {
      if (pol)
      {
        /* s >> x > t
         * with invertibility condition (synthesized):
         * (bvult t s)  */
        scl = nm->mkNode(Kind::BITVECTOR_ULT, t, s);
      }
      else
      {
        /* s >> x <= t
         * with invertibility condition:
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
    }
  }
  else if (litk == Kind::BITVECTOR_SLT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x >> s < t
         * with invertibility condition (synthesized):
         * (bvslt (bvlshr (bvnot (bvneg t)) s) t)  */
        Node nnt =
            nm->mkNode(Kind::BITVECTOR_NOT, nm->mkNode(Kind::BITVECTOR_NEG, t));
        Node lshr = nm->mkNode(Kind::BITVECTOR_LSHR, nnt, s);
        scl = nm->mkNode(Kind::BITVECTOR_SLT, lshr, t);
      }
      else
      {
        /* x >> s >= t
         * with invertibility condition:
         * (=> (not (= s z)) (bvsge (bvlshr ones s) t))
         * where
         * z = 0 with getSize(z) = w
         * and ones = ~0 with getSize(ones) = w  */
        Node ones = bv::utils::mkOnes(nm, w);
        Node lshr = nm->mkNode(Kind::BITVECTOR_LSHR, ones, s);
        Node nz = s.eqNode(z).notNode();
        scl = nz.impNode(nm->mkNode(Kind::BITVECTOR_SGE, lshr, t));
      }
    }
    else
    {
      if (pol)
      {
        /* s >> x < t
         * with invertibility condition (synthesized):
         * (or (bvslt s t) (bvslt z t))
         * where
         * z = 0 with getSize(z) = w  */
        Node st = nm->mkNode(Kind::BITVECTOR_SLT, s, t);
        Node zt = nm->mkNode(Kind::BITVECTOR_SLT, z, t);
        scl = st.orNode(zt);
      }
      else
      {
        /* s >> x >= t
         * with invertibility condition:
         * (and
         *  (=> (bvslt s z) (bvsge (bvlshr s (_ bv1 w)) t))
         *  (=> (bvsge s z) (bvsge s t)))
         * where
         * z = 0 with getSize(z) = w  */
        Node one = bv::utils::mkConst(nm, w, 1);
        Node sz = nm->mkNode(Kind::BITVECTOR_SLT, s, z);
        Node lshr = nm->mkNode(Kind::BITVECTOR_LSHR, s, one);
        Node sge1 = nm->mkNode(Kind::BITVECTOR_SGE, lshr, t);
        Node sge2 = nm->mkNode(Kind::BITVECTOR_SGE, s, t);
        scl = sz.impNode(sge1).andNode(sz.notNode().impNode(sge2));
      }
    }
  }
  else
  {
    Assert(litk == Kind::BITVECTOR_SGT);
    if (idx == 0)
    {
      if (pol)
      {
        /* x >> s > t
         * with invertibility condition (synthesized):
         * (bvslt t (bvlshr (bvshl max s) s))
         * where
         * max is the signed maximum value with getSize(max) = w  */
        Node max = bv::utils::mkMaxSigned(nm, w);
        Node shl = nm->mkNode(Kind::BITVECTOR_SHL, max, s);
        Node lshr = nm->mkNode(Kind::BITVECTOR_LSHR, shl, s);
        scl = nm->mkNode(Kind::BITVECTOR_SLT, t, lshr);
      }
      else
      {
        /* x >> s <= t
         * with invertibility condition (synthesized):
         * (bvsge t (bvlshr t s))  */
        scl = nm->mkNode(
            Kind::BITVECTOR_SGE, t, nm->mkNode(Kind::BITVECTOR_LSHR, t, s));
      }
    }
    else
    {
      if (pol)
      {
        /* s >> x > t
         * with invertibility condition:
         * (and
         *  (=> (bvslt s z) (bvsgt (bvlshr s one) t))
         *  (=> (bvsge s z) (bvsgt s t)))
         * where
         * z = 0 and getSize(z) = w  */
        Node one = bv::utils::mkOne(nm, w);
        Node sz = nm->mkNode(Kind::BITVECTOR_SLT, s, z);
        Node lshr = nm->mkNode(Kind::BITVECTOR_LSHR, s, one);
        Node sgt1 = nm->mkNode(Kind::BITVECTOR_SGT, lshr, t);
        Node sgt2 = nm->mkNode(Kind::BITVECTOR_SGT, s, t);
        scl = sz.impNode(sgt1).andNode(sz.notNode().impNode(sgt2));
      }
      else
      {
        /* s >> x <= t
         * with invertibility condition (synthesized):
         * (or (bvult t min) (bvsge t s))
         * where
         * min is the minimum signed value with getSize(min) = w  */
        Node min = bv::utils::mkMinSigned(nm, w);
        Node ult = nm->mkNode(Kind::BITVECTOR_ULT, t, min);
        Node sge = nm->mkNode(Kind::BITVECTOR_SGE, t, s);
        scl = ult.orNode(sge);
      }
    }
  }
  Node scr =
      nm->mkNode(litk, idx == 0 ? nm->mkNode(k, x, s) : nm->mkNode(k, s, x), t);
  Node ic = nm->mkNode(Kind::IMPLIES, scl, pol ? scr : scr.notNode());
  Trace("bv-invert") << "Add SC_" << k << "(" << x << "): " << ic << std::endl;
  return ic;
}

Node getICBvAshr(
    bool pol, Kind litk, Kind k, unsigned idx, Node x, Node s, Node t)
{
  Assert(k == Kind::BITVECTOR_ASHR);
  Assert(litk == Kind::EQUAL || litk == Kind::BITVECTOR_ULT
         || litk == Kind::BITVECTOR_SLT || litk == Kind::BITVECTOR_UGT
         || litk == Kind::BITVECTOR_SGT);

  NodeManager* nm = x.getNodeManager();
  Node scl;
  unsigned w = bv::utils::getSize(s);
  Assert(w == bv::utils::getSize(t));
  Node z = bv::utils::mkZero(nm, w);
  Node n = bv::utils::mkOnes(nm, w);

  if (litk == Kind::EQUAL)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x >> s = t
         * with invertibility condition:
         * (and
         *  (=> (bvult s w) (= (bvashr (bvshl t s) s) t))
         *  (=> (bvuge s w) (or (= t ones) (= t z)))
         * )
         * where
         * z = 0 with getSize(z) = w
         * and ones = ~0 with getSize(ones) = w
         * and w = getSize(t) = getSize(s)  */
        Node ww = bv::utils::mkConst(nm, w, w);
        Node shl = nm->mkNode(Kind::BITVECTOR_SHL, t, s);
        Node ashr = nm->mkNode(Kind::BITVECTOR_ASHR, shl, s);
        Node ult = nm->mkNode(Kind::BITVECTOR_ULT, s, ww);
        Node imp1 = ult.impNode(ashr.eqNode(t));
        Node to = t.eqNode(n);
        Node tz = t.eqNode(z);
        Node imp2 = ult.notNode().impNode(to.orNode(tz));
        scl = imp1.andNode(imp2);
      }
      else
      {
        /* x >> s != t
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
    }
    else
    {
      if (pol)
      {
        /* s >> x = t
         * with invertibility condition:
         * (or (= (bvashr s i) t) ...)
         * for i in 0..w  */
        scl = defaultShiftIC(Kind::EQUAL, Kind::BITVECTOR_ASHR, s, t);
      }
      else
      {
        /* s >> x != t
         * with invertibility condition:
         * (and
         *  (or (not (= t z)) (not (= s z)))
         *  (or (not (= t ones)) (not (= s ones))))
         * where
         * z = 0 with getSize(z) = w
         * and ones = ~0 with getSize(ones) = w  */
        scl = nm->mkNode(
            Kind::AND,
            nm->mkNode(Kind::OR, t.eqNode(z).notNode(), s.eqNode(z).notNode()),
            nm->mkNode(Kind::OR, t.eqNode(n).notNode(), s.eqNode(n).notNode()));
      }
    }
  }
  else if (litk == Kind::BITVECTOR_ULT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x >> s < t
         * with invertibility condition (synthesized):
         * (distinct t z)
         * where
         * z = 0 with getSize(z) = w  */
        scl = t.eqNode(z).notNode();
      }
      else
      {
        /* x >> s >= t
         * with invertibility condition (synthesized):
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
    }
    else
    {
      if (pol)
      {
        /* s >> x < t
         * with invertibility condition (synthesized):
         * (and (not (and (bvuge s t) (bvslt s z))) (not (= t z)))
         * where
         * z = 0 with getSize(z) = w  */
        Node st = nm->mkNode(Kind::BITVECTOR_UGE, s, t);
        Node sz = nm->mkNode(Kind::BITVECTOR_SLT, s, z);
        Node tz = t.eqNode(z).notNode();
        scl = st.andNode(sz).notNode().andNode(tz);
      }
      else
      {
        /* s >> x >= t
         * with invertibility condition (synthesized):
         * (not (and (bvult s (bvnot s)) (bvult s t)))  */
        Node ss = nm->mkNode(
            Kind::BITVECTOR_ULT, s, nm->mkNode(Kind::BITVECTOR_NOT, s));
        Node st = nm->mkNode(Kind::BITVECTOR_ULT, s, t);
        scl = ss.andNode(st).notNode();
      }
    }
  }
  else if (litk == Kind::BITVECTOR_UGT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x >> s > t
         * with invertibility condition (synthesized):
         * (bvult t ones)
         * where
         * ones = ~0 with getSize(ones) = w  */
        scl = nm->mkNode(Kind::BITVECTOR_ULT, t, bv::utils::mkOnes(nm, w));
      }
      else
      {
        /* x >> s <= t
         * with invertibility condition (synthesized):
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
    }
    else
    {
      if (pol)
      {
        /* s >> x > t
         * with invertibility condition (synthesized):
         * (or (bvslt s (bvlshr s (bvnot t))) (bvult t s))  */
        Node lshr = nm->mkNode(
            Kind::BITVECTOR_LSHR, s, nm->mkNode(Kind::BITVECTOR_NOT, t));
        Node ts = nm->mkNode(Kind::BITVECTOR_ULT, t, s);
        Node slt = nm->mkNode(Kind::BITVECTOR_SLT, s, lshr);
        scl = slt.orNode(ts);
      }
      else
      {
        /* s >> x <= t
         * with invertibility condition (synthesized):
         * (or (bvult s min) (bvuge t s))
         * where
         * min is the minimum signed value with getSize(min) = w  */
        Node min = bv::utils::mkMinSigned(nm, w);
        Node ult = nm->mkNode(Kind::BITVECTOR_ULT, s, min);
        Node uge = nm->mkNode(Kind::BITVECTOR_UGE, t, s);
        scl = ult.orNode(uge);
      }
    }
  }
  else if (litk == Kind::BITVECTOR_SLT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x >> s < t
         * with invertibility condition:
         * (bvslt (bvashr min s) t)
         * where
         * min is the minimum signed value with getSize(min) = w  */
        Node min = bv::utils::mkMinSigned(nm, w);
        scl = nm->mkNode(
            Kind::BITVECTOR_SLT, nm->mkNode(Kind::BITVECTOR_ASHR, min, s), t);
      }
      else
      {
        /* x >> s >= t
         * with invertibility condition:
         * (bvsge (bvlshr max s) t)
         * where
         * max is the signed maximum value with getSize(max) = w  */
        Node max = bv::utils::mkMaxSigned(nm, w);
        scl = nm->mkNode(
            Kind::BITVECTOR_SGE, nm->mkNode(Kind::BITVECTOR_LSHR, max, s), t);
      }
    }
    else
    {
      if (pol)
      {
        /* s >> x < t
         * with invertibility condition (synthesized):
         * (or (bvslt s t) (bvslt z t))
         * where
         * z = 0 and getSize(z) = w  */
        Node st = nm->mkNode(Kind::BITVECTOR_SLT, s, t);
        Node zt = nm->mkNode(Kind::BITVECTOR_SLT, z, t);
        scl = st.orNode(zt);
      }
      else
      {
        /* s >> x >= t
         * with invertibility condition (synthesized):
         * (not (and (bvult t (bvnot t)) (bvslt s t)))  */
        Node tt = nm->mkNode(
            Kind::BITVECTOR_ULT, t, nm->mkNode(Kind::BITVECTOR_NOT, t));
        Node st = nm->mkNode(Kind::BITVECTOR_SLT, s, t);
        scl = tt.andNode(st).notNode();
      }
    }
  }
  else
  {
    Assert(litk == Kind::BITVECTOR_SGT);
    Node max = bv::utils::mkMaxSigned(nm, w);
    if (idx == 0)
    {
      Node lshr = nm->mkNode(Kind::BITVECTOR_LSHR, max, s);
      if (pol)
      {
        /* x >> s > t
         * with invertibility condition (synthesized):
         * (bvslt t (bvlshr max s)))
         * where
         * max is the signed maximum value with getSize(max) = w  */
        scl = nm->mkNode(Kind::BITVECTOR_SLT, t, lshr);
      }
      else
      {
        /* x >> s <= t
         * with invertibility condition (synthesized):
         * (bvsge t (bvnot (bvlshr max s)))
         * where
         * max is the signed maximum value with getSize(max) = w  */
        scl = nm->mkNode(
            Kind::BITVECTOR_SGE, t, nm->mkNode(Kind::BITVECTOR_NOT, lshr));
      }
    }
    else
    {
      if (pol)
      {
        /* s >> x > t
         * with invertibility condition (synthesized):
         * (and (bvslt t (bvand s max)) (bvslt t (bvor s max)))
         * where
         * max is the signed maximum value with getSize(max) = w  */
        Node sam = nm->mkNode(Kind::BITVECTOR_AND, s, max);
        Node som = nm->mkNode(Kind::BITVECTOR_OR, s, max);
        Node slta = nm->mkNode(Kind::BITVECTOR_SLT, t, sam);
        Node slto = nm->mkNode(Kind::BITVECTOR_SLT, t, som);
        scl = slta.andNode(slto);
      }
      else
      {
        /* s >> x <= t
         * with invertibility condition (synthesized):
         * (or (bvsge t z) (bvsge t s))
         * where
         * z = 0 and getSize(z) = w  */
        Node tz = nm->mkNode(Kind::BITVECTOR_SGE, t, z);
        Node ts = nm->mkNode(Kind::BITVECTOR_SGE, t, s);
        scl = tz.orNode(ts);
      }
    }
  }
  Node scr =
      nm->mkNode(litk, idx == 0 ? nm->mkNode(k, x, s) : nm->mkNode(k, s, x), t);
  Node ic = nm->mkNode(Kind::IMPLIES, scl, pol ? scr : scr.notNode());
  Trace("bv-invert") << "Add SC_" << k << "(" << x << "): " << ic << std::endl;
  return ic;
}

Node getICBvShl(
    bool pol, Kind litk, Kind k, unsigned idx, Node x, Node s, Node t)
{
  Assert(k == Kind::BITVECTOR_SHL);
  Assert(litk == Kind::EQUAL || litk == Kind::BITVECTOR_ULT
         || litk == Kind::BITVECTOR_SLT || litk == Kind::BITVECTOR_UGT
         || litk == Kind::BITVECTOR_SGT);

  NodeManager* nm = x.getNodeManager();
  Node scl;
  unsigned w = bv::utils::getSize(s);
  Assert(w == bv::utils::getSize(t));
  Node z = bv::utils::mkZero(nm, w);

  if (litk == Kind::EQUAL)
  {
    if (idx == 0)
    {
      Node ww = bv::utils::mkConst(nm, w, w);

      if (pol)
      {
        /* x << s = t
         * with invertibility condition (synthesized):
         * (= (bvshl (bvlshr t s) s) t)  */
        Node lshr = nm->mkNode(Kind::BITVECTOR_LSHR, t, s);
        Node shl = nm->mkNode(Kind::BITVECTOR_SHL, lshr, s);
        scl = shl.eqNode(t);
      }
      else
      {
        /* x << s != t
         * with invertibility condition:
         * (or (distinct t z) (bvult s w))
         * with
         * w = getSize(s) = getSize(t)
         * and z = 0 with getSize(z) = w  */
        scl = nm->mkNode(Kind::OR,
                         t.eqNode(z).notNode(),
                         nm->mkNode(Kind::BITVECTOR_ULT, s, ww));
      }
    }
    else
    {
      if (pol)
      {
        /* s << x = t
         * with invertibility condition:
         * (or (= (bvshl s i) t) ...)
         * for i in 0..w  */
        scl = defaultShiftIC(Kind::EQUAL, Kind::BITVECTOR_SHL, s, t);
      }
      else
      {
        /* s << x != t
         * with invertibility condition:
         * (or (distinct s z) (distinct t z))
         * where
         * z = 0 with getSize(z) = w  */
        scl =
            nm->mkNode(Kind::OR, s.eqNode(z).notNode(), t.eqNode(z).notNode());
      }
    }
  }
  else if (litk == Kind::BITVECTOR_ULT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x << s < t
         * with invertibility condition (synthesized):
         * (not (= t z))  */
        scl = t.eqNode(z).notNode();
      }
      else
      {
        /* x << s >= t
         * with invertibility condition (synthesized):
         * (bvuge (bvshl ones s) t)  */
        Node shl = nm->mkNode(Kind::BITVECTOR_SHL, bv::utils::mkOnes(nm, w), s);
        scl = nm->mkNode(Kind::BITVECTOR_UGE, shl, t);
      }
    }
    else
    {
      if (pol)
      {
        /* s << x < t
         * with invertibility condition (synthesized):
         * (not (= t z))  */
        scl = t.eqNode(z).notNode();
      }
      else
      {
        /* s << x >= t
         * with invertibility condition:
         * (or (bvuge (bvshl s i) t) ...)
         * for i in 0..w  */
        scl = defaultShiftIC(Kind::BITVECTOR_UGE, Kind::BITVECTOR_SHL, s, t);
      }
    }
  }
  else if (litk == Kind::BITVECTOR_UGT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x << s > t
         * with invertibility condition (synthesized):
         * (bvult t (bvshl ones s))
         * where
         * ones = ~0 with getSize(ones) = w  */
        Node shl = nm->mkNode(Kind::BITVECTOR_SHL, bv::utils::mkOnes(nm, w), s);
        scl = nm->mkNode(Kind::BITVECTOR_ULT, t, shl);
      }
      else
      {
        /* x << s <= t
         * with invertibility condition:
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
    }
    else
    {
      if (pol)
      {
        /* s << x > t
         * with invertibility condition:
         * (or (bvugt (bvshl s i) t) ...)
         * for i in 0..w  */
        scl = defaultShiftIC(Kind::BITVECTOR_UGT, Kind::BITVECTOR_SHL, s, t);
      }
      else
      {
        /* s << x <= t
         * with invertibility condition:
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
    }
  }
  else if (litk == Kind::BITVECTOR_SLT)
  {
    if (idx == 0)
    {
      if (pol)
      {
        /* x << s < t
         * with invertibility condition (synthesized):
         * (bvslt (bvshl (bvlshr min s) s) t)
         * where
         * min is the signed minimum value with getSize(min) = w  */
        Node min = bv::utils::mkMinSigned(nm, w);
        Node lshr = nm->mkNode(Kind::BITVECTOR_LSHR, min, s);
        Node shl = nm->mkNode(Kind::BITVECTOR_SHL, lshr, s);
        scl = nm->mkNode(Kind::BITVECTOR_SLT, shl, t);
      }
      else
      {
        /* x << s >= t
         * with invertibility condition (synthesized):
         * (bvsge (bvand (bvshl max s) max) t)
         * where
         * max is the signed maximum value with getSize(max) = w  */
        Node max = bv::utils::mkMaxSigned(nm, w);
        Node shl = nm->mkNode(Kind::BITVECTOR_SHL, max, s);
        scl = nm->mkNode(
            Kind::BITVECTOR_SGE, nm->mkNode(Kind::BITVECTOR_AND, shl, max), t);
      }
    }
    else
    {
      if (pol)
      {
        /* s << x < t
         * with invertibility condition (synthesized):
         * (bvult (bvshl min s) (bvadd t min))
         * where
         * min is the signed minimum value with getSize(min) = w  */
        Node min = bv::utils::mkMinSigned(nm, w);
        Node shl = nm->mkNode(Kind::BITVECTOR_SHL, min, s);
        Node add = nm->mkNode(Kind::BITVECTOR_ADD, t, min);
        scl = nm->mkNode(Kind::BITVECTOR_ULT, shl, add);
      }
      else
      {
        /* s << x >= t
         * with invertibility condition:
         * (or (bvsge (bvshl s i) t) ...)
         * for i in 0..w  */
        scl = defaultShiftIC(Kind::BITVECTOR_SGE, Kind::BITVECTOR_SHL, s, t);
      }
    }
  }
  else
  {
    Assert(litk == Kind::BITVECTOR_SGT);
    if (idx == 0)
    {
      if (pol)
      {
        /* x << s > t
         * with invertibility condition (synthesized):
         * (bvslt t (bvand (bvshl max s) max))
         * where
         * max is the signed maximum value with getSize(max) = w  */
        Node max = bv::utils::mkMaxSigned(nm, w);
        Node shl = nm->mkNode(Kind::BITVECTOR_SHL, max, s);
        scl = nm->mkNode(
            Kind::BITVECTOR_SLT, t, nm->mkNode(Kind::BITVECTOR_AND, shl, max));
      }
      else
      {
        /* x << s <= t
         * with invertibility condition (synthesized):
         * (bvult (bvlshr t (bvlshr t s)) min)
         * where
         * min is the signed minimum value with getSize(min) = w  */
        Node min = bv::utils::mkMinSigned(nm, w);
        Node ts = nm->mkNode(Kind::BITVECTOR_LSHR, t, s);
        scl = nm->mkNode(
            Kind::BITVECTOR_ULT, nm->mkNode(Kind::BITVECTOR_LSHR, t, ts), min);
      }
    }
    else
    {
      if (pol)
      {
        /* s << x > t
         * with invertibility condition:
         * (or (bvsgt (bvshl s i) t) ...)
         * for i in 0..w  */
        scl = defaultShiftIC(Kind::BITVECTOR_SGT, Kind::BITVECTOR_SHL, s, t);
      }
      else
      {
        /* s << x <= t
         * with invertibility condition (synthesized):
         * (bvult (bvlshr t s) min)
         * where
         * min is the signed minimum value with getSize(min) = w  */
        Node min = bv::utils::mkMinSigned(nm, w);
        scl = nm->mkNode(
            Kind::BITVECTOR_ULT, nm->mkNode(Kind::BITVECTOR_LSHR, t, s), min);
      }
    }
  }
  Node scr =
      nm->mkNode(litk, idx == 0 ? nm->mkNode(k, x, s) : nm->mkNode(k, s, x), t);
  Node ic = nm->mkNode(Kind::IMPLIES, scl, pol ? scr : scr.notNode());
  Trace("bv-invert") << "Add SC_" << k << "(" << x << "): " << ic << std::endl;
  return ic;
}

Node getICBvConcat(bool pol, Kind litk, unsigned idx, Node x, Node sv_t, Node t)
{
  Assert(litk == Kind::EQUAL || litk == Kind::BITVECTOR_ULT
         || litk == Kind::BITVECTOR_SLT || litk == Kind::BITVECTOR_UGT
         || litk == Kind::BITVECTOR_SGT);

  Kind k = sv_t.getKind();
  Assert(k == Kind::BITVECTOR_CONCAT);
  NodeManager* nm = x.getNodeManager();
  unsigned nchildren = sv_t.getNumChildren();
  unsigned w1 = 0, w2 = 0;
  unsigned w = bv::utils::getSize(t), wx = bv::utils::getSize(x);
  NodeBuilder nbs1(nm, Kind::BITVECTOR_CONCAT),
      nbs2(nm, Kind::BITVECTOR_CONCAT);
  Node s1, s2;
  Node t1, t2, tx;
  Node scl, scr;

  if (idx != 0)
  {
    if (idx == 1)
    {
      s1 = sv_t[0];
    }
    else
    {
      for (unsigned i = 0; i < idx; ++i)
      {
        nbs1 << sv_t[i];
      }
      s1 = nbs1.constructNode();
    }
    w1 = bv::utils::getSize(s1);
    t1 = bv::utils::mkExtract(t, w - 1, w - w1);
  }

  tx = bv::utils::mkExtract(t, w - w1 - 1, w - w1 - wx);

  if (idx != nchildren - 1)
  {
    if (idx == nchildren - 2)
    {
      s2 = sv_t[nchildren - 1];
    }
    else
    {
      for (unsigned i = idx + 1; i < nchildren; ++i)
      {
        nbs2 << sv_t[i];
      }
      s2 = nbs2.constructNode();
    }
    w2 = bv::utils::getSize(s2);
    Assert(w2 == w - w1 - wx);
    t2 = bv::utils::mkExtract(t, w2 - 1, 0);
  }

  Assert(!s1.isNull() || t1.isNull());
  Assert(!s2.isNull() || t2.isNull());
  Assert(!s1.isNull() || !s2.isNull());
  Assert(s1.isNull() || w1 == bv::utils::getSize(t1));
  Assert(s2.isNull() || w2 == bv::utils::getSize(t2));

  if (litk == Kind::EQUAL)
  {
    if (s1.isNull())
    {
      if (pol)
      {
        /* x o s2 = t  (interpret t as tx o t2)
         * with invertibility condition:
         * (= s2 t2)  */
        scl = s2.eqNode(t2);
      }
      else
      {
        /* x o s2 != t
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
    }
    else if (s2.isNull())
    {
      if (pol)
      {
        /* s1 o x = t  (interpret t as t1 o tx)
         * with invertibility condition:
         * (= s1 t1)  */
        scl = s1.eqNode(t1);
      }
      else
      {
        /* s1 o x != t
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
    }
    else
    {
      if (pol)
      {
        /* s1 o x o s2 = t  (interpret t as t1 o tx o t2)
         * with invertibility condition:
         * (and (= s1 t1) (= s2 t2)) */
        scl = nm->mkNode(Kind::AND, s1.eqNode(t1), s2.eqNode(t2));
      }
      else
      {
        /* s1 o x o s2 != t
         * true (no invertibility condition)  */
        scl = nm->mkConst<bool>(true);
      }
    }
  }
  else if (litk == Kind::BITVECTOR_ULT)
  {
    if (s1.isNull())
    {
      if (pol)
      {
        /* x o s2 < t  (interpret t as tx o t2)
         * with invertibility condition:
         * (=> (= tx z) (bvult s2 t2))
         * where
         * z = 0 with getSize(z) = wx  */
        Node z = bv::utils::mkZero(nm, wx);
        Node ult = nm->mkNode(Kind::BITVECTOR_ULT, s2, t2);
        scl = nm->mkNode(Kind::IMPLIES, tx.eqNode(z), ult);
      }
      else
      {
        /* x o s2 >= t  (interpret t as tx o t2)
         * (=> (= tx ones) (bvuge s2 t2))
         * where
         * ones = ~0 with getSize(ones) = wx  */
        Node n = bv::utils::mkOnes(nm, wx);
        Node uge = nm->mkNode(Kind::BITVECTOR_UGE, s2, t2);
        scl = nm->mkNode(Kind::IMPLIES, tx.eqNode(n), uge);
      }
    }
    else if (s2.isNull())
    {
      if (pol)
      {
        /* s1 o x < t  (interpret t as t1 o tx)
         * with invertibility condition:
         * (and (bvule s1 t1) (=> (= s1 t1) (distinct tx z)))
         * where
         * z = 0 with getSize(z) = wx  */
        Node z = bv::utils::mkZero(nm, wx);
        Node ule = nm->mkNode(Kind::BITVECTOR_ULE, s1, t1);
        Node imp =
            nm->mkNode(Kind::IMPLIES, s1.eqNode(t1), tx.eqNode(z).notNode());
        scl = nm->mkNode(Kind::AND, ule, imp);
      }
      else
      {
        /* s1 o x >= t  (interpret t as t1 o tx)
         * with invertibility condition:
         * (bvuge s1 t1)  */
        scl = nm->mkNode(Kind::BITVECTOR_UGE, s1, t1);
      }
    }
    else
    {
      if (pol)
      {
        /* s1 o x o s2 < t  (interpret t as t1 o tx o t2)
         * with invertibility condition:
         * (and
         *   (bvule s1 t1)
         *   (=> (and (= s1 t1) (= tx z)) (bvult s2 t2)))
         * where
         * z = 0 with getSize(z) = wx  */
        Node z = bv::utils::mkZero(nm, wx);
        Node ule = nm->mkNode(Kind::BITVECTOR_ULE, s1, t1);
        Node a = nm->mkNode(Kind::AND, s1.eqNode(t1), tx.eqNode(z));
        Node imp = nm->mkNode(
            Kind::IMPLIES, a, nm->mkNode(Kind::BITVECTOR_ULT, s2, t2));
        scl = nm->mkNode(Kind::AND, ule, imp);
      }
      else
      {
        /* s1 o x o s2 >= t  (interpret t as t1 o tx o t2)
         * with invertibility condition:
         * (and
         *   (bvuge s1 t1)
         *   (=> (and (= s1 t1) (= tx ones)) (bvuge s2 t2)))
         * where
         * ones = ~0 with getSize(ones) = wx  */
        Node n = bv::utils::mkOnes(nm, wx);
        Node uge = nm->mkNode(Kind::BITVECTOR_UGE, s1, t1);
        Node a = nm->mkNode(Kind::AND, s1.eqNode(t1), tx.eqNode(n));
        Node imp = nm->mkNode(
            Kind::IMPLIES, a, nm->mkNode(Kind::BITVECTOR_UGE, s2, t2));
        scl = nm->mkNode(Kind::AND, uge, imp);
      }
    }
  }
  else if (litk == Kind::BITVECTOR_UGT)
  {
    if (s1.isNull())
    {
      if (pol)
      {
        /* x o s2 > t  (interpret t as tx o t2)
         * with invertibility condition:
         * (=> (= tx ones) (bvugt s2 t2))
         * where
         * ones = ~0 with getSize(ones) = wx  */
        Node n = bv::utils::mkOnes(nm, wx);
        Node ugt = nm->mkNode(Kind::BITVECTOR_UGT, s2, t2);
        scl = nm->mkNode(Kind::IMPLIES, tx.eqNode(n), ugt);
      }
      else
      {
        /* x o s2 <= t  (interpret t as tx o t2)
         * with invertibility condition:
         * (=> (= tx z) (bvule s2 t2))
         * where
         * z = 0 with getSize(z) = wx  */
        Node z = bv::utils::mkZero(nm, wx);
        Node ule = nm->mkNode(Kind::BITVECTOR_ULE, s2, t2);
        scl = nm->mkNode(Kind::IMPLIES, tx.eqNode(z), ule);
      }
    }
    else if (s2.isNull())
    {
      if (pol)
      {
        /* s1 o x > t  (interpret t as t1 o tx)
         * with invertibility condition:
         * (and (bvuge s1 t1) (=> (= s1 t1) (distinct tx ones)))
         * where
         * ones = ~0 with getSize(ones) = wx  */
        Node n = bv::utils::mkOnes(nm, wx);
        Node uge = nm->mkNode(Kind::BITVECTOR_UGE, s1, t1);
        Node imp =
            nm->mkNode(Kind::IMPLIES, s1.eqNode(t1), tx.eqNode(n).notNode());
        scl = nm->mkNode(Kind::AND, uge, imp);
      }
      else
      {
        /* s1 o x <= t  (interpret t as t1 o tx)
         * with invertibility condition:
         * (bvule s1 t1)  */
        scl = nm->mkNode(Kind::BITVECTOR_ULE, s1, t1);
      }
    }
    else
    {
      if (pol)
      {
        /* s1 o x o s2 > t  (interpret t as t1 o tx o t2)
         * with invertibility condition:
         * (and
         *   (bvuge s1 t1)
         *   (=> (and (= s1 t1) (= tx ones)) (bvugt s2 t2)))
         * where
         * ones = ~0 with getSize(ones) = wx  */
        Node n = bv::utils::mkOnes(nm, wx);
        Node uge = nm->mkNode(Kind::BITVECTOR_UGE, s1, t1);
        Node a = nm->mkNode(Kind::AND, s1.eqNode(t1), tx.eqNode(n));
        Node imp = nm->mkNode(
            Kind::IMPLIES, a, nm->mkNode(Kind::BITVECTOR_UGT, s2, t2));
        scl = nm->mkNode(Kind::AND, uge, imp);
      }
      else
      {
        /* s1 o x o s2 <= t  (interpret t as t1 o tx o t2)
         * with invertibility condition:
         * (and
         *   (bvule s1 t1)
         *   (=> (and (= s1 t1) (= tx z)) (bvule s2 t2)))
         * where
         * z = 0 with getSize(z) = wx  */
        Node z = bv::utils::mkZero(nm, wx);
        Node ule = nm->mkNode(Kind::BITVECTOR_ULE, s1, t1);
        Node a = nm->mkNode(Kind::AND, s1.eqNode(t1), tx.eqNode(z));
        Node imp = nm->mkNode(
            Kind::IMPLIES, a, nm->mkNode(Kind::BITVECTOR_ULE, s2, t2));
        scl = nm->mkNode(Kind::AND, ule, imp);
      }
    }
  }
  else if (litk == Kind::BITVECTOR_SLT)
  {
    if (s1.isNull())
    {
      if (pol)
      {
        /* x o s2 < t  (interpret t as tx o t2)
         * with invertibility condition:
         * (=> (= tx min) (bvult s2 t2))
         * where
         * min is the signed minimum value with getSize(min) = wx  */
        Node min = bv::utils::mkMinSigned(nm, wx);
        Node ult = nm->mkNode(Kind::BITVECTOR_ULT, s2, t2);
        scl = nm->mkNode(Kind::IMPLIES, tx.eqNode(min), ult);
      }
      else
      {
        /* x o s2 >= t  (interpret t as tx o t2)
         * (=> (= tx max) (bvuge s2 t2))
         * where
         * max is the signed maximum value with getSize(max) = wx  */
        Node max = bv::utils::mkMaxSigned(nm, wx);
        Node uge = nm->mkNode(Kind::BITVECTOR_UGE, s2, t2);
        scl = nm->mkNode(Kind::IMPLIES, tx.eqNode(max), uge);
      }
    }
    else if (s2.isNull())
    {
      if (pol)
      {
        /* s1 o x < t  (interpret t as t1 o tx)
         * with invertibility condition:
         * (and (bvsle s1 t1) (=> (= s1 t1) (distinct tx z)))
         * where
         * z = 0 with getSize(z) = wx  */
        Node z = bv::utils::mkZero(nm, wx);
        Node sle = nm->mkNode(Kind::BITVECTOR_SLE, s1, t1);
        Node imp =
            nm->mkNode(Kind::IMPLIES, s1.eqNode(t1), tx.eqNode(z).notNode());
        scl = nm->mkNode(Kind::AND, sle, imp);
      }
      else
      {
        /* s1 o x >= t  (interpret t as t1 o tx)
         * with invertibility condition:
         * (bvsge s1 t1)  */
        scl = nm->mkNode(Kind::BITVECTOR_SGE, s1, t1);
      }
    }
    else
    {
      if (pol)
      {
        /* s1 o x o s2 < t  (interpret t as t1 o tx o t2)
         * with invertibility condition:
         * (and
         *   (bvsle s1 t1)
         *   (=> (and (= s1 t1) (= tx z)) (bvult s2 t2)))
         * where
         * z = 0 with getSize(z) = wx  */
        Node z = bv::utils::mkZero(nm, wx);
        Node sle = nm->mkNode(Kind::BITVECTOR_SLE, s1, t1);
        Node a = nm->mkNode(Kind::AND, s1.eqNode(t1), tx.eqNode(z));
        Node imp = nm->mkNode(
            Kind::IMPLIES, a, nm->mkNode(Kind::BITVECTOR_ULT, s2, t2));
        scl = nm->mkNode(Kind::AND, sle, imp);
      }
      else
      {
        /* s1 o x o s2 >= t  (interpret t as t1 o tx o t2)
         * with invertibility condition:
         * (and
         *   (bvsge s1 t1)
         *   (=> (and (= s1 t1) (= tx ones)) (bvuge s2 t2)))
         * where
         * ones = ~0 with getSize(ones) = wx  */
        Node n = bv::utils::mkOnes(nm, wx);
        Node sge = nm->mkNode(Kind::BITVECTOR_SGE, s1, t1);
        Node a = nm->mkNode(Kind::AND, s1.eqNode(t1), tx.eqNode(n));
        Node imp = nm->mkNode(
            Kind::IMPLIES, a, nm->mkNode(Kind::BITVECTOR_UGE, s2, t2));
        scl = nm->mkNode(Kind::AND, sge, imp);
      }
    }
  }
  else
  {
    Assert(litk == Kind::BITVECTOR_SGT);
    if (s1.isNull())
    {
      if (pol)
      {
        /* x o s2 > t  (interpret t as tx o t2)
         * with invertibility condition:
         * (=> (= tx max) (bvugt s2 t2))
         * where
         * max is the signed maximum value with getSize(max) = wx  */
        Node max = bv::utils::mkMaxSigned(nm, wx);
        Node ugt = nm->mkNode(Kind::BITVECTOR_UGT, s2, t2);
        scl = nm->mkNode(Kind::IMPLIES, tx.eqNode(max), ugt);
      }
      else
      {
        /* x o s2 <= t  (interpret t as tx o t2)
         * with invertibility condition:
         * (=> (= tx min) (bvule s2 t2))
         * where
         * min is the signed minimum value with getSize(min) = wx  */
        Node min = bv::utils::mkMinSigned(nm, wx);
        Node ule = nm->mkNode(Kind::BITVECTOR_ULE, s2, t2);
        scl = nm->mkNode(Kind::IMPLIES, tx.eqNode(min), ule);
      }
    }
    else if (s2.isNull())
    {
      if (pol)
      {
        /* s1 o x > t  (interpret t as t1 o tx)
         * with invertibility condition:
         * (and (bvsge s1 t1) (=> (= s1 t1) (distinct tx ones)))
         * where
         * ones = ~0 with getSize(ones) = wx  */
        Node n = bv::utils::mkOnes(nm, wx);
        Node sge = nm->mkNode(Kind::BITVECTOR_SGE, s1, t1);
        Node imp =
            nm->mkNode(Kind::IMPLIES, s1.eqNode(t1), tx.eqNode(n).notNode());
        scl = nm->mkNode(Kind::AND, sge, imp);
      }
      else
      {
        /* s1 o x <= t  (interpret t as t1 o tx)
         * with invertibility condition:
         * (bvsle s1 t1)  */
        scl = nm->mkNode(Kind::BITVECTOR_SLE, s1, t1);
      }
    }
    else
    {
      if (pol)
      {
        /* s1 o x o s2 > t  (interpret t as t1 o tx o t2)
         * with invertibility condition:
         * (and
         *   (bvsge s1 t1)
         *   (=> (and (= s1 t1) (= tx ones)) (bvugt s2 t2)))
         * where
         * ones = ~0 with getSize(ones) = wx  */
        Node n = bv::utils::mkOnes(nm, wx);
        Node sge = nm->mkNode(Kind::BITVECTOR_SGE, s1, t1);
        Node a = nm->mkNode(Kind::AND, s1.eqNode(t1), tx.eqNode(n));
        Node imp = nm->mkNode(
            Kind::IMPLIES, a, nm->mkNode(Kind::BITVECTOR_UGT, s2, t2));
        scl = nm->mkNode(Kind::AND, sge, imp);
      }
      else
      {
        /* s1 o x o s2 <= t  (interpret t as t1 o tx o t2)
         * with invertibility condition:
         * (and
         *   (bvsle s1 t1)
         *   (=> (and (= s1 t1) (= tx z)) (bvule s2 t2)))
         * where
         * z = 0 with getSize(z) = wx  */
        Node z = bv::utils::mkZero(nm, wx);
        Node sle = nm->mkNode(Kind::BITVECTOR_SLE, s1, t1);
        Node a = nm->mkNode(Kind::AND, s1.eqNode(t1), tx.eqNode(z));
        Node imp = nm->mkNode(
            Kind::IMPLIES, a, nm->mkNode(Kind::BITVECTOR_ULE, s2, t2));
        scl = nm->mkNode(Kind::AND, sle, imp);
      }
    }
  }
  scr = s1.isNull() ? x : bv::utils::mkConcat(s1, x);
  if (!s2.isNull()) scr = bv::utils::mkConcat(scr, s2);
  scr = nm->mkNode(litk, scr, t);
  Node ic = nm->mkNode(Kind::IMPLIES, scl, pol ? scr : scr.notNode());
  Trace("bv-invert") << "Add SC_" << k << "(" << x << "): " << ic << std::endl;
  return ic;
}

Node getICBvSext(bool pol, Kind litk, unsigned idx, Node x, Node sv_t, Node t)
{
  Assert(litk == Kind::EQUAL || litk == Kind::BITVECTOR_ULT
         || litk == Kind::BITVECTOR_SLT || litk == Kind::BITVECTOR_UGT
         || litk == Kind::BITVECTOR_SGT);

  NodeManager* nm = x.getNodeManager();
  Node scl;
  Assert(idx == 0);
  (void)idx;
  unsigned ws = bv::utils::getSignExtendAmount(sv_t);
  unsigned w = bv::utils::getSize(t);

  if (litk == Kind::EQUAL)
  {
    if (pol)
    {
      /* x sext ws = t
       * with invertibility condition:
       * (or (= ((_ extract u l) t) z)
       *     (= ((_ extract u l) t) ones))
       * where
       * u = w - 1
       * l = w - 1 - ws
       * z = 0 with getSize(z) = ws + 1
       * ones = ~0 with getSize(ones) = ws + 1  */
      Node ext = bv::utils::mkExtract(t, w - 1, w - 1 - ws);
      Node z = bv::utils::mkZero(nm, ws + 1);
      Node n = bv::utils::mkOnes(nm, ws + 1);
      scl = nm->mkNode(Kind::OR, ext.eqNode(z), ext.eqNode(n));
    }
    else
    {
      /* x sext ws != t
       * true (no invertibility condition)  */
      scl = nm->mkConst<bool>(true);
    }
  }
  else if (litk == Kind::BITVECTOR_ULT)
  {
    if (pol)
    {
      /* x sext ws < t
       * with invertibility condition:
       * (distinct t z)
       * where
       * z = 0 with getSize(z) = w  */
      Node z = bv::utils::mkZero(nm, w);
      scl = t.eqNode(z).notNode();
    }
    else
    {
      /* x sext ws >= t
       * true (no invertibility condition)  */
      scl = nm->mkConst<bool>(true);
    }
  }
  else if (litk == Kind::BITVECTOR_UGT)
  {
    if (pol)
    {
      /* x sext ws > t
       * with invertibility condition:
       * (distinct t ones)
       * where
       * ones = ~0 with getSize(ones) = w  */
      Node n = bv::utils::mkOnes(nm, w);
      scl = t.eqNode(n).notNode();
    }
    else
    {
      /* x sext ws <= t
       * true (no invertibility condition)  */
      scl = nm->mkConst<bool>(true);
    }
  }
  else if (litk == Kind::BITVECTOR_SLT)
  {
    if (pol)
    {
      /* x sext ws < t
       * with invertibility condition:
       * (bvslt ((_ sign_extend ws) min) t)
       * where
       * min is the signed minimum value with getSize(min) = w - ws  */
      Node min = bv::utils::mkMinSigned(nm, w - ws);
      Node ext = bv::utils::mkSignExtend(min, ws);
      scl = nm->mkNode(Kind::BITVECTOR_SLT, ext, t);
    }
    else
    {
      /* x sext ws >= t
       * with invertibility condition (combination of sgt and eq):
       *
       * (or
       *   (or (= ((_ extract u l) t) z)         ; eq
       *       (= ((_ extract u l) t) ones))
       *   (bvslt t ((_ zero_extend ws) max)))   ; sgt
       * where
       * u = w - 1
       * l = w - 1 - ws
       * z = 0 with getSize(z) = ws + 1
       * ones = ~0 with getSize(ones) = ws + 1
       * max is the signed maximum value with getSize(max) = w - ws  */
      Node ext1 = bv::utils::mkExtract(t, w - 1, w - 1 - ws);
      Node z = bv::utils::mkZero(nm, ws + 1);
      Node n = bv::utils::mkOnes(nm, ws + 1);
      Node o1 = nm->mkNode(Kind::OR, ext1.eqNode(z), ext1.eqNode(n));
      Node max = bv::utils::mkMaxSigned(nm, w - ws);
      Node ext2 = bv::utils::mkConcat(bv::utils::mkZero(nm, ws), max);
      Node o2 = nm->mkNode(Kind::BITVECTOR_SLT, t, ext2);
      scl = nm->mkNode(Kind::OR, o1, o2);
    }
  }
  else
  {
    Assert(litk == Kind::BITVECTOR_SGT);
    if (pol)
    {
      /* x sext ws > t
       * with invertibility condition:
       * (bvslt t ((_ zero_extend ws) max))
       * where
       * max is the signed maximum value with getSize(max) = w - ws  */
      Node max = bv::utils::mkMaxSigned(nm, w - ws);
      Node ext = bv::utils::mkConcat(bv::utils::mkZero(nm, ws), max);
      scl = nm->mkNode(Kind::BITVECTOR_SLT, t, ext);
    }
    else
    {
      /* x sext ws <= t
       * with invertibility condition:
       * (bvsge t (bvnot ((_ zero_extend ws) max)))
       * where
       * max is the signed maximum value with getSize(max) = w - ws  */
      Node max = bv::utils::mkMaxSigned(nm, w - ws);
      Node ext = bv::utils::mkConcat(bv::utils::mkZero(nm, ws), max);
      scl = nm->mkNode(
          Kind::BITVECTOR_SGE, t, nm->mkNode(Kind::BITVECTOR_NOT, ext));
    }
  }
  Node scr = nm->mkNode(litk, bv::utils::mkSignExtend(x, ws), t);
  Node ic = nm->mkNode(Kind::IMPLIES, scl, pol ? scr : scr.notNode());
  Trace("bv-invert") << "Add SC_" << Kind::BITVECTOR_SIGN_EXTEND << "(" << x
                     << "): " << ic << std::endl;
  return ic;
}

}  // namespace utils
}  // namespace quantifiers
}  // namespace theory
}  // namespace cvc5::internal
