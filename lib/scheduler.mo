(require-mo "fiber" 'fiber)

(define sleeping-q (make-hash-table))
(define running-q (make-hash-table))
(define stopped #f)

(define (spawn body)
  (let ((f (fiber/new body)))
    (hashq-set! running-q f f)
    f))

(define (sleep f)
  (hashq-remove! running-q f)
  (hashq-set! sleeping-q f f)
  (fiber/yield f))

(define (wake f)
  (hashq-remove! sleeping-q f)
  (hashq-set! running-q f f))

(define (run-one)
  (hash-for-each
    (lambda (k v)
      (fiber/resume v))
    running-q))

(define (run)
  (letrec ((r (lambda ()
                (if (not stopped)
                  (begin
                    (run-one)
                    (next-tick r))))))
    (next-tick r)))

(define (stop)
  (set! stopped #t))

(export-mo 'run)
(export-mo 'spawn)
(export-mo 'sleep)
(export-mo 'wake)
(export-mo 'stop)

