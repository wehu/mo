(require-mo "scheduler" 's)

(define f1)
(define f2)

(define f1 (s/spawn
  (lambda ()
    (display "1\n")
    (s/sleep f1)
    (display "2\n")
    (s/stop))))

(set! f2 (s/spawn
  (lambda ()
    (s/wake f1))))

(s/run)
