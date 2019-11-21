#include "../headers/utils.h"

void initMapWithWall(char matrix[MAPSIZE_X][MAPSIZE_Y])
{
	for(int i = 0; i < MAPSIZE_X; ++i)
		for(int j = 0; j < MAPSIZE_Y; ++j)
			matrix[i][j] = ' ';
}

/*void moveVehicleOnMatrix(Vehicle v, char matrix[MAPSIZE_X][MAPSIZE_Y])
{
	///for(int i = )
}*/
