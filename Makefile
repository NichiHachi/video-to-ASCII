CXX = g++
CXX_C = gcc
CXXFLAGS = `pkg-config --cflags opencv4`
LDLIBS_OPENCV = `pkg-config --libs opencv4`
LDFLAGS = -L/usr/local/lib
LDLIBS = -lsfml-graphics -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

OBJDIR = obj
BINDIR = bin

OBJC = $(OBJDIR)/asciiToFrame.o $(OBJDIR)/charToMatrix.o $(OBJDIR)/frameToASCII.o $(OBJDIR)/frameToVideo.o $(OBJDIR)/videoToFrame.o

all : directories $(OBJDIR)/videoToASCII.o $(BINDIR)/main

directories: ${OBJDIR} ${BINDIR}

${OBJDIR}:
	mkdir -p ${OBJDIR}

${BINDIR}:
	mkdir -p ${BINDIR}

$(BINDIR)/main: src/main.cpp $(OBJDIR)/videoToASCII.o
	$(CXX) $^ $(OBJC) -o $@ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $(LDLIBS_OPENCV)

main : main.c
	$(CXX_C) $< -o $@ $(shell pkg-config --cflags --libs gtk+-3.0)

$(OBJDIR)/videoToASCII.o: src/videoToASCII.cpp src/videoToASCII.h $(OBJC)
	$(CXX) -c src/videoToASCII.cpp -o $@ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $(LDLIBS_OPENCV)

$(OBJDIR)/asciiToFrame.o: src/asciiToFrame.cpp src/asciiToFrame.h
	$(CXX) -c src/asciiToFrame.cpp -o $@

$(OBJDIR)/charToMatrix.o: src/charToMatrix.cpp src/charToMatrix.h
	$(CXX) -c src/charToMatrix.cpp -o $@

$(OBJDIR)/frameToASCII.o: src/frameToASCII.cpp src/frameToASCII.h
	$(CXX) -c src/frameToASCII.cpp -o $@

$(OBJDIR)/frameToVideo.o: src/frameToVideo.cpp src/frameToVideo.h
	$(CXX) -c src/frameToVideo.cpp $(CXXFLAGS) $(LDFLAGS) $(LDLIBS_OPENCV) -o $@

$(OBJDIR)/videoToFrame.o: src/videoToFrame.cpp src/videoToFrame.h
	$(CXX) -c src/videoToFrame.cpp $(CXXFLAGS) $(LDFLAGS) $(LDLIBS_OPENCV) -o $@

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/main