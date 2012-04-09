(require-mo "http" 'http)

(http/start-server "0.0.0.0" 8080
  (lambda (req resp)
    (let ((method (hashq-ref req 'method))
          (uri (hashq-ref req 'uri)))
      (if (string=? uri "/")
        (hashq-set! resp 'body (string-append
                                 "<b>hello world!</b><br>"
                                 "<a href=\"/test\">test</a>")))
      (if (string=? uri "/test")
        (hashq-set! resp 'body (string-append
                                 "<a href=\"/\">back</a>")))
      resp)))
