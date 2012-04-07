bin_PROGRAMS = mo
mo_SOURCES = src/main.cpp \
             src/mo_object.cpp \
             src/mo.cpp \
             src/mo_tick.cpp \
             src/mo_timer.cpp \
             src/mo_event.cpp \
             src/mo_process.cpp \
             src/mo_fs.cpp \
             src/mo_logger.cpp \
             src/mo_stream.cpp \
             src/mo_pipe.cpp \
             src/mo_tcp.cpp \
             src/mo_tty.cpp \
             src/mo_module.cpp \
             src/mo_utils.cpp
SUBDIRS = deps/libuv
INCLUDES = -Ideps/libuv/include -Isrc
AM_CPPFLAGS = `pkg-config guile-2.0 --cflags`
LIBS = `pkg-config guile-2.0 --libs` deps/libuv/uv.a -lm -ldl -lrt -pthread

#mo_DEPENDENCIES = deps/libuv/.git

#deps/libuv/.git : deps/libuv
#	@mkdir -p $<
#	cd deps ; git clone https://github.com/joyent/libuv.git
