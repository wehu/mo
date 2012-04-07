(next-tick 
  (lambda () 
    (let ()
      (display "hi1\n")
      (next-tick
        (lambda ()
          (display "hi2\n"))))))

(set-timeout
  (lambda ()
    (display "hi3\n")) 1000)

(let ((id (set-interval
  (lambda ()
    (display "hi4\n")) 1000)))
    (clear-timer id))

(event-on "hi5"
  (lambda (args)
    (display args)))
(event-notify "hi5" `("a"))

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
;(pipe-write ss `(1 3 13))


;(fs-open "test.log" 0 1
;  (lambda (fd, errno)
;     (info "hi8")))

(define server (tcp-new))
(tcp-on server "read" (lambda (o) (display o)))
(tcp-on server "connection" (lambda (c) 
                (tcp-on c "read" (lambda (o) (display o)
                                              (tcp-destroy c)
                                              (tcp-destroy server)))
                (tcp-resume c)))
(tcp-bind server "0.0.0.0" 3000)
(tcp-listen server 128 (lambda (o) (display o)))
(define client (tcp-new))
(tcp-connect client "0.0.0.0" 3000 (lambda (o) (display "try to connect\n")))
(tcp-write client `(1 2 3 4 5))
(tcp-on client "write" (lambda (o) (tcp-destroy client)))

(gc)
