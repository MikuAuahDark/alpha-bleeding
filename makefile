ifeq ($(shell uname | grep 'MINGW32_NT' -c),1)
  flags += -static-libgcc -static-libstdc++
endif

all: bin bin/alpha-bleeding.exe bin/alpha-remove.exe bin/alpha-set.exe bin/alpha-trim.exe

bin:
	mkdir bin

bin/alpha-bleeding.exe: src/alpha-bleeding.cpp src/png.cpp src/png.h src/lodepng.h src/lodepng.cpp
	g++ $(CFLAGS) src/alpha-bleeding.cpp src/png.cpp src/lodepng.cpp $(LDFLAGS) -O3 $(flags) -o $@

bin/alpha-remove.exe: src/alpha-remove.cpp src/png.cpp src/png.h src/lodepng.h src/lodepng.cpp
	g++ $(CFLAGS) src/alpha-remove.cpp src/png.cpp src/lodepng.cpp $(LDFLAGS) -O3 $(flags) -o $@

bin/alpha-set.exe: src/alpha-set.cpp src/png.cpp src/png.h src/lodepng.h src/lodepng.cpp
	g++ $(CFLAGS) src/alpha-set.cpp src/png.cpp src/lodepng.cpp $(LDFLAGS) -O3 $(flags) -o $@

bin/alpha-trim.exe: src/alpha-trim.cpp src/png.cpp src/png.h src/lodepng.h src/lodepng.cpp
	g++ $(CFLAGS) src/alpha-trim.cpp src/png.cpp src/lodepng.cpp $(LDFLAGS) -O3 $(flags) -o $@

clean:
	rm -r bin

.PHONY: clean
