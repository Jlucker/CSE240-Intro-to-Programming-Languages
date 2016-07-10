; Programmed by: Justin Lucker
; Date: 04/19/2014
; Homework # 8
; Program: List Manipulation

(define (list-push-front lst new-list)
  (append new-list lst))


(define (list-push-back lst new-list)
  (append lst new-list))


(define (list-draw-front n lst)
  (if (zero? n)
      '()
      (append (list(car lst)) (list-draw-front (- n 1) (cdr lst)))))
      

(define (list-draw-back n lst)
  (cond
    ((< n (length lst)) (list-draw-back n (cdr lst)))
    ((= n (length lst)) list-draw-front n lst)))
 

(define (list-shuffle lst1 lst2)
  
  (if (null? lst1)
      '()
     (list-push-back (list-push-back (list-draw-front 1 lst1) (list-draw-front 1 lst2)) (list-shuffle (cdr lst1) (cdr lst2)))))


; Sample output calls:
(list-push-front '(4 3 7 1 2 9) '( 1 2))
(list-push-back '(4 3 7 1 2 9) '( 1 2))
(list-draw-front 3 '(4 3 7 1 2 9))
(list-draw-back 3 '(4 3 7 1 2 9))
(list-shuffle '(1 2) '(a b))