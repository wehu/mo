(event-on "e1"
 (lambda ()
   (display "e1\n")
   (event-notify "e2" `("e2\n"))))

(event-notify "e1" `())

(event-on "e2"
 (lambda (a)
   (display a)))
