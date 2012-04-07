;(use-modules (web http))

(define data->string
  (lambda (data)
    (list->string (map (lambda (i) (integer->char i)) data))))

(define string->data
  (lambda (s)
    (map (lambda (i) (char->integer i)) (string->list s))))

(define http-response
  (lambda (resp)
    (string-append "HTTP/1.1 200 OK\nAccept-Ranges: bytes\nConnection: close\nContent-Length: "
         (number->string (string-length resp)) 
         "\nContent-Type: text/html; charset=UTF-8\n\n" 
         resp)))

(define start-server
  (lambda (ip port cb)
    (let ((server (tcp-new)))
      (tcp-on server "connection"
        (lambda (client)
          (tcp-on client "read"
            (lambda (data)
              ;(display (data->string data))
              ;(display (http-response (cb (data->string data))))
              (tcp-write client (string->data (http-response (cb (data->string data)))))))
          (tcp-on client "write"
            (lambda (arg) (display "response done\n")))
          (tcp-resume client)))
      (tcp-bind server ip port)
      (tcp-listen server 127
        (lambda (o) (display o)))
      (display (string-append "http server start @ "
                           ip ":"
                           (number->string port) "\n")))))

(export-mo 'start-server)
