# Makefile Projet Arnold 
# Clément Caumes

CFLAGS=-c -g -Wall -Wextra
EXEC=arnold
CC=gcc
SRC=app/src
INC=app/inc
TEST=test/
BLD=build

compil: clean $(BLD) audio.o graphic.o arnold.o errors.o 
	$(CC) -o $(BLD)/src/$(EXEC) $(BLD)/src/arnold.o $(BLD)/src/audio.o $(BLD)/src/graphic.o $(BLD)/src/errors.o `sdl-config --libs`

run: compil 
	$(BLD)/src/$(EXEC)
	
build: 
	mkdir $(BLD)/
	
build_src: build
	mkdir $(BLD)/src/
	
build_test: build
	mkdir $(BLD)/test/
	
doxygen:
	doxygen doc/Doxyfile 
	
test: clean $(BLD) test_audio.o audio.o arnold.o errors.o 
	$(CC) -o $(BLD)/test/test_audio $(BLD)/test/test_audio.o $(BLD)/src/audio.o $(BLD)/src/errors.o -lcunit
	$(BLD)/test/test_audio

test_audio.o: build_test
	$(CC) $(CFLAGS) $(TEST)/test_audio.c -o $(BLD)/test/test_audio.o
	
audio.o: build_src
	$(CC) $(CFLAGS) $(SRC)/audio.c -o $(BLD)/src/audio.o

arnold.o: build_src
	$(CC) $(CFLAGS) $(SRC)/arnold.c -o $(BLD)/src/arnold.o
		
errors.o: build_src
	$(CC) $(CFLAGS) $(SRC)/errors.c -o $(BLD)/src/errors.o
	
graphic.o: build_src
	$(CC) $(CFLAGS) $(SRC)/graphic.c `sdl-config --cflags` -o $(BLD)/src/graphic.o
	
clean: 
	rm -rf build/
	
install_cunit:
	sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev
