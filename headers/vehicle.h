#ifndef VEHICLE_H
#define VEHICLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define CAR_WIDTH 74
#define CAR_HEIGHT 74
#define CAR_CLIPS 8

#define ENTER_POINT_X 1865
#define ENTER_POINT_Y 730

#define NORTH 'N'
#define SOUTH 'S'
#define EAST 'E'
#define WEST 'O'

#define LEFT 'g'
#define RIGHT 'd'

#define CAR 'v'
#define TRUCK 'c'

#define ACTIVE '1'
#define INACTIVE '0'

typedef struct NodeVehicleElement
{
	char currentDirection;
	char previousDirection;
	int posx;
	int posy;
	int vitesse;
	char alignement;
	char type;
	char etat;
	unsigned long int temps;
	struct NodeVehicleElement * next;
}NodeVehicleElement, * Vehicle;

Vehicle newVehicle(char direction, int posx, int posy, 
				   int vitesse, char alignement, char type, char etat);
Vehicle addVehicle(Vehicle v, 
				   char direction, int posx, int posy, 
				   int vitesse, char alignement, char type, char etat);
Vehicle pollFirstVehicle(Vehicle v, Vehicle * vPoll);
void changeVehicleDirection(Vehicle * v, SDL_Keycode key);
void changeVehiclePosition(Vehicle * v, SDL_Keycode key);
void moveVehicle(Vehicle * v, SDL_Keycode key);
bool isEmpty(Vehicle v);
#endif