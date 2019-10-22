#include "../headers/vehicle.h"

Vehicle newVehicle(char direction, int posx, int posy, 
				   int vitesse, char alignement, char type, char etat)
{
	NodeVehicleElement * v = malloc(sizeof(*v));
	if(!v)
	{
		fprintf(stderr, "Unable to allocate the node, out of memory\n");
		exit(EXIT_FAILURE);
	}

	v->currentDirection = direction;
	v->previousDirection = direction;
	v->posx = posx;
	v->posy = posy;
	v->vitesse = vitesse;
	v->alignement = alignement;
	v->type = type;
	v->etat = etat;
	v->next = NULL;

	return v;
}

Vehicle addVehicle(Vehicle v, char direction, int posx, int posy, 
				   int vitesse, char alignement, char type, char etat)
{
	NodeVehicleElement * vElement = newVehicle(direction, posx, posy, 
									vitesse, alignement, type, etat);
	if(isEmpty(v))
		return vElement;

	NodeVehicleElement * temp;
	temp = v;
	while(temp->next != NULL)
		temp = temp->next;

	temp->next = vElement;

	return v;
}

Vehicle pollFirstVehicle(Vehicle v, Vehicle * vPoll)
{
	if(isEmpty(v))
		return v;

	NodeVehicleElement * vElement = malloc(sizeof(*vElement));
	if(vElement == NULL)
	{
		fprintf(stderr, "Unable to allocate memory\n");
		exit(EXIT_FAILURE);
	}

	vElement = v->next;

	(*vPoll)->next = NULL;
	(*vPoll)->currentDirection = v->currentDirection;
	(*vPoll)->previousDirection = v->previousDirection;
	(*vPoll)->posx = v->posx;
	(*vPoll)->posy = v->posy;
	(*vPoll)->vitesse = v->vitesse;
	(*vPoll)->alignement = v->alignement;
	(*vPoll)->type = v->type;
	(*vPoll)->etat = v->etat;
	(*vPoll)->temps = v->temps;

	free(v);
	v = NULL;

	return vElement;
}

bool isEmpty(Vehicle v)
{
	return v == NULL ? true : false;
}

void changeVehicleDirection(Vehicle * v, SDL_Keycode key)
{
	(*v)->previousDirection = (*v)->currentDirection;
	switch(key)
	{
		case SDLK_UP:
			if((*v)->previousDirection != SOUTH)
				(*v)->currentDirection = NORTH;
			break;
		case SDLK_DOWN:
			if((*v)->previousDirection != NORTH)
				(*v)->currentDirection = SOUTH;
			break;
		case SDLK_LEFT:
			if((*v)->previousDirection != EAST)
				(*v)->currentDirection = WEST;
			break;
		case SDLK_RIGHT:
			if((*v)->previousDirection != WEST)
				(*v)->currentDirection = EAST;
			break;
	}
}

void changeVehiclePosition(Vehicle * v, SDL_Keycode key)
{
	switch(key)
	{
		case SDLK_UP:
				(*v)->posy -= (*v)->vitesse;
			break;
		case SDLK_DOWN:
				(*v)->posy += (*v)->vitesse;
			break;
		case SDLK_LEFT:
				(*v)->posx -= (*v)->vitesse;
			break;
		case SDLK_RIGHT:
				(*v)->posx += (*v)->vitesse;
			break;
	}
}

void moveVehicle(Vehicle * v)
{
	//Rentre dans le Parking
	if(isBetween((*v)->posx - 5, (*v)->posy, FIRST_INT_X, ENTER_POINT_X, FIRST_INT_Y, ENTER_POINT_Y))
	{
		changeVehicleDirection(v, SDLK_LEFT);
		changeVehiclePosition(v, SDLK_LEFT);
	}

	//Choix intersection 1
	if((*v)->posx == FIRST_INT_X && (*v)->posy == FIRST_INT_Y)
	{
		if(rand() % 2 == 0)
		{
			changeVehicleDirection(v, SDLK_LEFT);
			changeVehiclePosition(v, SDLK_LEFT);
			changeVehicleDirection(v, SDLK_LEFT);
			changeVehiclePosition(v, SDLK_LEFT);
		}

		else
		{
			changeVehicleDirection(v, SDLK_UP);
			changeVehiclePosition(v, SDLK_UP);
			changeVehicleDirection(v, SDLK_UP);
			changeVehiclePosition(v, SDLK_UP);
		}
	}

	//Choix intersection place 1
	if((*v)->posx == FIRST_PP_X && (*v)->posy == FIRST_PP_Y)
	{
		if(rand() % 2 == 0)
		{
			changeVehicleDirection(v, SDLK_LEFT);
			changeVehiclePosition(v, SDLK_LEFT);
			changeVehicleDirection(v, SDLK_LEFT);
			changeVehiclePosition(v, SDLK_LEFT);	
		}

		else
		{
			changeVehicleDirection(v, SDLK_UP);
			changeVehiclePosition(v, SDLK_UP);
		}
	}

	//Avance jusqu'à la deuxieme intersection
	else if(isBetween((*v)->posx, (*v)->posy, SEC_INT_X, FIRST_INT_X, SEC_INT_Y, FIRST_INT_Y))
	{
		changeVehicleDirection(v, SDLK_LEFT);
		changeVehiclePosition(v, SDLK_LEFT);
	}

	//Avance jusqu'à l'intersection avec la première place
	else if(isBetween((*v)->posx, (*v)->posy, FIRST_PP_X, FIRST_INT_X, FIRST_PP_Y, FIRST_INT_Y))
	{
		changeVehicleDirection(v, SDLK_UP);
		changeVehiclePosition(v, SDLK_UP);
	}

	//Se gare à la première place
	else if(isBetween((*v)->posx, (*v)->posy, FIRST_PLACE_X, FIRST_PP_X, FIRST_PLACE_Y, FIRST_PP_Y))
	{
		changeVehicleDirection(v, SDLK_LEFT);
		changeVehiclePosition(v, SDLK_LEFT);
	}
}

bool isAnObstacles(SDL_Rect * obstacles, int size, int x, int y)
{
	return isInRegion(x, y, obstacles[0].x, obstacles[size - 1].x + obstacles[size - 1].w, 
							obstacles[0].y, obstacles[size - 1].y + obstacles[size - 1].h);
}

bool isInRegion(int x, int y, int xInf, int xSup, int yInf, int ySup)
{
	return ((x >= xInf && x <= xSup) && (y >= yInf && y <= ySup));
}

bool isBetween(int x, int y, int x2, int x3, int y2, int y3)
{
	return (x >= x2 && x <= x3) && (y >= y2 && y <= y3);
}