(require-mo "fiber" 'fiber)

(define f (fiber/new
  (lambda ()
    (display "1\n")
    (fiber/yield)
    (display "2\n")
    (fiber/yield)
    (display "3\n"))))

(fiber/resume f)
(fiber/resume f)
(fiber/resume f)

