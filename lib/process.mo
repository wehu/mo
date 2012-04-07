(define popen2
  (lambda (cmd stdout-cb stderr-cb)
    (let ((p (process-new))
          ;(in (pipe-new))
          (out (pipe-new))
          (err (pipe-new)))
       (process-set-file p (car cmd))
       (process-set-args p cmd)
       ;(process-on-exit p exit-cb)
       (pipe-on out "read"
         (lambda (o)
           (stdout-cb (list->string (map (lambda (i) (integer->char i)) o)))))
       (process-set-stdout p out)
       (pipe-on err "read"
         (lambda (o) 
           (stdout-cb (list->string (map (lambda (i) (integer->char i)) o)))))
       (process-set-stderr p err)
       (process-run p)
       (pipe-resume out)
       (pipe-resume err))))

;(popen2 `("echo" "hi")
;  (lambda (o) (display o))
;  (lambda (o) (display o)))

(export-mo 'popen2)

