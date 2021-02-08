.PHONY: oscopegl
oscopegl:
	clang++ -lportaudio -O3 -o test $(wildcard src/*.cpp)
