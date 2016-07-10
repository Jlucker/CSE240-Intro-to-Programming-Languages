; Programmed by: Justin Lucker
; Date: 04/09/2014
; Homework # 7
; Program: Fibonacci Sequence
; Description: Displays the specified fibonacci number
;              Example: The 9th fibonacci number in the sequence is 34

(define(fib n)
  (cond 
   (( = n 0 )0)
   (( = n 1 )1) 
   (else
      (+(fib(- n 1))(fib(- n 2))))))         


; Programmed by: Justin Lucker
; Date: 04/09/2014
; Homework # 7
; Program: fib-sum
; Description: Calculates the sum of all even fibonacci numbers
;              that are less than 1000000

( define fib1 0 )
( define fib2 1 )                                   
( define sum  0 )                                   

(define(addEven n)
   ( set! sum ( + sum n)))

( define (fib-sum n)
   (cond
     (( > n 1000000 ) display sum)
     ((even? n)(addEven n)))
   (cond
     (( < n 1000000 )
      (set! fib1(+ fib2 0))
      (set! fib2(+ fib2 n))
      (fib-sum fib1))
     (else
      (display sum))))


   
         
                

   
   


                  
               
                   
        