(require-mo "scheduler" 's)

(define f2 (s/spawn
  (lambda ()
    (display "1\n")
    (s/sleep f2)
    (display "2\n"))))

(s/run)
