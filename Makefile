CC=g++
SRC=src
BIN=bin

FLAGS=
INCLUDES = -lsfml-graphics -lsfml-window -lsfml-system -ljsoncpp

all:main.exe clean run

main.exe:main.o logic-Gate.o logic-operandi.o
	$(CC) $(FLAGS) *.o -o $(BIN)/main.exe $(INCLUDES)


%.o:$(SRC)/%.cpp
	$(CC) $(FLAGS) -c $<
%.o:$(SRC)/hsrc/%.cpp
	$(CC) $(FLAGS) -c $<

clean:
	rm *.o

run:
	cmd /K start $(BIN)/main.exe