(set-logic ALL)
(set-info :status sat)

 
(declare-fun s () String)
(assert (= s "I like cookies."))
 
(declare-fun target () String)
(assert (= target "like"))
 
(declare-fun location () Int)
(assert (= location (str.indexof s target 0)))
 
(check-sat)