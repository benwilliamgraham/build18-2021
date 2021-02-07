CC = clang++
CFLAGS = -Wall -Wextra -O3
INC = -I/usr/include/opencv4
LIBS = -lportaudio -lopencv_core -lopencv_imgcodecs
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
	$(CC) -c -o $@ $< $(INC) $(CFLAGS) $(LIBS)

$(BDIR)/$(TARGET): $(OBJS)
	mkdir -p bld/
	$(CC) -o $@ $^ $(INC) $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f obj/*
	rm -f bld/*
