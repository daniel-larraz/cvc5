/******************************************************************************
 * Top contributors (to current version):
 *   Gereon Kremer, Andrew Reynolds, Aina Niemetz
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2025 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Unit tests for the coverings module for nonlinear arithmetic.
 */

#ifdef CVC5_USE_POLY

#include <poly/polyxx.h>

#include <iostream>
#include <memory>
#include <vector>

#include "test_smt.h"
#include "options/options_handler.h"
#include "options/proof_options.h"
#include "options/smt_options.h"
#include "smt/proof_manager.h"
#include "theory/arith/nl/coverings/cdcac.h"
#include "theory/arith/nl/coverings/lazard_evaluation.h"
#include "theory/arith/nl/coverings/projections.h"
#include "theory/arith/nl/coverings_solver.h"
#include "theory/arith/nl/nl_lemma_utils.h"
#include "theory/arith/nl/poly_conversion.h"
#include "theory/arith/theory_arith.h"
#include "theory/rewriter.h"
#include "theory/theory.h"
#include "theory/theory_engine.h"
#include "util/poly_util.h"

namespace cvc5::internal::test {

using namespace cvc5::internal;
using namespace cvc5::internal::theory;
using namespace cvc5::internal::theory::arith;
using namespace cvc5::internal::theory::arith::nl;

NodeManager* nodeManager;
class TestTheoryWhiteArithCoverings : public TestSmt
{
 protected:
  void SetUp() override
  {
    TestSmt::SetUp();
    d_realType.reset(new TypeNode(d_nodeManager->realType()));
    d_intType.reset(new TypeNode(d_nodeManager->integerType()));
    nodeManager = d_nodeManager.get();
  }

  void TearDown() override
  {
    d_dummyCache.clear();
    TestSmt::TearDown();
  }

  Node dummy(int i)
  {
    auto it = d_dummyCache.find(i);
    if (it == d_dummyCache.end())
    {
      it = d_dummyCache
               .emplace(i,
                        d_nodeManager->mkBoundVar("c" + std::to_string(i),
                                                  d_nodeManager->booleanType()))
               .first;
    }
    return it->second;
  }

