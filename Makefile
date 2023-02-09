CC=g++
SRC=src
BIN=bin

FLAGS= -Wall
INCLUDES = -lsfml-graphics -lsfml-window -lsfml-system -ljsoncpp

all:main.exe clean run

main.exe:main.o logic-Gate.o logic-operandi.o DrawGate.o scrollContainer.o
	$(CC) $(FLAGS) *.o -o $(BIN)/main.exe $(INCLUDES)
console.exe:logic-Gate.o logic-operandi.o console.o
	$(CC) $(FLAGS) *.o -o $(BIN)/main.exe $(INCLUDES)


%.o:$(SRC)/%.cpp
	$(CC) $(FLAGS) -c $<
%.o:$(SRC)/hsrc/%.cpp
	$(CC) $(FLAGS) -c $<

clean:
	rm *.o

run:
	cmd /K start $(BIN)/main.exe