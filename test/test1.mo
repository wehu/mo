(define a "aa")
(next-tick 
  (lambda ()
    (display a)))
(export-mo 'a)
