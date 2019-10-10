#include "../headers/vehicle.h"

Vehicle newVehicle(char direction, int posx, int posy, 
				   int vitesse, char alignement, char type, char etat)
{
	NodeVehicleElement * v = malloc(sizeof(Vehicle));
	if(!v)
	{
		fprintf(stderr, "Unable to allocate the node, out of memory\n");
		exit(EXIT_FAILURE);
	}

	v->direction = direction;
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
	(*vPoll)->direction = v->direction;
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