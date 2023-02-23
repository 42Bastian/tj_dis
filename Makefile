all: tj_dis

CFLAGS = -O2 -fomit-frame-pointer
#EXT=.exe

tj_dis.exe: tj_dis.c
	x86_64-w64-mingw32-gcc $(CFLAGS) -o $@ $<
	upx -9 $@$(EXT)

clean:
	rm tj_dis$(EXT)
