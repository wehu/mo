(use-modules (ice-9 control))

(define cur #f)

;; return current fiber
(define (current)
  cur)

;; create a new fiber
(define (new body)
  (let ((o (make-hash-table)))
    (hashq-set! o 'pre cur)
    (hashq-set! o 'alive #t)
    (hashq-set! o 'cont (lambda ()
      (reset
        (body)
        (hashq-set! o 'alive #f))))
    o))

;; yield current fiber and return to previous fiber
(define (yield)
  (shift cont
    (hashq-set! cur 'cont cont)
    (set! cur (hashq-ref cur 'pre))
    (if cur
      ((hashq-ref cur 'cont)))))

;; resume fiber
(define (resume f . args)
  (hashq-set! f 'pre cur)
  (set! cur f)
  (apply (hashq-ref f 'cont) args))

;; test alive
(define (alive? f)
  (hashq-ref f 'alive))

(export-mo 'new)
(export-mo 'current)
(export-mo 'yield)
(export-mo 'resume)
(export-mo 'alive?)
