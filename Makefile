<<<<<<< HEAD
build18: main.cpp
	clang++ -O3 -lopencv_core -lopencv_imgcodecs -lportaudio -I/usr/include/opencv4 -o build18 main.cpp
=======
CC = clang++
CFLAGS = -Wall -Wextra -O3
LIBS = -lportaudio
SDIR = src
BDIR = bld
ODIR = obj
SRCS = $(wildcard $(SDIR)/*.cpp)
OBJS = $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRCS))
TARGET = main

.PHONY: all
all: $(TARGET)

.PHONY: main
$(TARGET): $(BDIR)/$(TARGET)

$(ODIR)/%.o: $(SDIR)/%.cpp
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

$(BDIR)/$(TARGET): $(OBJS)
	mkdir -p bld/
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f obj/*
	rm -f bld/*
>>>>>>> 7dbbbff1173bb7a149233c56b5e0416a835e17c3
