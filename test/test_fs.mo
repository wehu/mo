(fs-open "test.log" 0 0
  (lambda (fd err)
    (display (fd))))
