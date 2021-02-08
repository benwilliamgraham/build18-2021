.PHONY: oscopegl
oscopegl:
	clang++ -lportaudio -O3 -o spot $(wildcard src/*.cpp)
