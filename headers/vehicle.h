#ifndef VEHICLE_H
#define VEHICLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define CAR_WIDTH 74
#define CAR_HEIGHT 74
#define CAR_CLIPS 8

#define ENTER_POINT_X 1990
#define ENTER_POINT_Y 730

#define FIRST_INT_X 1685
#define FIRST_INT_Y 730

#define SEC_INT_X 1150
#define SEC_INT_Y 730

#define FIRST_PP_X 1685
#define FIRST_PP_Y 635

#define SEC_PP_X 1685
#define SEC_PP_Y 565

#define THIR_PP_X 1685
#define THIR_PP_Y 495

#define FOUR_PP_X 1685
#define FOUR_PP_Y 425

#define FIF_PP_X 1685
#define FIF_PP_Y 355

#define FIF_PP_X 1685
#define FIF_PP_Y 355

#define SIX_PP_X 1685
#define SIX_PP_Y 285

#define FIRST_PLACE_X 1430
#define FIRST_PLACE_Y 630

#define SEC_PLACE_X 1430
#define SEC_PLACE_Y 560

#define THIR_PLACE_X 1430
#define THIR_PLACE_Y 490

#define FOUR_PLACE_X 1430
#define FOUR_PLACE_Y 420

#define FIF_PLACE_X 1430
#define FIF_PLACE_Y 350

#define SIX_PLACE_X 1685
#define SIX_PLACE_Y 280

#define DELIMITER_INT 80
#define DELIMITER_PP 285

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

Vehicle newVehicleList(void);
Vehicle newVehicle(char direction, int posx, int posy, 
				   int vitesse, char alignement, char type, char etat);
Vehicle addVehicle(Vehicle v, 
				   char direction, int posx, int posy, 
				   int vitesse, char alignement, char type, char etat);
Vehicle pollFirstVehicle(Vehicle v, Vehicle * vPoll);
void changeVehicleDirection(Vehicle * v, SDL_Keycode key);
void changeVehiclePosition(Vehicle * v, SDL_Keycode key);
void moveVehicle(Vehicle * v);
bool isEmpty(Vehicle v);
bool isAnObstacles(SDL_Rect * obstacles, int size, int x, int y);
bool isInRegion(int x, int y, int xInf, int xSup, int yInf, int ySup);
bool isBetween(int x, int y, int x2, int x3, int y2, int y3);
void freeVehicleList(Vehicle v);
#endif