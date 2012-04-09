(use-modules (web http))

;; convert data into string
(define (data->string data)
  (list->string
    (map
      (lambda (i) (integer->char i))
      data)))

;; convert string to data
(define (string->data s)
  (map
    (lambda (i) (char->integer i))
    (string->list s)))

;; convert http requset to a hash table
(define (parse-http-req req)
  (let ((t (make-hash-table))
        (r (string-split req #\newline)))
    (let ((f (string-split (car r) #\space)))
      (hashq-set! t 'method (car f))
      (hashq-set! t 'uri (car (cdr f))))
    (map
      (lambda (l)
        (let* ((p (string-split l #\:))
               (k (string->header (car p)))
               (v (string-join (cdr p))))
          (if (string<> v "")
            (hashq-set! t k v))))
      (cdr r))
    t))

;; generate a default http response table
(define (default-http-response)
  (let ((t (make-hash-table)))
    (hashq-set! t 'accept-ranges "bytes")
    (hashq-set! t 'connection "close")
    (hashq-set! t 'content-length "0")
    (hashq-set! t 'content-type "text/html; charset=UTF-8")
    (hashq-set! t 'body "")
    t))

;; generate a http response from a table
(define (http-response resp)
  (let ((body (hashq-ref resp 'body)))
    (hashq-remove! resp 'body)
    (let ((t (hash-map->list
             (lambda (k v)
               (if (eq? k 'content-length)
                 (string-append (header->string k) ": " (number->string (string-length body)))
                 (string-append (header->string k) ": " v)))
             resp)))
      (string-append
        "HTTP/1.1 200 OK\n"
        (string-join t "\n")
        "\n\n"
        body))))

;; run a http server
(define (start-server ip port cb)
  (let ((server (tcp-new)))
    (tcp-on server "connection"
      (lambda (client)
        (tcp-on client "read"
          (lambda (data)
            (let ((req (parse-http-req (data->string data))))
            (tcp-write client
              (string->data (http-response (cb req (default-http-response))))))))
        (tcp-on client "write"
          (lambda (arg)
            (display "response done\n")))
        (tcp-resume client)))
    (tcp-bind server ip port)
    (tcp-listen server 127)
    (display (string-append "http server start @ "
                         ip ":"
                         (number->string port) "\n"))))

(export-mo 'start-server)
