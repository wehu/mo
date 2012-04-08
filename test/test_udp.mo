(define u1 (udp-new))
(udp-bind u1 "0.0.0.0" 12345)
(udp-on u1 "receive"
  (lambda (d)
    (display d)
    (udp-stop-receive u1)
    (udp-destroy u1)))
(udp-receive u1)

(define u2 (udp-new))
(udp-on u2 "send"
  (lambda (s)
    (display "u2 send done")
    (udp-destroy u2)))
(udp-send u2 `(1 2 3 4) "127.0.0.1" 12345)

