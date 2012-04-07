# mo

mo is an attempt to copy node.js's architecture on top of libuv and guile.
By guile, it's funny to program in scheme in a platform like node.js.
User will benefit from both scheme language features and asynchronous IO.

This project is under heavy development.

## Build Instructions

See INSTALL.

## Example

See `test/test_http.mo`:

	(require-mo "http" 'http)
	
	(http/start-server "0.0.0.0" 8080
	  (lambda (req)
	    "<b>hello world</b>")


