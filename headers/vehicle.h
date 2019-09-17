#ifndef VEHICLE_H
#define VEHICLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct NodeVehicleElement
{
	char direction;
	int posx;
	int posy;
	int vitesse;
	char alignement;
	char type;
	char etat;
	unsigned long int temps;
	struct NodeVehicleElement * next;
}NodeVehicleElement, * Vehicle;

Vehicle addVehicle(char direction, int posx, int posy, 
				   int vitesse, char alignement, char type, char etat);
#endif