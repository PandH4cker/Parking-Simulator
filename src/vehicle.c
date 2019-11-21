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
	return v == NULL;
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
	if(!v) return;

	//Rentre dans le Parking
	if(isBetween((*v)->posx - 5, (*v)->posy, FIRST_INT_X, ENTER_POINT_X + 9500, FIRST_INT_Y, ENTER_POINT_Y))
	{
		changeVehicleDirection(v, SDLK_LEFT);
		changeVehiclePosition(v, SDLK_LEFT);
	}

	//for(int pp_y = FIRST_PP_Y; pp_y > DELIMITER_PP; pp_y -= 70)

	for(int int_x = FIRST_INT_X; int_x > DELIMITER_INT; int_x -= 535)
	{
		if((*v)->posx == int_x && (*v)->posy == FIRST_INT_Y)
		{
			// Continue jusque l'intersection 2
			/*if(rand() % 2 == 0)
			{
				changeVehicleDirection(v, SDLK_LEFT);
				changeVehiclePosition(v, SDLK_LEFT);
				changeVehicleDirection(v, SDLK_LEFT);
				changeVehiclePosition(v, SDLK_LEFT);
			}

			// Continue jusque la place 1
			else
			{*/
				changeVehicleDirection(v, SDLK_UP);
				changeVehiclePosition(v, SDLK_UP);
				changeVehicleDirection(v, SDLK_UP);
				changeVehiclePosition(v, SDLK_UP);
			//}			
		}
	}

	for(int pp_y = FIRST_PP_Y; pp_y > DELIMITER_PP; pp_y -= 70)
	{
		if((*v)->posx == FIRST_PP_X && (*v)->posy == pp_y)
		{
			//Se gare à la place 1
			if(rand() % 2 == 0)
			{
				changeVehicleDirection(v, SDLK_LEFT);
				changeVehiclePosition(v, SDLK_LEFT);
				changeVehicleDirection(v, SDLK_LEFT);
				changeVehiclePosition(v, SDLK_LEFT);	
			}

			// Continue jusque la place 2
			else
			{
				changeVehicleDirection(v, SDLK_UP);
				changeVehiclePosition(v, SDLK_UP);
				changeVehicleDirection(v, SDLK_UP);
				changeVehiclePosition(v, SDLK_UP);
				changeVehicleDirection(v, SDLK_UP);
				changeVehiclePosition(v, SDLK_UP);
			}
		}	
	}

	//for(int place_y = FIRST_PLACE_Y; place_y >  DELIMITER_PLACE; place_y -= 70)

	//Choix intersection 1
	/*if((*v)->posx == FIRST_INT_X && (*v)->posy == FIRST_INT_Y)
	{
		// Continue jusque l'intersection 2
		if(rand() % 2 == 0)
		{
			changeVehicleDirection(v, SDLK_LEFT);
			changeVehiclePosition(v, SDLK_LEFT);
			changeVehicleDirection(v, SDLK_LEFT);
			changeVehiclePosition(v, SDLK_LEFT);
		}

		// Continue jusque la place 1
		else
		{
			changeVehicleDirection(v, SDLK_UP);
			changeVehiclePosition(v, SDLK_UP);
			changeVehicleDirection(v, SDLK_UP);
			changeVehiclePosition(v, SDLK_UP);
		}
	}*/

	//Choix intersection place 1
	/*if((*v)->posx == FIRST_PP_X && (*v)->posy == FIRST_PP_Y)
	{
		// Se gare à la place 1
		if(rand() % 2 == 0)
		{
			changeVehicleDirection(v, SDLK_LEFT);
			changeVehiclePosition(v, SDLK_LEFT);
			changeVehicleDirection(v, SDLK_LEFT);
			changeVehiclePosition(v, SDLK_LEFT);	
		}

		// Continue jusque la place 2
		else
		{
			changeVehicleDirection(v, SDLK_UP);
			changeVehiclePosition(v, SDLK_UP);
			changeVehicleDirection(v, SDLK_UP);
			changeVehiclePosition(v, SDLK_UP);
		}
	}*/

	//Choix intersection place 2
	/*if((*v)->posx == SEC_PP_X && (*v)->posy == SEC_PP_Y)
	{
		changeVehicleDirection(v, SDLK_LEFT);
		changeVehiclePosition(v, SDLK_LEFT);
		changeVehicleDirection(v, SDLK_LEFT);
		changeVehiclePosition(v, SDLK_LEFT);
	}*/

	//Avance jusqu'à la deuxieme intersection
	/*if(isBetween((*v)->posx, (*v)->posy, SEC_INT_X, FIRST_INT_X, SEC_INT_Y, FIRST_INT_Y))
	{
		changeVehicleDirection(v, SDLK_LEFT);
		changeVehiclePosition(v, SDLK_LEFT);
	}*/

	//Avance jusqu'à l'intersection avec la première place
	if(isBetween((*v)->posx, (*v)->posy, FIRST_PP_X, FIRST_INT_X, FIRST_PP_Y, FIRST_INT_Y))
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

	//Avance jusqu'à l'intersection avec la deuxième place
	else if(isBetween((*v)->posx, (*v)->posy, SEC_PP_X, FIRST_PP_X, SEC_PP_Y, FIRST_PP_Y))
	{
		changeVehicleDirection(v, SDLK_UP);
		changeVehiclePosition(v, SDLK_UP);
	}

	else if(isBetween((*v)->posx, (*v)->posy, SEC_PLACE_X, SEC_PP_X, SEC_PLACE_Y, SEC_PP_Y))
	{
		changeVehicleDirection(v, SDLK_LEFT);
		changeVehiclePosition(v, SDLK_LEFT);
	}

	else if(isBetween((*v)->posx, (*v)->posy, THIR_PP_X, SEC_PP_X, THIR_PP_Y, SEC_PP_Y))
	{
		changeVehicleDirection(v, SDLK_UP);
		changeVehiclePosition(v, SDLK_UP);
	}

	else if(isBetween((*v)->posx, (*v)->posy, THIR_PLACE_X, THIR_PP_X, THIR_PLACE_Y, THIR_PP_Y))
	{
		changeVehicleDirection(v, SDLK_LEFT);
		changeVehiclePosition(v, SDLK_LEFT);
	}

	else if(isBetween((*v)->posx, (*v)->posy, THIR_PP_X, SEC_PP_X, THIR_PP_Y, SEC_PP_Y))
	{
		changeVehicleDirection(v, SDLK_UP);
		changeVehiclePosition(v, SDLK_UP);
	}

	else if(isBetween((*v)->posx, (*v)->posy, THIR_PLACE_X, THIR_PP_X, THIR_PLACE_Y, THIR_PP_Y))
	{
		changeVehicleDirection(v, SDLK_LEFT);
		changeVehiclePosition(v, SDLK_LEFT);
	}

	else if(isBetween((*v)->posx, (*v)->posy, FOUR_PP_X, THIR_PP_X, FOUR_PP_Y, THIR_PP_Y))
	{
		changeVehicleDirection(v, SDLK_UP);
		changeVehiclePosition(v, SDLK_UP);
	}

	else if(isBetween((*v)->posx, (*v)->posy, FOUR_PLACE_X, FOUR_PP_X, FOUR_PLACE_Y, FOUR_PP_Y))
	{
		changeVehicleDirection(v, SDLK_LEFT);
		changeVehiclePosition(v, SDLK_LEFT);
	}

	else if(isBetween((*v)->posx, (*v)->posy, FIF_PP_X, FOUR_PP_X, FIF_PP_Y, FOUR_PP_Y))
	{
		changeVehicleDirection(v, SDLK_UP);
		changeVehiclePosition(v, SDLK_UP);
	}

	else if(isBetween((*v)->posx, (*v)->posy, FIF_PLACE_X, FIF_PP_X, FIF_PLACE_Y, FIF_PP_Y))
	{
		changeVehicleDirection(v, SDLK_LEFT);
		changeVehiclePosition(v, SDLK_LEFT);
	}

	else if(isBetween((*v)->posx, (*v)->posy, SIX_PP_X, FIF_PP_X, SIX_PP_Y, FIF_PP_Y))
	{
		changeVehicleDirection(v, SDLK_UP);
		changeVehiclePosition(v, SDLK_UP);
	}

	else if(isBetween((*v)->posx, (*v)->posy, SIX_PLACE_X, SIX_PP_X, SIX_PLACE_Y, SIX_PP_Y))
	{
		changeVehicleDirection(v, SDLK_LEFT);
		changeVehiclePosition(v, SDLK_LEFT);
	}

	for(int int_x = FIRST_INT_X; int_x > DELIMITER_INT; int_x -= 535)
	{
		if(isBetween((*v)->posx, (*v)->posy, int_x - 535, int_x, FIRST_INT_Y, FIRST_INT_Y))
		{
			changeVehicleDirection(v, SDLK_LEFT);
			changeVehiclePosition(v, SDLK_LEFT);
		}
	}

	if((*v)->next)
		moveVehicle(&(*v)->next);
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

Vehicle newVehicleList(void)
{
	return NULL;
}

void freeVehicleList(Vehicle v)
{
	if(!v) return;
	if(v->next) freeVehicleList(v->next);
	free(v);
}