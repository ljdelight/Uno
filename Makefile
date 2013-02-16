CC=g++
FLAGS=-O2 -Wall
PROJECT=Uno
FILES=src/main.cc src/Card.h src/Deck.h src/Uno.h src/Player.h Makefile $(PROJECT)

$(PROJECT): main.o
	$(CC) $(FLAGS) main.o -o $@

main.o: src/main.cc
	$(CC) $(FLAGS) -c $^

tar: $(PROJECT)
	tar -cvzf Uno_source.tar $(FILES)

clean:
	rm -f *~ *.o *.gch $(PROJECT)
