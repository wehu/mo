(require-mo "fiber" 'fiber)

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
    f))

;; sleep current fiber
(define (sleep)
  (hashq-remove! running-q (fiber/current))
  (hashq-set! sleeping-q (fiber/current) (fiber/current))
  (fiber/yield))

;; wake up a fiber
(define (wake f)
  (hashq-remove! sleeping-q f)
  (hashq-set! running-q f f))

;; run a round
(define (run-one)
  (hash-for-each
    (lambda (k v)
      (fiber/resume v))
    running-q))

;; start scheduler
(define (run)
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
(export-mo 'sleep)
(export-mo 'wake)
(export-mo 'stop)

