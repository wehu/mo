(require-mo "http" 'http)

(http/start-server "0.0.0.0" 8080
  (lambda (req resp)
    (display (hashq-ref req 'method))
    (display "\n")
    (display (hashq-ref req 'uri))
    (display "\n")
    (hashq-set! resp 'body "<b>hello world!</b>")
    resp))
