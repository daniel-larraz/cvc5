(set-logic QF_SLIA)
(set-info :status unsat)
(declare-fun x () String)
(assert (str.in_re x (re.inter (re.comp (re.++ re.allchar (re.* re.allchar))) (re.++ (str.to_re "a") (re.* re.allchar)))))
(check-sat)