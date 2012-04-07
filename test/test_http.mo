(require-mo "http" 'http)

(http/start-server "0.0.0.0" 8080
  (lambda (req)
    "<b>hello world</b>"))
