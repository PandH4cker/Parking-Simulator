#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "../headers/vehicle.h"

#define MAPSIZE_X 1920
#define MAPSIZE_Y 1080

void initMapWithWall(char matrix[MAPSIZE_X][MAPSIZE_Y]);
void setPlaces(char matrix[MAPSIZE_X][MAPSIZE_Y]);
void moveVehicleOnMatrix(Vehicle v);
#endif