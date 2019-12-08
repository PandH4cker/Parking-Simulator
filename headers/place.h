#ifndef PLACE_H
#define PLACE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Place
{
	int posX;
	int posY;
	bool isFree;
	int posXLight;
	int posYLight;
}Place;



#endif