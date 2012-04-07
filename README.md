# mo

mo is an attempt to copy node.js's architecture based on guile and libuv.
On top of guile and libuv, it's funny to program in scheme in a platform like node.js.
User will benefit from both scheme language features and asynchronous IO.

This project is under heavy development.

## Build Instructions

Download source code from github:

	git clone git://github.com/wehu/mo.git
	cd mo
	./configure
	make
	make install

## Example

See `test/test_http.mo`:

	(require-mo "http" 'http)
	
	(http/start-server "0.0.0.0" 8080
	  (lambda (req)
	    "<b>hello world</b>")

Run example:

	mo test/test_http.mo

