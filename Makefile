sample:
	g++ samples/samples.cpp \
	-std=c++17 -Iinclude/   \
	-o sample

gtests:
	g++ tests/Tests.cpp                        \
	-std=c++17 -Iinclude/                      \
	$(shell pkg-config --cflags --libs gtest)  \
	-pthread                                   \
	-o gtests
