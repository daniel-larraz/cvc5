(set-info :smt-lib-version 2.6)
(set-logic QF_SLIA)
(set-info :status sat)

(declare-fun x () String)
(declare-fun y () String)
(assert (= (str.indexof x y 1) (str.len x)))
(assert (str.contains x y))
(check-sat)
