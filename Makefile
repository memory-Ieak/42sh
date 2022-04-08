all: 42sh

42sh: 
	meson setup builddir
	ninja -C builddir

check:
	make -f makefile_testsuite check

clean:
	rm -rf builddir