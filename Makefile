CC=g++
SRC=src
BIN=bin
CPPFILES=$(foreach D,$(SRC)/hsrc,$(wildcard $(D)/*.cpp))
OBJFILES=$(patsubst %.cpp,%.o,$(CPPFILES))

FLAGS= -Wall
OINCLUDES = -lstdc++fs -std=c++20 
INCLUDES = -lstdc++fs -std=c++20 -lsfml-graphics -lsfml-window -lsfml-system -ljsoncpp

all:main run

main: main.o $(OBJFILES)
	$(CC) $(FLAGS) $(BIN)/*.o -o $(BIN)/main $(INCLUDES)

%.o:%.cpp
	g++ -o $(patsubst src/hsrc/%.cpp,bin/%.o,$<) -c $< $(OINCLUDES)
main.o:$(SRC)/main.cpp
	$(CC) -o $(BIN)/main.o -c $(SRC)/main.cpp $(OINCLUDES)

release:$(BIN)/main.exe
	@mkdir -p rel
	@cp MAIN_FONT.TTF rel
	@cp -r jsonData rel
	@cp $(BIN)/main.exe rel

clean:
	rm *.o

run:
	cmd /K start $(BIN)/main.exe