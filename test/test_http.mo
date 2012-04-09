(require-mo "http" 'http)

(http/start-server "0.0.0.0" 8080
  (lambda (req resp)
    (hashq-set! resp 'body "<b>hello world!</b>")
    resp))
