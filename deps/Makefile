all : libuv/.git

libuv/.git : Makefile
	mkdir -p libuv
	if test -d "libuv/.git"; then \
	  rm libuv -rf; \
	fi;
	git clone https://github.com/joyent/libuv.git

clean :
