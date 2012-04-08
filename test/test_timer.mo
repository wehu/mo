(set-timeout
  (lambda ()
    (display "hi3\n")) 1000)

(define id (set-interval
  (lambda ()
    (display "hi4\n")) 1000))

;(clear-timer id)

(gc)

(set-timeout (lambda () (clear-timer id)) 4000)
