(set-info :smt-lib-version 2.6)
(set-logic QF_SLIA)

(set-info :status unsat)
(declare-fun x () String)
(declare-fun y () String)
(declare-fun z () String)
(assert (or (= x y)(= x z)))
(assert (str.in_re x (re.++ (str.to_re "A") (re.* (str.to_re "BAA")))))
(assert (str.in_re y (re.++ (str.to_re "AB") (re.* (str.to_re "AAB")) (str.to_re "A"))))
(assert (str.in_re z (re.++ (str.to_re "AB") (re.* (str.to_re "AAB")) (str.to_re "A"))))
; requires RE solver to reason modulo string equalties
(check-sat)
