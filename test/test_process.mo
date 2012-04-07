
(define p (process-new))
(process-set-file p "echo")
(process-set-args p `("echo" "abc" "def"))
(process-on-exit p (lambda (e s) (display "hi7\n")))

(define s (pipe-new))
(pipe-on s "read" (lambda (o) (display (list->string (map (lambda (i) (integer->char i)) o)))))
(process-set-stdout p s)

(define ss (pipe-new))
(pipe-on ss "read" (lambda (o) (display (list->string (map (lambda (i) (integer->char i)) o)))))
(process-set-stderr p ss)

(process-run p)

(pipe-resume s)
(pipe-resume ss)

(display (process-get-pid p))
(display (process-get-file p))
(display (process-get-args p))

(require-mo "process" 'process)

(process/popen2 `("echo" "hihihi")
  (lambda (o) (display o))
  (lambda (o) (display o)))

(gc)
