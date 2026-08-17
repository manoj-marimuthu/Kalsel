TARGET = kalsel
OBJ = build/main.o build/error.o build/gc.o build/token.o build/lexer.o build/parser.o build/tac.o build/asmX86.o

COMMAND = gcc -g -Iinclude -Wall 

$(TARGET): $(OBJ)
	$(COMMAND) $(OBJ) -o $(TARGET)

build/main.o: main.c
	$(COMMAND) -c main.c -o build/main.o

build/error.o: src/error.c
	$(COMMAND) -c src/error.c -o build/error.o

build/gc.o: src/gc.c
	$(COMMAND) -c src/gc.c -o build/gc.o

build/token.o: src/token.c
	$(COMMAND) -c src/token.c -o build/token.o

build/lexer.o: src/lexer.c
	$(COMMAND) -c src/lexer.c -o build/lexer.o

build/parser.o: src/parser.c
	$(COMMAND) -c src/parser.c -o build/parser.o

build/tac.o: src/tac.c
	$(COMMAND) -c src/tac.c -o build/tac.o

build/asmX86.o: src/asmX86.c
	$(COMMAND) -c src/asmX86.c -o build/asmX86.o

clean:
	rm $(TARGET) build/*.o
