; EXPECT: unsat
(set-logic ALL)
(declare-sort N 0)
(declare-sort D 0)
(declare-sort N_ 0)
(declare-sort _l 0)
(declare-sort d 0)
(declare-datatypes ((T 0) (l 0) (T_ 0) (i 0) (_d 0) (N_l 0)) (((r (r D))) ((i) (o ($ N) (l2 l))) ((ni)) ((l3)) ((ni)) ((ni))))
(declare-fun f () N_)
(declare-fun n () N)
(declare-fun c (D) d)
(declare-fun p (N_) _l)
(declare-fun m (T d) Bool)
(declare-fun u (_l l) Bool)
(declare-fun u (N_ N) D)
(assert (not (m (r (u f n)) (c (u f n)))))
(assert (u (p f) (o n (o n i))))
(assert (forall ((?v l)) (= (u (p f) ?v) (or (exists ((? N) (v N)) (and (= ?v (o v (o ? i))) (m (r (u f ?)) (c (u f v)))))))))
(check-sat)