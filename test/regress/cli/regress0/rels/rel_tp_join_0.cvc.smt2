; EXPECT: unsat
(set-option :incremental false)
(set-logic ALL)

(declare-fun x () (Set (Tuple Int Int)))
(declare-fun y () (Set (Tuple Int Int)))
(declare-fun r () (Set (Tuple Int Int)))
(declare-fun z () (Tuple Int Int))
(assert (= z (tuple 1 2)))
(declare-fun zt () (Tuple Int Int))
(assert (= zt (tuple 2 1)))
(declare-fun v () (Tuple Int Int))
(assert (= v (tuple 1 1)))
(declare-fun a () (Tuple Int Int))
(assert (= a (tuple 5 1)))
(declare-fun b () (Tuple Int Int))
(assert (= b (tuple 7 5)))
(assert (set.member (tuple 1 7) x))
(assert (set.member (tuple 2 3) x))
(assert (set.member (tuple 3 4) x))
(assert (set.member b y))
(assert (set.member (tuple 7 3) y))
(assert (set.member (tuple 4 7) y))
(assert (= r (rel.join x y)))
(assert (set.member z x))
(assert (set.member zt y))
(assert (not (set.member a (rel.transpose r))))
(check-sat)