CC = gcc
EXEC = ParkingSimulator

ParkingSimulator:main.o parkingSimulator.o utils.o vehicle.o
	$(CC) -o $(EXEC) main.o parkingSimulator.o utils.o vehicle.o -lSDL2 -lSDL2_image -lSDL2_ttf -std=c99
	./$(EXEC)

main.o:main.c ../headers/parkingSimulator.h
	$(CC) -o main.o -c main.c -std=c99

parkingSimulator.o:parkingSimulator.c ../headers/parkingSimulator.h
	$(CC) -o parkingSimulator.o -c parkingSimulator.c -std=c99

utils.o:utils.c ../headers/utils.h
	$(CC) -o utils.o -c utils.c -std=c99

vehicle.o:vehicle.c ../headers/vehicle.h
	$(CC) -o vehicle.o -c vehicle.c -std=c99

clean:
	rm -rf *.o

mrproper:clean
	rm -rf $(EXEC)