CPP := g++
CPPFLAGS := -I include/ -L lib/
LIBFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SRC_FILES := src/*.cpp

EXEC := "Epic Tetris.exe"

all:
	$(CPP) $(SRC_FILES) -o bin/$(EXEC) $(CPPFLAGS) $(LIBFLAGS)

release:
	$(CPP) $(SRC_FILES) -o bin/$(EXEC) -O3 $(CPPFLAGS) -mwindows $(LIBFLAGS)

run:
	cd bin && ./$(EXEC)
