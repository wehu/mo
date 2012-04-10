(require-mo "fiber" 'fiber)
(use-modules (srfi srfi-1))

;; sleeping queue
(define sleeping-q (make-hash-table))

;; running queue
(define running-q (make-hash-table))

;; stop flag
(define stopped #f)

;; spawn a new fiber into running queue
(define (spawn body)
  (let ((f (fiber/new body)))
    (hashq-set! running-q f f)
    (hashq-set! f 'on `())
    f))

;; sleep current fiber
(define (sleep)
  (hashq-remove! running-q (fiber/current))
  (hashq-set! sleeping-q (fiber/current) (fiber/current))
  (fiber/yield))

;; wake up a fiber
(define (wake f)
  (if (hashq-ref sleeping-q f)
    (begin
      (hashq-remove! sleeping-q f)
      (hashq-set! running-q f f))))

;; join fibers
(define (join . fs)
  (for-each
    (lambda (f)
      (hashq-set! f 'on (cons (fiber/current) (hashq-ref f 'on))))
    fs)
  (letrec ((l (lambda ()
                (let ((r (fold 
                          (lambda (f t)
                            (or (fiber/alive? f) t))
                          #f
                          fs)))
                  (if r 
                    (begin
                      (sleep)
                      (l)))))))
     (l)))

;; run a round
(define (run-one)
  (hash-for-each
    (lambda (k v)
      (hashq-remove! running-q k)
      (fiber/resume v)
      (for-each
        (lambda (f)
          (wake f))
        (hashq-ref v 'on)))
    running-q))

;; start scheduler
(define (run)
  (set! stopped #f)
  (letrec ((r (lambda ()
                (if (not stopped)
                  (begin
                    (run-one)
                    (next-tick r))))))
    (next-tick r)))

;; stop scheduler
(define (stop)
  (set! stopped #t))

(export-mo 'run)
(export-mo 'spawn)
(export-mo 'join)
(export-mo 'sleep)
(export-mo 'wake)
(export-mo 'stop)

