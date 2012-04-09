(require-mo "scheduler" 's)

(define f1)
(define f2)

(set! f1 (s/spawn
  (lambda ()
    (display "1\n")
    (s/sleep)
    (display "2\n")
    (s/wake f2))))

(set! f2 (s/spawn
  (lambda ()
    (s/wake f1)
    (s/sleep)
    (display "3\n")
    (s/stop))))


(s/run)
