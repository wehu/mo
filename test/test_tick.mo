(next-tick 
  (lambda () 
    (let ()
      (display "hi1\n")
      (next-tick
        (lambda ()
          (display "hi2\n"))))))

(gc)