  Theory::Effort d_level = Theory::EFFORT_FULL;
  std::unique_ptr<TypeNode> d_realType;
  std::unique_ptr<TypeNode> d_intType;
  const Rational d_zero = 0;
  const Rational d_one = 1;
  std::map<int, Node> d_dummyCache;
};

poly::AlgebraicNumber get_ran(std::initializer_list<long> init,
                              int lower,
                              int upper)
{
  return poly::AlgebraicNumber(poly::UPolynomial(init),
                               poly::DyadicInterval(lower, upper));
}

Node operator==(const Node& a, const Node& b)
{
  return nodeManager->mkNode(Kind::EQUAL, a, b);
}
Node operator>=(const Node& a, const Node& b)
{
  return nodeManager->mkNode(Kind::GEQ, a, b);
}
Node operator+(const Node& a, const Node& b)
{
  return nodeManager->mkNode(Kind::ADD, a, b);
}
Node operator*(const Node& a, const Node& b)
{
  return nodeManager->mkNode(Kind::NONLINEAR_MULT, a, b);
}
Node operator!(const Node& a) { return nodeManager->mkNode(Kind::NOT, a); }
Node make_real_variable(const std::string& s)
{
  SkolemManager* sm = nodeManager->getSkolemManager();
  return sm->mkDummySkolem(
      s, nodeManager->realType(), "", SkolemFlags::SKOLEM_EXACT_NAME);
}
Node make_int_variable(const std::string& s)
{
  SkolemManager* sm = nodeManager->getSkolemManager();
  return sm->mkDummySkolem(
      s, nodeManager->integerType(), "", SkolemFlags::SKOLEM_EXACT_NAME);
}

TEST_F(TestTheoryWhiteArithCoverings, test_univariate_isolation)
{
  poly::UPolynomial poly({-2, 2, 3, -3, -1, 1});
  auto roots = poly::isolate_real_roots(poly);

  EXPECT_TRUE(roots.size() == 4);
  EXPECT_TRUE(roots[0] == get_ran({-2, 0, 1}, -2, -1));
  EXPECT_TRUE(roots[1] == poly::Integer(-1));
  EXPECT_TRUE(roots[2] == poly::Integer(1));
  EXPECT_TRUE(roots[3] == get_ran({-2, 0, 1}, 1, 2));
}

TEST_F(TestTheoryWhiteArithCoverings, test_multivariate_isolation)
{
  poly::Context ctx;
  poly::Variable v_x(ctx, "x");
  poly::Variable v_y(ctx, "y");
  poly::Variable v_z(ctx, "z");
  poly::Polynomial x(ctx, v_x);
  poly::Polynomial y(ctx, v_y);
  poly::Polynomial z(ctx, v_z);

  poly::Assignment a(ctx);
  a.set(v_x, get_ran({-2, 0, 1}, 1, 2));
  a.set(v_y, get_ran({-2, 0, 0, 0, 1}, 1, 2));

  poly::Polynomial poly = (y * y + x) - z;

  auto roots = poly::isolate_real_roots(poly, a);

  EXPECT_TRUE(roots.size() == 1);
  EXPECT_TRUE(roots[0] == get_ran({-8, 0, 1}, 2, 3));
}

TEST_F(TestTheoryWhiteArithCoverings, test_univariate_factorization)
{
  poly::UPolynomial poly({-24, 44, -18, -1, 1, -3, 1});

  auto factors = square_free_factors(poly);
  std::sort(factors.begin(), factors.end());
  EXPECT_EQ(factors[0], poly::UPolynomial({-1, 1}));
  EXPECT_EQ(factors[1], poly::UPolynomial({-24, -4, -2, -1, 1}));
}

TEST_F(TestTheoryWhiteArithCoverings, test_projection)
{
  // Gereon's thesis, Ex 5.1
  poly::Context ctx;
  poly::Variable v_x(ctx, "x");
  poly::Variable v_y(ctx, "y");
  poly::Polynomial x(ctx, v_x);
  poly::Polynomial y(ctx, v_y);
  poly::Polynomial c1(ctx, 1);
  poly::Polynomial c2(ctx, 2);
  poly::Polynomial c3(ctx, 3);
  poly::Polynomial c5(ctx, 5);
  poly::Polynomial c7(ctx, 7);
  poly::Polynomial c14(ctx, 14);

  poly::Polynomial p = (y + c1) * (y + c1) - x * x * x + c3 * x - c2;
  poly::Polynomial q = (x + c1) * y - c3;

  auto res = coverings::projectionMcCallum({p, q});
  std::sort(res.begin(), res.end());
  EXPECT_EQ(res[0], x - c1);
  EXPECT_EQ(res[1], x + c1);
  EXPECT_EQ(res[2], x + c2);
  EXPECT_EQ(res[3], x * x * x - c3 * x + c1);
  EXPECT_EQ(res[4],
            x * x * x * x * x + c2 * x * x * x * x - c2 * x * x * x - c5 * x * x
                - c7 * x - c14);
}

TEST_F(TestTheoryWhiteArithCoverings, lazard_simp)
{
  Rewriter* rewriter = d_slvEngine->getEnv().getRewriter();
  Node a = d_nodeManager->mkVar(*d_realType);
  Node c = d_nodeManager->mkVar(*d_realType);
  Node orig = d_nodeManager->mkAnd(std::vector<Node>{
      d_nodeManager->mkNode(Kind::EQUAL, a, d_nodeManager->mkConstReal(d_zero)),
      d_nodeManager->mkNode(
          Kind::EQUAL,
          d_nodeManager->mkNode(
              Kind::ADD,
              d_nodeManager->mkNode(Kind::NONLINEAR_MULT, a, c),
              d_nodeManager->mkConstReal(d_one)),
          d_nodeManager->mkConstReal(d_zero))});

  {
    Node rewritten = rewriter->rewrite(orig);
    EXPECT_NE(rewritten, d_nodeManager->mkConst(false));
  }
  {
    Node rewritten = rewriter->extendedRewrite(orig);
    EXPECT_EQ(rewritten, d_nodeManager->mkConst(false));
  }
}

#ifdef CVC5_USE_COCOA
TEST_F(TestTheoryWhiteArithCoverings, lazard_eval)
{
  const poly::Context& ctx = d_nodeManager->getPolyContext();
  poly::Variable v_x(ctx, "x");
  poly::Variable v_y(ctx, "y");
  poly::Variable v_z(ctx, "z");
  poly::Variable v_f(ctx, "f");
  poly::Polynomial x(ctx, v_x);
  poly::Polynomial y(ctx, v_y);
  poly::Polynomial z(ctx, v_z);
  poly::Polynomial f(ctx, v_f);
  poly::AlgebraicNumber ax = get_ran({-2, 0, 1}, 1, 2);
  poly::AlgebraicNumber ay = get_ran({-2, 0, 0, 0, 1}, 1, 2);
  poly::AlgebraicNumber az = get_ran({-3, 0, 1}, 1, 2);

  Options opts;
  Env env(d_nodeManager.get(), &opts);
  coverings::LazardEvaluation lazard(env.getStatisticsRegistry(), ctx);
  lazard.add(v_x, ax);
  lazard.add(v_y, ay);
  lazard.add(v_z, az);

  poly::Polynomial c2(ctx, 2);
  poly::Polynomial q = (x * x - c2) * (y * y * y * y - c2) * z * f;
  lazard.addFreeVariable(v_f);
  auto qred = lazard.reducePolynomial(q);
  EXPECT_EQ(qred, std::vector<poly::Polynomial>{f});
}
#endif

TEST_F(TestTheoryWhiteArithCoverings, test_cdcac_1)
{
  Options opts;
  Env env(d_nodeManager.get(), &opts);
  const poly::Context& ctx = d_nodeManager->getPolyContext();
  coverings::CDCAC cac(env, {});
  poly::Variable v_x =
      cac.getConstraints().varMapper()(make_real_variable("x"));
  poly::Variable v_y =
      cac.getConstraints().varMapper()(make_real_variable("y"));
  poly::Polynomial x(ctx, v_x);
  poly::Polynomial y(ctx, v_y);
  poly::Polynomial c1(ctx, 1);
  poly::Polynomial c2(ctx, 2);
  poly::Polynomial c4(ctx, 4);

  cac.getConstraints().addConstraint(
      c4 * y - x * x + c4, poly::SignCondition::LT, dummy(1));
  cac.getConstraints().addConstraint(
      c4 * y - c4 + (x - c1) * (x - c1), poly::SignCondition::GT, dummy(2));
  cac.getConstraints().addConstraint(
      c4 * y - x - c2, poly::SignCondition::GT, dummy(3));

  cac.computeVariableOrdering();

  auto cover = cac.getUnsatCover();
  EXPECT_TRUE(cover.empty());
  std::cout << "SAT: " << cac.getModel() << std::endl;
}

TEST_F(TestTheoryWhiteArithCoverings, test_cdcac_2)
{
  Options opts;
  Env env(d_nodeManager.get(), &opts);
  const poly::Context& ctx = d_nodeManager->getPolyContext();
  coverings::CDCAC cac(env, {});
  poly::Variable v_x =
      cac.getConstraints().varMapper()(make_real_variable("x"));
  poly::Variable v_y =
      cac.getConstraints().varMapper()(make_real_variable("y"));
  poly::Polynomial x(ctx, v_x);
  poly::Polynomial y(ctx, v_y);
  poly::Polynomial c1(ctx, 1);
  poly::Polynomial c2(ctx, 2);
  poly::Polynomial c3(ctx, 3);

  cac.getConstraints().addConstraint(
      y - pow(-x - c3, 11) + pow(-x - c3, 10) + c1,
      poly::SignCondition::GT,
      dummy(1));
  cac.getConstraints().addConstraint(
      c2 * y - x + c2, poly::SignCondition::LT, dummy(2));
  cac.getConstraints().addConstraint(
      c2 * y - c1 + x * x, poly::SignCondition::GT, dummy(3));
  cac.getConstraints().addConstraint(
      c3 * y + x + c2, poly::SignCondition::LT, dummy(4));
  cac.getConstraints().addConstraint(
      y * y * y - pow(x - c2, 11) + pow(x - c2, 10) + c1,
      poly::SignCondition::GT,
      dummy(5));

  cac.computeVariableOrdering();

  auto cover = cac.getUnsatCover();
  EXPECT_TRUE(!cover.empty());
  auto nodes = coverings::collectConstraints(cover);
  std::vector<Node> ref{dummy(1), dummy(2), dummy(3), dummy(4), dummy(5)};
  std::sort(nodes.begin(), nodes.end());
  std::sort(ref.begin(), ref.end());
  EXPECT_EQ(nodes, ref);
}

TEST_F(TestTheoryWhiteArithCoverings, test_cdcac_3)
{
  Options opts;
  Env env(d_nodeManager.get(), &opts);
  const poly::Context& ctx = d_nodeManager->getPolyContext();
  coverings::CDCAC cac(env, {});
  poly::Variable v_x =
      cac.getConstraints().varMapper()(make_real_variable("x"));
  poly::Variable v_y =
      cac.getConstraints().varMapper()(make_real_variable("y"));
  poly::Variable v_z =
      cac.getConstraints().varMapper()(make_real_variable("z"));
  poly::Polynomial x(ctx, v_x);
  poly::Polynomial y(ctx, v_y);
  poly::Polynomial z(ctx, v_z);
  poly::Polynomial c1(ctx, 1);
  poly::Polynomial c2(ctx, 2);
  poly::Polynomial c3(ctx, 3);
  poly::Polynomial c4(ctx, 4);

  cac.getConstraints().addConstraint(
      x * x + y * y + z * z - c1, poly::SignCondition::LT, dummy(1));
  cac.getConstraints().addConstraint(
      c4 * x * x + (c2 * y - c3) * (c2 * y - c3) + c4 * z * z - c4,
      poly::SignCondition::LT,
      dummy(2));

  cac.computeVariableOrdering();

  auto cover = cac.getUnsatCover();
  EXPECT_TRUE(cover.empty());
  std::cout << "SAT: " << cac.getModel() << std::endl;
}

TEST_F(TestTheoryWhiteArithCoverings, test_cdcac_4)
{
  Options opts;
  Env env(d_nodeManager.get(), &opts);
  const poly::Context& ctx = d_nodeManager->getPolyContext();
  coverings::CDCAC cac(env, {});
  poly::Variable v_x =
      cac.getConstraints().varMapper()(make_real_variable("x"));
  poly::Variable v_y =
      cac.getConstraints().varMapper()(make_real_variable("y"));
  poly::Variable v_z =
      cac.getConstraints().varMapper()(make_real_variable("z"));
  poly::Polynomial x(ctx, v_x);
  poly::Polynomial y(ctx, v_y);
  poly::Polynomial z(ctx, v_z);
  poly::Polynomial c1(ctx, 1);
  poly::Polynomial c6(ctx, 6);
  poly::Polynomial c9(ctx, 9);
  poly::Polynomial c25(ctx, 25);
  poly::Polynomial c100(ctx, 100);

  cac.getConstraints().addConstraint(
      -z * z + y * y + x * x - c25, poly::SignCondition::GT, dummy(1));
  cac.getConstraints().addConstraint(
      (y - x - c6) * z * z - c9 * y * y + x * x - c1,
      poly::SignCondition::GT,
      dummy(2));
  cac.getConstraints().addConstraint(
      y * y - c100, poly::SignCondition::LT, dummy(3));

  cac.computeVariableOrdering();

  auto cover = cac.getUnsatCover();
  EXPECT_TRUE(cover.empty());
  std::cout << "SAT: " << cac.getModel() << std::endl;
}

void test_delta(NodeManager* nm, const std::vector<Node>& a)
{
  Options opts;
  Env env(nm, &opts);
  coverings::CDCAC cac(env, {});
  cac.reset();
  for (const Node& n : a)
  {
    cac.getConstraints().addConstraint(n);
  }
  cac.computeVariableOrdering();

  // Do full theory check here

  auto covering = cac.getUnsatCover();
  if (covering.empty())
  {
    std::cout << "SAT: " << cac.getModel() << std::endl;
  }
  else
  {
    auto mis = coverings::collectConstraints(covering);
    std::cout << "Collected MIS: " << mis << std::endl;
    Assert(!mis.empty()) << "Infeasible subset can not be empty";
    Node lem = nm->mkAnd(mis).negate();
    std::cout << "UNSAT with MIS: " << lem << std::endl;
  }
}

TEST_F(TestTheoryWhiteArithCoverings, test_cdcac_proof_1)
{
  Options opts;
  // enable proofs
  opts.write_smt().proofMode = options::ProofMode::FULL;
  opts.write_smt().produceProofs = true;
  Env env(d_nodeManager.get(), &opts);
  const poly::Context& ctx = d_nodeManager->getPolyContext();
  smt::PfManager pfm(env);
  env.finishInit(&pfm);
  EXPECT_TRUE(env.isTheoryProofProducing());
  // register checkers that we need
  NodeManager * nm = env.getNodeManager();
  builtin::BuiltinProofRuleChecker btchecker(nm, env.getRewriter(), env);
  btchecker.registerTo(env.getProofNodeManager()->getChecker());
  coverings::CoveringsProofRuleChecker checker(nm);
  checker.registerTo(env.getProofNodeManager()->getChecker());
  // do the coverings problem
  coverings::CDCAC cac(env, {});
  EXPECT_TRUE(cac.getProof() != nullptr);
  cac.startNewProof();
  poly::Variable v_x =
      cac.getConstraints().varMapper()(make_real_variable("x"));
  poly::Variable v_y =
      cac.getConstraints().varMapper()(make_real_variable("y"));
  poly::Polynomial x(ctx, v_x);
  poly::Polynomial y(ctx, v_y);
  poly::Polynomial c1(ctx, 1);
  poly::Polynomial c2(ctx, 2);
  poly::Polynomial c3(ctx, 3);
  poly::Polynomial c4(ctx, 4);
  poly::Polynomial c5(ctx, 5);

  cac.getConstraints().addConstraint(
      c4 * y - x * x + c4, poly::SignCondition::LT, dummy(1));
  cac.getConstraints().addConstraint(
      c3 * y - c5 + (x - c2) * (x - c2), poly::SignCondition::GT, dummy(2));
  cac.getConstraints().addConstraint(
      c4 * y - x - c2, poly::SignCondition::GT, dummy(3));
  cac.getConstraints().addConstraint(x + c1, poly::SignCondition::GT, dummy(4));
  cac.getConstraints().addConstraint(x - c2, poly::SignCondition::LT, dummy(5));

  cac.computeVariableOrdering();

  auto cover = cac.getUnsatCover();
  EXPECT_FALSE(cover.empty());
  
  std::vector<Node> mis{dummy(1), dummy(3), dummy(4), dummy(5)};
  LazyTreeProofGenerator* pg = dynamic_cast<LazyTreeProofGenerator*>(cac.closeProof(mis));
  EXPECT_TRUE(pg != nullptr);
}

TEST_F(TestTheoryWhiteArithCoverings, test_delta_one)
{
  std::vector<Node> a;
  Node zero = d_nodeManager->mkConstReal(Rational(0));
  Node one = d_nodeManager->mkConstReal(Rational(1));
  Node mone = d_nodeManager->mkConstReal(Rational(-1));
  Node fifth = d_nodeManager->mkConstReal(Rational(1, 2));
  Node g = make_real_variable("g");
  Node l = make_real_variable("l");
  Node q = make_real_variable("q");
  Node s = make_real_variable("s");
  Node u = make_real_variable("u");

  a.emplace_back(l == mone);
  a.emplace_back(!(g * s == zero));
  a.emplace_back(q * s == zero);
  a.emplace_back(u == zero);
  a.emplace_back(q == (one + (fifth * g * s)));
  a.emplace_back(l == u + q * s + (fifth * g * s * s));

  test_delta(d_nodeManager.get(), a);
}

TEST_F(TestTheoryWhiteArithCoverings, test_delta_two)
{
  std::vector<Node> a;
  Node zero = d_nodeManager->mkConstReal(Rational(0));
  Node one = d_nodeManager->mkConstReal(Rational(1));
  Node mone = d_nodeManager->mkConstReal(Rational(-1));
  Node fifth = d_nodeManager->mkConstReal(Rational(1, 2));
  Node g = make_real_variable("g");
  Node l = make_real_variable("l");
  Node q = make_real_variable("q");
  Node s = make_real_variable("s");
  Node u = make_real_variable("u");

  a.emplace_back(l == mone);
  a.emplace_back(!(g * s == zero));
  a.emplace_back(u == zero);
  a.emplace_back(q * s == zero);
  a.emplace_back(q == (one + (fifth * g * s)));
  a.emplace_back(l == u + q * s + (fifth * g * s * s));

  test_delta(d_nodeManager.get(), a);
}

TEST_F(TestTheoryWhiteArithCoverings, test_ran_conversion)
{
  RealAlgebraicNumber ran(
      std::vector<Rational>({-2, 0, 1}), Rational(1, 3), Rational(7, 3));
  {
    Node x = nodeManager->mkBoundVar("x", nodeManager->realType());
    Node n = PolyConverter::ran_to_node(ran, x);
    RealAlgebraicNumber back = PolyConverter::node_to_ran(n, x);
    EXPECT_TRUE(ran == back);
  }
}
}  // namespace cvc5::internal::test

#endif
