/******************************************************************************
 * Top contributors (to current version):
 *   Andrew Reynolds, Aina Niemetz, Paul Meng
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2025 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Implementation of QuantifiersAttributes class.
 */

#include "theory/quantifiers/quantifiers_attributes.h"

#include "expr/skolem_manager.h"
#include "options/quantifiers_options.h"
#include "smt/print_benchmark.h"
#include "theory/arith/arith_msum.h"
#include "theory/quantifiers/fmf/bounded_integers.h"
#include "theory/quantifiers/sygus/synth_engine.h"
#include "theory/quantifiers/term_util.h"
#include "util/rational.h"
#include "util/string.h"

using namespace std;
using namespace cvc5::internal::kind;
using namespace cvc5::context;

namespace cvc5::internal {
namespace theory {
namespace quantifiers {

/**
 * Mapping from terms to their "instantiation level", for details see
 * QuantAttributes::getInstantiationLevel.
 */
struct InstLevelAttributeId
{
};
using InstLevelAttribute = expr::Attribute<InstLevelAttributeId, uint64_t>;

/** Attribute true for quantifiers we are doing quantifier elimination on */
struct QuantElimAttributeId
{
};
using QuantElimAttribute = expr::Attribute<QuantElimAttributeId, bool>;

/** Attribute true for quantifiers we which to preserve structure for, including
 * those that we are doing quantifier elimination on */
struct PreserveStructureAttributeId
{
};
using PreserveStructureAttribute =
    expr::Attribute<PreserveStructureAttributeId, bool>;

bool QAttributes::isStandard() const
{
  return !d_sygus && !d_preserveStructure && !isFunDef() && !isOracleInterface()
         && !d_isQuantBounded && !d_hasPool;
}

QuantAttributes::QuantAttributes() {}

void QuantAttributes::setUserAttribute(const std::string& attr,
                                       TNode n,
                                       const std::vector<Node>& nodeValues)
{
  Trace("quant-attr-debug") << "Set " << attr << " " << n << std::endl;
  if (attr == "fun-def")
  {
    Trace("quant-attr-debug") << "Set function definition " << n << std::endl;
    FunDefAttribute fda;
    n.setAttribute( fda, true );
  }
  else if (attr == "qid")
  {
    // using z3 syntax "qid"
    Trace("quant-attr-debug") << "Set quant-name " << n << std::endl;
    QuantNameAttribute qna;
    n.setAttribute(qna, true);
  }else if( attr=="quant-inst-max-level" ){
    Assert(nodeValues.size() == 1);
    uint64_t lvl = nodeValues[0].getConst<Rational>().getNumerator().getLong();
    Trace("quant-attr-debug") << "Set instantiation level " << n << " to " << lvl << std::endl;
    QuantInstLevelAttribute qila;
    n.setAttribute( qila, lvl );
  }else if( attr=="quant-elim" ){
    Trace("quant-attr-debug") << "Set quantifier elimination " << n << std::endl;
    QuantElimAttribute qea;
    n.setAttribute( qea, true );
  }else if( attr=="quant-elim-partial" ){
    Trace("quant-attr-debug") << "Set partial quantifier elimination " << n << std::endl;
    QuantElimPartialAttribute qepa;
    n.setAttribute( qepa, true );
  }
}

Node QuantAttributes::getFunDefHead( Node q ) {
  //&& q[1].getKind()==EQUAL && q[1][0].getKind()==APPLY_UF &&
  if (q.getKind() == Kind::FORALL && q.getNumChildren() == 3)
  {
    Node ipl = q[2];
    for (unsigned i = 0; i < ipl.getNumChildren(); i++)
    {
      if (ipl[i].getKind() == Kind::INST_ATTRIBUTE
          && ipl[i][0].getAttribute(FunDefAttribute()))
      {
        return ipl[i][0];
      }
    }
  }
  return Node::null();
}
Node QuantAttributes::getFunDefBody( Node q ) {
  Node h = getFunDefHead( q );
  if( !h.isNull() ){
    if (q[1].getKind() == Kind::EQUAL)
    {
      if( q[1][0]==h ){
        return q[1][1];
      }else if( q[1][1]==h ){
        return q[1][0];
      }
      else if (q[1][0].getType().isRealOrInt())
      {
        // solve for h in the equality
        std::map<Node, Node> msum;
        if (ArithMSum::getMonomialSumLit(q[1], msum))
        {
          Node veq;
          int res = ArithMSum::isolate(h, msum, veq, Kind::EQUAL);
          if (res != 0)
          {
            Assert(veq.getKind() == Kind::EQUAL);
            return res == 1 ? veq[1] : veq[0];
          }
        }
      }
    }
    else
    {
      Node atom = q[1].getKind() == Kind::NOT ? q[1][0] : q[1];
      bool pol = q[1].getKind() != Kind::NOT;
      if( atom==h ){
        return q.getNodeManager()->mkConst(pol);
      }
    }
  }
  return Node::null();
}

bool QuantAttributes::checkSygusConjecture( Node q ) {
  return (q.getKind() == Kind::FORALL && q.getNumChildren() == 3)
             ? checkSygusConjectureAnnotation(q[2])
             : false;
}

bool QuantAttributes::checkSygusConjectureAnnotation( Node ipl ){
  if( !ipl.isNull() ){
    for( unsigned i=0; i<ipl.getNumChildren(); i++ ){
      if (ipl[i].getKind() == Kind::INST_ATTRIBUTE)
      {
        Node avar = ipl[i][0];
        if( avar.getAttribute(SygusAttribute()) ){
          return true;
        }
      }
    }
  }
  return false;
}

bool QuantAttributes::hasPattern(Node q)
{
  Assert(q.getKind() == Kind::FORALL);
  if (q.getNumChildren() != 3)
  {
    return false;
  }
  for (const Node& qc : q[2])
  {
    if (qc.getKind() == Kind::INST_PATTERN
        || qc.getKind() == Kind::INST_NO_PATTERN)
    {
      return true;
    }
  }
  return false;
}

void QuantAttributes::computeAttributes( Node q ) {
  computeQuantAttributes( q, d_qattr[q] );
  QAttributes& qa = d_qattr[q];
  if (qa.isFunDef())
  {
    Node f = qa.d_fundef_f;
    if( d_fun_defs.find( f )!=d_fun_defs.end() ){
      AlwaysAssert(false) << "Cannot define function " << f
                          << " more than once." << std::endl;
    }
    d_fun_defs[f] = true;
  }
}

void QuantAttributes::computeQuantAttributes( Node q, QAttributes& qa ){
  Trace("quant-attr-debug") << "Compute attributes for " << q << std::endl;
  if( q.getNumChildren()==3 ){
    NodeManager* nm = q.getNodeManager();
    qa.d_ipl = q[2];
    for( unsigned i=0; i<q[2].getNumChildren(); i++ ){
      Kind k = q[2][i].getKind();
      Trace("quant-attr-debug")
          << "Check : " << q[2][i] << " " << k << std::endl;
      if (k == Kind::INST_PATTERN || k == Kind::INST_NO_PATTERN)
      {
        qa.d_hasPattern = true;
      }
      else if (k == Kind::INST_POOL || k == Kind::INST_ADD_TO_POOL
               || k == Kind::SKOLEM_ADD_TO_POOL)
      {
        qa.d_hasPool = true;
      }
      else if (k == Kind::INST_ATTRIBUTE)
      {
        Node avar;
        // We support two use cases of INST_ATTRIBUTE:
        // (1) where the user constructs a term of the form
        // (INST_ATTRIBUTE "keyword" [nodeValues])
        // (2) where we internally generate nodes of the form
        // (INST_ATTRIBUTE v) where v has an internal attribute set on it.
        // We distinguish these two cases by checking the kind of the first
        // child.
        if (q[2][i][0].getKind() == Kind::CONST_STRING)
        {
          // make a dummy variable to be used below
          avar = NodeManager::mkBoundVar(nm->booleanType());
          std::vector<Node> nodeValues(q[2][i].begin() + 1, q[2][i].end());
          // set user attribute on the dummy variable
          setUserAttribute(
              q[2][i][0].getConst<String>().toString(), avar, nodeValues);
        }
        else
        {
          // assume the dummy variable has already had its attributes set
          avar = q[2][i][0];
        }
        if( avar.getAttribute(FunDefAttribute()) ){
          Trace("quant-attr") << "Attribute : function definition : " << q << std::endl;
          //get operator directly from pattern
          qa.d_fundef_f = q[2][i][0].getOperator();
        }
        if( avar.getAttribute(SygusAttribute()) ){
          //not necessarily nested existential
          //Assert( q[1].getKind()==NOT );
          //Assert( q[1][0].getKind()==FORALL );
          Trace("quant-attr") << "Attribute : sygus : " << q << std::endl;
          qa.d_sygus = true;
        }
        // oracles are specified by a distinguished variable kind
        if (avar.getKind() == Kind::ORACLE)
        {
          qa.d_oracle = avar;
          Trace("quant-attr")
              << "Attribute : oracle interface : " << q << std::endl;
        }
        if (avar.hasAttribute(SygusSideConditionAttribute()))
        {
          qa.d_sygusSideCondition =
              avar.getAttribute(SygusSideConditionAttribute());
          Trace("quant-attr")
              << "Attribute : sygus side condition : "
              << qa.d_sygusSideCondition << " : " << q << std::endl;
        }
        if (avar.getAttribute(QuantNameAttribute()))
        {
          // only set the name if there is a value
          if (q[2][i].getNumChildren() > 1)
          {
            std::string name = q[2][i][1].getName();
            // mark that this symbol should not be printed with the print
            // benchmark utility
            Node sym = q[2][i][1];
            smt::PrintBenchmark::markNoPrint(sym);
            Trace("quant-attr") << "Attribute : quantifier name : " << name
                                << " for " << q << std::endl;
            // assign the name to a variable with the given name (to avoid
            // enclosing the name in quotes)
            qa.d_name = NodeManager::mkBoundVar(name, nm->booleanType());
          }
          else
          {
            Warning() << "Missing name for qid attribute";
          }
        }
        if( avar.hasAttribute(QuantInstLevelAttribute()) ){
          qa.d_qinstLevel = avar.getAttribute(QuantInstLevelAttribute());
          Trace("quant-attr") << "Attribute : quant inst level " << qa.d_qinstLevel << " : " << q << std::endl;
        }
        if (avar.getAttribute(PreserveStructureAttribute()))
        {
          Trace("quant-attr")
              << "Attribute : preserve structure : " << q << std::endl;
          qa.d_preserveStructure = true;
        }
        if( avar.getAttribute(QuantElimAttribute()) ){
          Trace("quant-attr") << "Attribute : quantifier elimination : " << q << std::endl;
          qa.d_preserveStructure = true;
          qa.d_quant_elim = true;
          //don't set owner, should happen naturally
        }
        if( avar.getAttribute(QuantElimPartialAttribute()) ){
          Trace("quant-attr") << "Attribute : quantifier elimination partial : " << q << std::endl;
          qa.d_preserveStructure = true;
          qa.d_quant_elim = true;
          qa.d_quant_elim_partial = true;
          //don't set owner, should happen naturally
        }
        if (BoundedIntegers::isBoundedForallAttribute(avar))
        {
          Trace("quant-attr")
              << "Attribute : bounded quantifiers : " << q << std::endl;
          qa.d_isQuantBounded = true;
        }
        if( avar.hasAttribute(QuantIdNumAttribute()) ){
          qa.d_qid_num = avar;
          Trace("quant-attr") << "Attribute : id number " << qa.d_qid_num.getAttribute(QuantIdNumAttribute()) << " : " << q << std::endl;
        }
      }
    }
  }
}

bool QuantAttributes::isFunDef( Node q ) {
  std::map< Node, QAttributes >::iterator it = d_qattr.find( q );
  if( it==d_qattr.end() ){
    return false;
  }
  return it->second.isFunDef();
}

bool QuantAttributes::isSygus( Node q ) {
  std::map< Node, QAttributes >::iterator it = d_qattr.find( q );
  if( it==d_qattr.end() ){
    return false;
  }
  return it->second.d_sygus;
}

bool QuantAttributes::isOracleInterface(Node q)
{
  std::map<Node, QAttributes>::iterator it = d_qattr.find(q);
  if (it == d_qattr.end())
  {
    return false;
  }
  return it->second.isOracleInterface();
}

int64_t QuantAttributes::getQuantInstLevel(Node q)
{
  std::map< Node, QAttributes >::iterator it = d_qattr.find( q );
  if( it==d_qattr.end() ){
    return -1;
  }else{
    return it->second.d_qinstLevel;
  }
}

bool QuantAttributes::isQuantElim(Node q) const
{
  std::map<Node, QAttributes>::const_iterator it = d_qattr.find(q);
  if (it == d_qattr.end())
  {
    return false;
  }
  return it->second.d_quant_elim;
}
bool QuantAttributes::isQuantElimPartial(Node q) const
{
  std::map<Node, QAttributes>::const_iterator it = d_qattr.find(q);
  if( it==d_qattr.end() ){
    return false;
  }
  return it->second.d_quant_elim_partial;
}

bool QuantAttributes::isQuantBounded(Node q) const
{
  std::map<Node, QAttributes>::const_iterator it = d_qattr.find(q);
  if (it != d_qattr.end())
  {
    return it->second.d_isQuantBounded;
  }
  return false;
}

Node QuantAttributes::getQuantName(Node q) const
{
  std::map<Node, QAttributes>::const_iterator it = d_qattr.find(q);
  if (it != d_qattr.end())
  {
    return it->second.d_name;
  }
  return Node::null();
}

std::string QuantAttributes::quantToString(Node q) const
{
  std::stringstream ss;
  Node name = getQuantName(q);
  ss << (name.isNull() ? q : name);
  return ss.str();
}

int QuantAttributes::getQuantIdNum( Node q ) {
  std::map< Node, QAttributes >::iterator it = d_qattr.find( q );
  if( it!=d_qattr.end() ){
    if( !it->second.d_qid_num.isNull() ){
      return it->second.d_qid_num.getAttribute(QuantIdNumAttribute());
    }
  }
  return -1;
}

Node QuantAttributes::getQuantIdNumNode( Node q ) {
  std::map< Node, QAttributes >::iterator it = d_qattr.find( q );
  if( it==d_qattr.end() ){
    return Node::null();
  }else{
    return it->second.d_qid_num;
  }
}

Node QuantAttributes::mkAttrPreserveStructure(NodeManager* nm)
{
  Node nattr = mkAttrInternal(nm, AttrType::ATTR_PRESERVE_STRUCTURE);
  PreserveStructureAttribute psa;
  nattr[0].setAttribute(psa, true);
  return nattr;
}

Node QuantAttributes::mkAttrQuantifierElimination(NodeManager* nm)
{
  Node nattr = mkAttrInternal(nm, AttrType::ATTR_QUANT_ELIM);
  QuantElimAttribute qea;
  nattr[0].setAttribute(qea, true);
  return nattr;
}
Node QuantAttributes::mkAttrInternal(NodeManager* nm, AttrType at)
{
  SkolemManager* sm = nm->getSkolemManager();
  // use internal skolem id so that this method is deterministic
  Node id = nm->mkConstInt(Rational(static_cast<uint32_t>(at)));
  Node nattr = sm->mkInternalSkolemFunction(
      InternalSkolemId::QUANTIFIERS_ATTRIBUTE_INTERNAL,
      nm->booleanType(),
      {id});
  nattr = nm->mkNode(Kind::INST_ATTRIBUTE, nattr);
  return nattr;
}

void QuantAttributes::setInstantiationLevelAttr(Node n, uint64_t level)
{
  InstLevelAttribute ila;
  if (!n.hasAttribute(ila))
  {
    n.setAttribute(ila, level);
    Trace("inst-level-debug") << "Set instantiation level " << n << " to "
                              << level << std::endl;
    for (unsigned i = 0; i < n.getNumChildren(); i++)
    {
      setInstantiationLevelAttr(n[i], level);
    }
  }
}

bool QuantAttributes::getInstantiationLevel(const Node& n, uint64_t& level)
{
  InstLevelAttribute ila;
  if (n.hasAttribute(ila))
  {
    level = n.getAttribute(ila);
    return true;
  }
  return false;
}

Node mkNamedQuant(Kind k, Node bvl, Node body, const std::string& name)
{
  NodeManager* nm = bvl.getNodeManager();
  Node v = NodeManager::mkDummySkolem(
      name, nm->booleanType(), "", SkolemFlags::SKOLEM_EXACT_NAME);
  Node attr = nm->mkConst(String("qid"));
  Node ip = nm->mkNode(Kind::INST_ATTRIBUTE, attr, v);
  Node ipl = nm->mkNode(Kind::INST_PATTERN_LIST, ip);
  return nm->mkNode(k, bvl, body, ipl);
}

}  // namespace quantifiers
}  // namespace theory
}  // namespace cvc5::internal
