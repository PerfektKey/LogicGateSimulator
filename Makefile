CC=g++
SRC=src
BIN=bin

FLAGS=
INCLUDES = -lsfml-graphics -lsfml-window -lsfml-system -ljsoncpp

all:main.exe clean run

main.exe:main.o
	$(CC) $(FLAGS) *.o -o $(BIN)/main.exe $(INCLUDES)


%.o:$(SRC)/%.cpp
	$(CC) $(FLAGS) -c $<

clean:
	rm *.o

run:
	cmd /K start $(BIN)/main.exe