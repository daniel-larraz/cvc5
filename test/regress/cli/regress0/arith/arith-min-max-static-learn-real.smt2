; EXPECT: unsat
(set-logic ALL)
(declare-fun a () Real)
(define-fun b () Real 1.1)
(declare-fun d () Real)
(define-fun c () Real 2.1)
(declare-fun f () Real)
(define-fun e () Real 3.1)
(declare-fun g () Real)
(define-fun h () Real 4.1)
(assert (let ((x (ite (< a b) a b)) (y (ite (<= c d) c d)) (z (ite (> e f) e f)) (w (ite (>= g h) g h)))
  (or (> x a) (> x b) (> y c) (> y d) (< z e) (< z f) (< w g) (< w h))))
(check-sat)
