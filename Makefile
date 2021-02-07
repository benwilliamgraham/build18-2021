build18: main.cpp
	clang++ -O3 -I/usr/include/opencv2 -o build18 main.cpp -lopencv_core -lopencv_imgcodecs -lportaudio -lopencv_videoio -lopencv_imgproc -lopencv_highgui -lX11
