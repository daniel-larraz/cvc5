/******************************************************************************
 * Top contributors (to current version):
 *   Andrew Reynolds, Hans-Joerg Schurr, Aina Niemetz
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2025 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Implementation of proof.
 */

#include "proof/proof.h"

#include "proof/proof_checker.h"
#include "proof/proof_node.h"
#include "proof/proof_node_manager.h"
#include "rewriter/rewrites.h"
#include "smt/env.h"

using namespace cvc5::internal::kind;

namespace cvc5::internal {

CDProof::CDProof(Env& env,
                 context::Context* c,
                 const std::string& name,
                 bool autoSymm)
    : EnvObj(env),
      d_context(),
      d_nodes(c ? c : &d_context),
      d_name(name),
      d_autoSymm(autoSymm)
{
}

CDProof::~CDProof() {}

std::shared_ptr<ProofNode> CDProof::getProofFor(Node fact)
{
  std::shared_ptr<ProofNode> pf = getProofSymm(fact);
  if (pf != nullptr)
  {
    return pf;
  }
  // add as assumption
  std::vector<Node> pargs = {fact};
  std::vector<std::shared_ptr<ProofNode>> passume;
  ProofNodeManager* pnm = getManager();
  std::shared_ptr<ProofNode> pfa =
      pnm->mkNode(ProofRule::ASSUME, passume, pargs, fact);
  d_nodes.insert(fact, pfa);
  return pfa;
}

std::shared_ptr<ProofNode> CDProof::getProof(Node fact) const
{
  NodeProofNodeMap::iterator it = d_nodes.find(fact);
  if (it != d_nodes.end())
  {
    return (*it).second;
  }
  return nullptr;
}

std::shared_ptr<ProofNode> CDProof::getProofSymm(Node fact)
{
  Trace("cdproof") << "CDProof::getProofSymm: " << fact << std::endl;
  std::shared_ptr<ProofNode> pf = getProof(fact);
  if (pf != nullptr && !isAssumption(pf.get()))
  {
    Trace("cdproof") << "...existing non-assume " << pf->getRule() << std::endl;
    return pf;
  }
  else if (!d_autoSymm)
  {
    Trace("cdproof") << "...not auto considering symmetry" << std::endl;
    return pf;
  }
  Node symFact = getSymmFact(fact);
  if (symFact.isNull())
  {
    Trace("cdproof") << "...no possible symm" << std::endl;
    // no symmetry possible, return original proof (possibly assumption)
    return pf;
  }
  // See if a proof exists for the opposite direction, if so, add the step.
  // Notice that SYMM is also disallowed.
  std::shared_ptr<ProofNode> pfs = getProof(symFact);
  if (pfs != nullptr)
  {
    // The symmetric fact exists, and the current one either does not, or is
    // an assumption. We make a new proof that applies SYMM to pfs.
    std::vector<std::shared_ptr<ProofNode>> pschild;
    pschild.push_back(pfs);
    std::vector<Node> args;
    ProofNodeManager* pnm = getManager();
    if (pf == nullptr)
    {
      Trace("cdproof") << "...fresh make symm" << std::endl;
      std::shared_ptr<ProofNode> psym = pnm->mkSymm(pfs, fact);
      Assert(psym != nullptr);
      d_nodes.insert(fact, psym);
      return psym;
    }
    else if (!isAssumption(pfs.get()))
    {
      // if its not an assumption, make the connection
      Trace("cdproof") << "...update symm" << std::endl;
      // update pf
      bool sret = pnm->updateNode(pf.get(), ProofRule::SYMM, pschild, args);
      AlwaysAssert(sret);
    }
  }
  else
  {
    Trace("cdproof") << "...no symm, return "
                     << (pf == nullptr ? "null" : "non-null") << std::endl;
  }
  // return original proof (possibly assumption)
  return pf;
}

bool CDProof::addStep(Node expected,
                      ProofRule id,
                      const std::vector<Node>& children,
                      const std::vector<Node>& args,
                      bool ensureChildren,
                      CDPOverwrite opolicy)
{
  Trace("cdproof") << "CDProof::addStep: " << identify() << " : " << id << " "
                   << expected << ", ensureChildren = " << ensureChildren
                   << ", overwrite policy = " << opolicy << std::endl;
  Trace("cdproof-debug") << "CDProof::addStep: " << identify()
                         << " : children: " << children << "\n";
  Trace("cdproof-debug") << "CDProof::addStep: " << identify()
                         << " : args: " << args << "\n";
  // We must always provide expected to this method
  Assert(!expected.isNull());

  std::shared_ptr<ProofNode> pprev = getProofSymm(expected);
  if (pprev != nullptr)
  {
    if (!shouldOverwrite(pprev.get(), id, opolicy))
    {
      // we should not overwrite the current step
      Trace("cdproof") << "...success, no overwrite" << std::endl;
      return true;
    }
    Trace("cdproof") << "existing proof " << pprev->getRule()
                     << ", overwrite..." << std::endl;
    // we will overwrite the existing proof node by updating its contents below
  }
  // collect the child proofs, for each premise
  ProofNodeManager* pnm = getManager();
  std::vector<std::shared_ptr<ProofNode>> pchildren;
  for (const Node& c : children)
  {
    Trace("cdproof") << "- get child " << c << std::endl;
    std::shared_ptr<ProofNode> pc = getProofSymm(c);
    if (pc == nullptr)
    {
      if (ensureChildren)
      {
        // failed to get a proof for a child, fail
        Trace("cdproof") << "...fail, no child" << std::endl;
        return false;
      }
      Trace("cdproof") << "--- add assume" << std::endl;
      // otherwise, we initialize it as an assumption
      std::vector<Node> pcargs = {c};
      std::vector<std::shared_ptr<ProofNode>> pcassume;
      pc = pnm->mkNode(ProofRule::ASSUME, pcassume, pcargs, c);
      // assumptions never fail to check
      Assert(pc != nullptr);
      d_nodes.insert(c, pc);
    }
    pchildren.push_back(pc);
  }

  // The user may have provided SYMM of an assumption. This block is only
  // necessary if d_autoSymm is enabled.
  if (d_autoSymm && id == ProofRule::SYMM)
  {
    Assert(pchildren.size() == 1);
    if (isAssumption(pchildren[0].get()))
    {
      // the step we are constructing is a (symmetric fact of an) assumption, so
      // there is no use adding it to the proof.
      return true;
    }
  }

  bool ret = true;
  // create or update it
  std::shared_ptr<ProofNode> pthis;
  if (pprev == nullptr)
  {
    Trace("cdproof") << "  new node " << expected << "..." << std::endl;
    pthis = pnm->mkNode(id, pchildren, args, expected);
    if (pthis == nullptr)
    {
      // failed to construct the node, perhaps due to a proof checking failure
      Trace("cdproof") << "...fail, proof checking" << std::endl;
      return false;
    }
    d_nodes.insert(expected, pthis);
  }
  else
  {
    Trace("cdproof") << "  update node " << expected << "..." << std::endl;
    // update its value
    pthis = pprev;
    // We return the value of updateNode here. This means this method may return
    // false if this call failed, regardless of whether we already have a proof
    // step for expected.
    ret = pnm->updateNode(pthis.get(), id, pchildren, args);
  }
  if (ret)
  {
    // the result of the proof node should be expected
    Assert(pthis->getResult() == expected);

    // notify new proof
    notifyNewProof(expected);
  }

  Trace("cdproof") << "...return " << ret << std::endl;
  return ret;
}

void CDProof::notifyNewProof(Node expected)
{
  if (!d_autoSymm)
  {
    return;
  }
  // ensure SYMM proof is also linked to an existing proof, if it is an
  // assumption.
  Node symExpected = getSymmFact(expected);
  if (!symExpected.isNull())
  {
    Trace("cdproof") << "  check connect symmetry " << symExpected << std::endl;
    // if it exists, we may need to update it
    std::shared_ptr<ProofNode> pfs = getProof(symExpected);
    if (pfs != nullptr)
    {
      Trace("cdproof") << "  connect via getProofSymm method..." << std::endl;
      // call the get function with symmetry, which will do the update
      std::shared_ptr<ProofNode> pfss = getProofSymm(symExpected);
    }
    else
    {
      Trace("cdproof") << "  no connect" << std::endl;
    }
  }
}

bool CDProof::addTrustedStep(Node expected,
                             TrustId id,
                             const std::vector<Node>& children,
                             const std::vector<Node>& args,
                             bool ensureChildren,
                             CDPOverwrite opolicy)
{
  std::vector<Node> sargs;
  sargs.push_back(mkTrustId(nodeManager(), id));
  sargs.push_back(expected);
  sargs.insert(sargs.end(), args.begin(), args.end());
  return addStep(
      expected, ProofRule::TRUST, children, sargs, ensureChildren, opolicy);
}

bool CDProof::addTheoryRewriteStep(Node expected,
                                   ProofRewriteRule id,
                                   bool ensureChildren,
                                   CDPOverwrite opolicy)
{
  if (expected.getKind() != Kind::EQUAL)
  {
    return false;
  }
  std::vector<Node> sargs;
  sargs.push_back(rewriter::mkRewriteRuleNode(nodeManager(), id));
  sargs.push_back(expected);
  return addStep(
      expected, ProofRule::THEORY_REWRITE, {}, sargs, ensureChildren, opolicy);
}

bool CDProof::addStep(Node expected,
                      const ProofStep& step,
                      bool ensureChildren,
                      CDPOverwrite opolicy)
{
  return addStep(expected,
                 step.d_rule,
                 step.d_children,
                 step.d_args,
                 ensureChildren,
                 opolicy);
}

bool CDProof::addSteps(const ProofStepBuffer& psb,
                       bool ensureChildren,
                       CDPOverwrite opolicy)
{
  const std::vector<std::pair<Node, ProofStep>>& steps = psb.getSteps();
  for (const std::pair<Node, ProofStep>& ps : steps)
  {
    if (!addStep(ps.first, ps.second, ensureChildren, opolicy))
    {
      return false;
    }
  }
  return true;
}

bool CDProof::addProof(std::shared_ptr<ProofNode> pn,
                       CDPOverwrite opolicy,
                       bool doCopy)
{
  if (!doCopy)
  {
    // If we are automatically managing symmetry, we strip off SYMM steps.
    // This avoids cyclic proofs in cases where P and (SYMM P) are added as
    // proofs to the same CDProof.
    if (d_autoSymm)
    {
      std::vector<std::shared_ptr<ProofNode>> processed;
      while (pn->getRule() == ProofRule::SYMM)
      {
        pn = pn->getChildren()[0];
        if (std::find(processed.begin(), processed.end(), pn)
            != processed.end())
        {
          Unreachable() << "Cyclic proof encountered when cancelling symmetry "
                           "steps during addProof";
        }
        processed.push_back(pn);
      }
    }
    // If we aren't doing a deep copy, we either store pn or link its top
    // node into the existing pointer
    Node curFact = pn->getResult();
    std::shared_ptr<ProofNode> cur = getProofSymm(curFact);
    if (cur == nullptr)
    {
      // Assert that the checker of this class agrees with (the externally
      // provided) pn. This ensures that if pn was checked by a different
      // checker than the one of the manager in this class, then it is double
      // checked here, so that this class maintains the invariant that all of
      // its nodes in d_nodes have been checked by the underlying checker.
      Assert(getManager()->getChecker() == nullptr
             || getManager()->getChecker()->check(pn.get(), curFact)
                    == curFact);
      // just store the proof for fact
      d_nodes.insert(curFact, pn);
    }
    else if (shouldOverwrite(cur.get(), pn->getRule(), opolicy))
    {
      // We update cur to have the structure of the top node of pn. Notice that
      // the interface to update this node will ensure that the proof apf is a
      // proof of the assumption. If it does not, then pn was wrong.
      if (!getManager()->updateNode(
              cur.get(), pn->getRule(), pn->getChildren(), pn->getArguments()))
      {
        return false;
      }
    }
    // also need to connect via SYMM if necessary
    notifyNewProof(curFact);
    return true;
  }
  std::unordered_map<ProofNode*, bool> visited;
  std::unordered_map<ProofNode*, bool>::iterator it;
  std::vector<ProofNode*> visit;
  ProofNode* cur;
  Node curFact;
  visit.push_back(pn.get());
  bool retValue = true;
  do
  {
    cur = visit.back();
    curFact = cur->getResult();
    visit.pop_back();
    it = visited.find(cur);
    if (it == visited.end())
    {
      // visit the children
      visited[cur] = false;
      visit.push_back(cur);
      const std::vector<std::shared_ptr<ProofNode>>& cs = cur->getChildren();
      for (const std::shared_ptr<ProofNode>& c : cs)
      {
        visit.push_back(c.get());
      }
    }
    else if (!it->second)
    {
      // we always call addStep, which may or may not overwrite the
      // current step
      std::vector<Node> pexp;
      const std::vector<std::shared_ptr<ProofNode>>& cs = cur->getChildren();
      for (const std::shared_ptr<ProofNode>& c : cs)
      {
        Assert(!c->getResult().isNull());
        pexp.push_back(c->getResult());
      }
      // can ensure children at this point
      bool res = addStep(
          curFact, cur->getRule(), pexp, cur->getArguments(), true, opolicy);
      // should always succeed
      Assert(res);
      retValue = retValue && res;
      visited[cur] = true;
    }
  } while (!visit.empty());

  return retValue;
}

bool CDProof::hasStep(Node fact)
{
  std::shared_ptr<ProofNode> pf = getProof(fact);
  if (pf != nullptr && !isAssumption(pf.get()))
  {
    return true;
  }
  else if (!d_autoSymm)
  {
    return false;
  }
  Node symFact = getSymmFact(fact);
  if (symFact.isNull())
  {
    return false;
  }
  pf = getProof(symFact);
  if (pf != nullptr && !isAssumption(pf.get()))
  {
    return true;
  }
  return false;
}

size_t CDProof::getNumProofNodes() const { return d_nodes.size(); }

ProofNodeManager* CDProof::getManager() const
{
  ProofNodeManager* pnm = d_env.getProofNodeManager();
  Assert(pnm != nullptr);
  return pnm;
}

bool CDProof::shouldOverwrite(ProofNode* pn, ProofRule newId, CDPOverwrite opol)
{
  Assert(pn != nullptr);
  // we overwrite only if opol is CDPOverwrite::ALWAYS, or if
  // opol is CDPOverwrite::ASSUME_ONLY and the previously
  // provided proof pn was an assumption and the currently provided step is not
  return opol == CDPOverwrite::ALWAYS
         || (opol == CDPOverwrite::ASSUME_ONLY && isAssumption(pn)
             && newId != ProofRule::ASSUME);
}

bool CDProof::isAssumption(ProofNode* pn)
{
  ProofRule rule = pn->getRule();
  if (rule == ProofRule::ASSUME)
  {
    return true;
  }
  else if (rule != ProofRule::SYMM)
  {
    return false;
  }
  pn = ProofNodeManager::cancelDoubleSymm(pn);
  rule = pn->getRule();
  if (rule == ProofRule::ASSUME)
  {
    return true;
  }
  else if (rule != ProofRule::SYMM)
  {
    return false;
  }
  const std::vector<std::shared_ptr<ProofNode>>& pc = pn->getChildren();
  Assert(pc.size() == 1);
  return pc[0]->getRule() == ProofRule::ASSUME;
}

bool CDProof::isSame(TNode f, TNode g)
{
  if (f == g)
  {
    return true;
  }
  Kind fk = f.getKind();
  Kind gk = g.getKind();
  if (fk == Kind::EQUAL && gk == Kind::EQUAL && f[0] == g[1] && f[1] == g[0])
  {
    // symmetric equality
    return true;
  }
  if (fk == Kind::NOT && gk == Kind::NOT && f[0].getKind() == Kind::EQUAL
      && g[0].getKind() == Kind::EQUAL && f[0][0] == g[0][1]
      && f[0][1] == g[0][0])
  {
    // symmetric disequality
    return true;
  }
  return false;
}

Node CDProof::getSymmFact(TNode f)
{
  bool polarity = f.getKind() != Kind::NOT;
  TNode fatom = polarity ? f : f[0];
  if (fatom.getKind() != Kind::EQUAL || fatom[0] == fatom[1])
  {
    return Node::null();
  }
  Node symFact = fatom[1].eqNode(fatom[0]);
  return polarity ? symFact : symFact.notNode();
}

std::string CDProof::identify() const { return d_name; }

}  // namespace cvc5::internal
