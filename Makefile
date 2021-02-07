build18: main.cpp
	clang++ -O3 -lopencv_core -lopencv_imgcodecs -lportaudio -I/usr/include/opencv4 -o build18 main.cpp
