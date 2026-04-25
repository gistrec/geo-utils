sample :
	g++ samples/Samples.cpp \
	-std=c++14 -Iinclude/   \
	-o Sample

gtests :
	g++ tests/Tests.cpp                        \
	-std=c++17 -Iinclude/                      \
	$(shell pkg-config --cflags --libs gtest)  \
	-pthread                                   \
	-o GTests
