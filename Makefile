all:mytable.c
	gcc -o mytable -I/usr/include/mysql  -g -pipe -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -m32 -fasynchronous-unwind-tables -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -fno-strict-aliasing -fwrapv -fPIC   -DUNIV_LINUX -DUNIV_LINUX mytable.c -rdynamic -L/usr/lib/mysql -lmysqlclient -lz -lcrypt -lnsl -lm -lssl -lcrypto
clean:
	rm -f mytable
