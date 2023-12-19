

finalProject: main.o evidence.o room.o building.o ghost.o hunter.o
	gcc main.o evidence.o room.o building.o ghost.o hunter.o -o finalProject

main.o: main.c defs.h
	gcc -c main.c

evidence.o: evidence.c defs.h
	gcc -c evidence.c

room.o: room.c defs.h
	gcc -c room.c

building.o: building.c defs.h
	gcc -c building.c

ghost.o: ghost.c defs.h
	gcc -c ghost.c

hunter.o: hunter.c defs.h
	gcc -c hunter.c

clean:
	rm *.o finalProject


