all:
	gcc -c -Wall -Werror -fpic i2c1602.c	# compile pic
	gcc -shared -o libi2c1602.so i2c1602.o	# compile shared
install:
	sudo cp libi2c1602.so /usr/lib		# copy .so to /usr/lib
	sudo cp i2c1602.h /usr/include		# copy .h to /usr/include
	sudo chmod 0755 /usr/lib/ libi2c1602.so	# change permision
	sudo ldconfig				# update ld.so config file and cache
clean:
	rm -rf *.so *.o 