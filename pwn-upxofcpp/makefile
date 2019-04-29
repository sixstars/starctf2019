CXXFLAGS = -O2 -Wall -fstack-protector-all -pie -fPIE -s
chall:

upx: chall
	cp chall upxofcpp 
	upx upxofcpp
	
clean:
	rm chall

all: upx
	cp /lib/x86_64-linux-gnu/libc-2.23.so .
	tar czf upxofcpp.tar.gz libc-2.23.so upxofcpp
