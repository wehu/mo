# mo

mo = guile + libuv

## features:

  * asynchronous IO by libuv
  * scheme scripting for frontend 
  * node.js style module system
  * ruby style fiber library
  * fiber scheduler library in event loop

## module system:

mo's module system is like node.js's. A mo file is a module. 'require-mo' is to load/evaluate file and 'export-mo' is to export functions in a module, so that other modules can access the functions.

  * (require-mo "module name" 'instance-name)

    'require-mo' loads module if it's not loaded before. The second argument is the instance name. It's used as export function's prefix. The functions exported by module will be like 'instance-name/foo'.

  * (export-mo 'function)

    'export-mo' exports the functions in current module. 

  example:

  file: `m1.mo`

	(define (foo)
	  (display "foo\n"))
	(export-mo 'foo)

  file: `m2.mo`

	(require-mo "m1.mo" 'm)
	(m/foo)

## tick:

  * (next-tick callback):

    'next-tick' registers a callback into event loop. When event loop is free from IO activities, callback will be invoked.

  example:

	(next-tick 
	  (lambda ()
	    (display "hi\n")))

## timer:

  * (set-timeout callback delay):

    'set-timeout' registers a timer. After time indicated by delay is out, callback will be invoked.

  example:

	(set-timeout (lambda ()
	  (display "timeout\n"))
	  100)

  * (set-interval callback delay):

    'set-interval' is like set-timeout, but the callback will be repeatly invoked after delay ms.

  * (clear-timer id)

    set-timer/interval return a IP for timer reference. 'clear-timer' can be used to terminate the timer.

  example:

	(define timer (set-timeout
	  (lambda ()
	    (display "hi\n"))
          1000))
	(clear-timer timer)


