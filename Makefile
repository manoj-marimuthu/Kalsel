TARGET = kalsel
OBJ = build/main.o \
      build/lexer.o \
      build/parser.o \
      build/ir.o \
      build/code_gen.o

COM = g++
CFLAGS = -Wall -g -Iincludes


$(TARGET): $(OBJ)
	$(COM) $(CFLAGS) $(OBJ) -o $(TARGET)

build/main.o: main.cpp
	$(COM)  $(CFLAGS) -c main.cpp -o build/main.o

build/lexer.o: src/lexer.cpp
	$(COM) $(CFLAGS) -c src/lexer.cpp -o build/lexer.o
build/parser.o: src/parser.cpp
	$(COM) $(CFLAGS) -c src/parser.cpp -o build/parser.o
build/ir.o: src/ir.cpp
	$(COM) $(CFLAGS) -c src/ir.cpp -o build/ir.o
build/code_gen.o: src/code_gen.cpp
	$(COM) $(CFLAGS) -c src/code_gen.cpp -o build/code_gen.o
clean:
	rm -Rf build/*.o kalsel asm-modules/program.asm
