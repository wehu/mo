(require-mo "scheduler" 's)

(define f1)
(define f2)

(set! f2 (s/spawn
  (lambda ()
    (set! f1 (s/spawn
      (lambda ()
        (display "1\n")
        (s/wake f2 1 2)
        (s/sleep)
        (display "2\n")
        (s/wake f2))))
    (s/sleep)
    (s/wake f1)
    (s/sleep)
    (display "3\n"))))

(s/spawn
  (lambda ()
    (s/join f1 f2)
    (s/stop)))

(s/run)
