all: tj_dis

tj_dis: tj_dis.c
	x86_64-w64-mingw32-gcc -o $@ $< -O3
