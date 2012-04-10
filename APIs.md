# mo

mo = guile + libuv

## features:

  * asynchronous IO by libuv
  * scheme scripting for frontend 
  * node.js style module system
  * ruby style fiber library
  * fiber scheduler library in event loop

## tick:

  * (next-tick callback):
    next-tick will register a callback into event loop. When event loop is free from IO activities, callback will be invoked.

    example:
	(next-tick 
	  (lambda ()
	    (display "hi\n")))
    
