(use-modules (ice-9 control))

(define _cur #f)

;; return current fiber
(define (current)
  _cur)

;; create a new fiber
(define (new body)
  (let ((o (make-hash-table)))
    (hashq-set! o 'pre _cur)
    (hashq-set! o 'cont (lambda ()
      (reset
        (body))))
    o))

;; yield current fiber and return to previous fiber
(define (yield f)
  (shift cont
    (set! _cur (hashq-ref f 'pre))
    (hashq-set! f 'cont cont)
    (if _cur
      ((hashq-ref _cur 'cont)))))

;; resume fiber
(define (resume f)
  (hashq-set! f 'pre _cur)
  (set! _cur f)
  ((hashq-ref f 'cont)))

(export-mo 'new)
(export-mo 'current)
(export-mo 'yield)
(export-mo 'resume)
