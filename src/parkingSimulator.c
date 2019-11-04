#include "../headers/parkingSimulator.h"

int SDLnIMGnTTFInitialize()
{
	srand(time(NULL));
	int flags = IMG_INIT_PNG | IMG_INIT_JPG;
	int initted = IMG_Init(flags);

	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	if((initted & flags) != flags)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize png images: %s\n", IMG_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	if(TTF_Init() == -1)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize font feature: %s\n", TTF_GetError());
		IMG_Quit();
		SDL_Quit();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void cleanUp(SDL_Window * window, SDL_Renderer * renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture * loadTexture(const char * filename, SDL_Renderer * renderer)
{
	SDL_Texture * texture = IMG_LoadTexture(renderer, filename);
	if(!texture)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture: %s\n", IMG_GetError());
		return NULL;
	}

	return texture;
}

SDL_Texture * renderText(const char * text, const char * fontName, SDL_Color color, int fontSize, SDL_Renderer * renderer)
{
	TTF_Font * font = TTF_OpenFont(fontName, fontSize);
	if(!font)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to open font: %s\n", TTF_GetError());
		return NULL;
	}

	SDL_Surface * surface = TTF_RenderText_Blended(font, text, color);
	if(!surface)
	{
		TTF_CloseFont(font);
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to apply text on surface: %s\n", TTF_GetError());
		return NULL;
	}

	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
	if(!texture)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture: %s\n", SDL_GetError());

	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
	return texture;
}

void renderTexture(SDL_Texture * texture, SDL_Renderer * renderer, int x, int y, SDL_Rect * clip)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	if(!clip)
		SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	else
	{
		dest.w = clip->w;
		dest.h = clip->h;
	}
	renderTextureClip(texture, renderer, dest, clip);
}

void renderTextureClip(SDL_Texture * texture, SDL_Renderer * renderer, SDL_Rect dest, SDL_Rect * clip)
{
	SDL_RenderCopy(renderer, texture, clip, &dest);
}

SDL_Window * createWindow(const char * windowName, int posx, int posy, int width, int height, Uint32 flags)
{
	SDL_Window * window = SDL_CreateWindow(windowName, posx, posy, width, height, flags);
	if(!window)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create window: %s\n", SDL_GetError());
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return NULL;
	}
	return window;
}

SDL_Renderer * createRenderer(SDL_Window * window, int index, Uint32 flags)
{
	SDL_Renderer * renderer = SDL_CreateRenderer(window, index, flags);
	if(!renderer)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return NULL;
	}
	return renderer;
}

void handleMenuButtonsEvent(SDL_Event * event, bool * fluideSelect, bool * chargeSelect, bool * exitSelect)
{
	if(!*fluideSelect && !*chargeSelect && !*exitSelect)
	{
		if(isInRegion(event->button.x, event->button.y,
					  EXIT_BUTTON_X, EXIT_BUTTON_X + BUTTON_WIDTH,
					  EXIT_BUTTON_Y, EXIT_BUTTON_Y + BUTTON_HEIGHT))
		{
			*fluideSelect = false;
			*chargeSelect = false;
			*exitSelect = true;
		}
		if(isInRegion(event->button.x, event->button.y,
					  FLUIDE_BUTTON_X, FLUIDE_BUTTON_X + BUTTON_WIDTH,
					  FLUIDE_BUTTON_Y, FLUIDE_BUTTON_Y + BUTTON_HEIGHT))
		{
			*fluideSelect = true;
			*chargeSelect = false;
			*exitSelect = false;
		}
		if(isInRegion(event->button.x, event->button.y,
					  CHARGE_BUTTON_X, CHARGE_BUTTON_X + BUTTON_WIDTH,
					  CHARGE_BUTTON_Y, CHARGE_BUTTON_Y + BUTTON_HEIGHT))
		{
			*fluideSelect = false;
			*chargeSelect = true;
			*exitSelect = false;
		}
	}	
}

void updateEvents(SDL_Event * event, bool * fluideSelect, bool * chargeSelect, bool * exitSelect)
{
	while(SDL_PollEvent(event))
	{
		switch(event->type)
		{
			case SDL_MOUSEBUTTONDOWN:
				if(event->button.button == SDL_BUTTON_LEFT)
					handleMenuButtonsEvent(event, fluideSelect, chargeSelect, exitSelect);
				break;

			case SDL_MOUSEBUTTONUP:
				break;

			case SDL_MOUSEMOTION:
				break;

			case SDL_KEYDOWN:
				/*if(*fluideSelect)
					moveVehicle(v, event->key.keysym.sym, r, size);*/
				break;
		}
	}	
}

void displayMenu(SDL_Renderer * renderer, SDL_Texture * background, SDL_Texture * textParkingSimulator, SDL_Texture * fluideButton, SDL_Texture * chargeButton, SDL_Texture * exitButton)
{
	renderTexture(background, renderer, 0, 0, NULL);
	renderTexture(textParkingSimulator, renderer, TEXT_PARKING_SIMULATOR_X, TEXT_PARKING_SIMULATOR_Y, NULL);
	renderTexture(fluideButton, renderer, FLUIDE_BUTTON_X, FLUIDE_BUTTON_Y, NULL);
	renderTexture(chargeButton, renderer, CHARGE_BUTTON_X, CHARGE_BUTTON_Y, NULL);
	renderTexture(exitButton, renderer, EXIT_BUTTON_X, EXIT_BUTTON_Y, NULL);
}

void displayCar(Vehicle v, SDL_Texture * carAllTexture, SDL_Renderer * renderer, SDL_Rect carClips[CAR_CLIPS])
{
	if(!v) return;
	if(v->currentDirection != v->previousDirection)
		switch(v->previousDirection)
		{
			case NORTH:
				if(v->currentDirection == EAST)
					renderTexture(carAllTexture, renderer, v->posx, v->posy, &carClips[1]);
				else
					renderTexture(carAllTexture, renderer, v->posx, v->posy, &carClips[7]);
				break;
			case SOUTH:
				if(v->currentDirection == EAST)
					renderTexture(carAllTexture, renderer, v->posx, v->posy, &carClips[3]);
				else
					renderTexture(carAllTexture, renderer, v->posx, v->posy, &carClips[5]);
				break;
			case EAST:
				if(v->currentDirection == NORTH)
					renderTexture(carAllTexture, renderer, v->posx, v->posy, &carClips[1]);
				else
					renderTexture(carAllTexture, renderer, v->posx, v->posy, &carClips[3]);
				break;
			case WEST:
				if(v->currentDirection == NORTH)
					renderTexture(carAllTexture, renderer, v->posx, v->posy, &carClips[7]);
				else
					renderTexture(carAllTexture, renderer, v->posx, v->posy, &carClips[5]);
				break;
		}
	else
		switch(v->currentDirection)
		{
			case NORTH:
				renderTexture(carAllTexture, renderer, v->posx, v->posy, &carClips[0]);
				break;
			case SOUTH:
				renderTexture(carAllTexture, renderer, v->posx, v->posy, &carClips[4]);
				break;
			case EAST:
				renderTexture(carAllTexture, renderer, v->posx, v->posy, &carClips[2]);
				break;
			case WEST:
				renderTexture(carAllTexture, renderer, v->posx, v->posy, &carClips[6]);
				break;
		}

	if(v->next) displayCar(v->next, carAllTexture, renderer, carClips);
}


int simulate()
{
	if(SDLnIMGnTTFInitialize() == EXIT_FAILURE)
		return EXIT_FAILURE;

	SDL_Window * window = createWindow("Parking Simulator", SDL_WINDOWPOS_CENTERED, 
										SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, 
										SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	SDL_Renderer * renderer = createRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_Texture * background = loadTexture("../Images/Backgrounds/MainMenu.png", renderer);
	if(!background)
	{
		cleanUp(window, renderer);
		return EXIT_FAILURE;
	}

	SDL_Color white = WHITE;
	SDL_Texture * textParkingSimulator = renderText("Parking Simulator", FONT_PATH, white, 140, renderer);
	if(!textParkingSimulator)
	{
		SDL_DestroyTexture(background);
		cleanUp(window, renderer);
		return EXIT_FAILURE;
	}

	SDL_Texture * fluideButton = loadTexture("../Images/Backgrounds/FluideButton.png", renderer);
	if(!fluideButton)
	{
		SDL_DestroyTexture(textParkingSimulator);
		SDL_DestroyTexture(background);
		cleanUp(window, renderer);
		return EXIT_FAILURE;
	}

	SDL_Texture * chargeButton = loadTexture("../Images/Backgrounds/ChargeButton.png", renderer);
	if(!chargeButton)
	{
		SDL_DestroyTexture(fluideButton);
		SDL_DestroyTexture(textParkingSimulator);
		SDL_DestroyTexture(background);
		cleanUp(window, renderer);
		return EXIT_FAILURE;
	}

	SDL_Texture * exitButton = loadTexture("../Images/Backgrounds/exitButton.png", renderer);
	if(!exitButton)
	{
		SDL_DestroyTexture(chargeButton);
		SDL_DestroyTexture(fluideButton);
		SDL_DestroyTexture(textParkingSimulator);
		SDL_DestroyTexture(background);
		cleanUp(window, renderer);
		return EXIT_FAILURE;
	}

	SDL_Texture * mapTexture = loadTexture("../Images/terrain2.png", renderer);
	if(!mapTexture)
	{
		SDL_DestroyTexture(exitButton);
		SDL_DestroyTexture(chargeButton);
		SDL_DestroyTexture(fluideButton);
		SDL_DestroyTexture(textParkingSimulator);
		SDL_DestroyTexture(background);
		cleanUp(window, renderer);
		return EXIT_FAILURE;
	}

	SDL_Texture * carAllTexture = loadTexture("../Images/Cars/carAll_red.png", renderer);
	if(!carAllTexture)
	{
		SDL_DestroyTexture(mapTexture);
		SDL_DestroyTexture(chargeButton);
		SDL_DestroyTexture(fluideButton);
		SDL_DestroyTexture(textParkingSimulator);
		SDL_DestroyTexture(background);
		cleanUp(window, renderer);
		return EXIT_FAILURE;
	}
	SDL_Rect carClips[CAR_CLIPS];
	for(int i = 0; i < CAR_CLIPS; ++i)
	{
		carClips[i].x = i * CAR_WIDTH;
		carClips[i].y = 0;
		carClips[i].w = CAR_WIDTH;
		carClips[i].h = CAR_HEIGHT;
	}

	/*SDL_Rect leftTopWallArea[NUMB_LEFT_TOP_WALL];
	for(int i = 1; i <= NUMB_LEFT_TOP_WALL; ++i)
	{
		leftTopWallArea[i - 1].x = i * FIRST_LEFT_TOP_WALL_X + (i - 1) * BLOCK_WIDTH;
		leftTopWallArea[i - 1].y = FIRST_LEFT_TOP_WALL_Y;
		leftTopWallArea[i - 1].w = BLOCK_WIDTH;
		leftTopWallArea[i - 1].h = BLOCK_HEIGHT;
	}

	SDL_Rect leftTopWallShiftArea[NUMB_LEFT_TOP_WALL_SHIFT];
	for(int i = 1; i <= NUMB_LEFT_TOP_WALL_SHIFT; ++i)
	{
		leftTopWallShiftArea[i - 1].x = FIRST_LEFT_TOP_WALL_X;
		leftTopWallShiftArea[i - 1].y = i * FIRST_LEFT_TOP_WALL_Y + (i - 1) * BLOCK_HEIGHT;
		leftTopWallShiftArea[i - 1].w = BLOCK_WIDTH;
		leftTopWallShiftArea[i - 1].h = BLOCK_HEIGHT;
	}

	SDL_Rect leftBottomWallShiftArea[NUMB_LEFT_BOTTOM_WALL_SHIFT];
	for(int i = 1; i <= NUMB_LEFT_BOTTOM_WALL_SHIFT; ++i)
	{
		leftBottomWallShiftArea[i - 1].x = FIRST_LEFT_BOTTOM_WALL_SHIFT_X;
		leftBottomWallShiftArea[i - 1].y = i * FIRST_LEFT_BOTTOM_WALL_SHIFT_Y + (i - 1) * BLOCK_HEIGHT;
		leftBottomWallShiftArea[i - 1].w = BLOCK_WIDTH;
		leftBottomWallShiftArea[i - 1].h = BLOCK_HEIGHT;
	}

	SDL_Rect leftBottomWallArea[NUMB_LEFT_BOTTOM_WALL];
	for(int i = 1; i <= NUMB_LEFT_BOTTOM_WALL; ++i)
	{
		leftBottomWallArea[i - 1].x = i * FIRST_LEFT_BOTTOM_WALL_X + (i - 1) * BLOCK_WIDTH;
		leftBottomWallArea[i - 1].y = FIRST_LEFT_BOTTOM_WALL_Y;
		leftBottomWallArea[i - 1].w = BLOCK_WIDTH;
		leftBottomWallArea[i - 1].h = BLOCK_HEIGHT;
	}

	SDL_Rect rightTopWallShiftArea[NUMB_RIGHT_TOP_WALL_SHIFT];
	for(int i = 1; i <= NUMB_RIGHT_TOP_WALL_SHIFT; ++i)
	{
		rightTopWallShiftArea[i - 1].x = FIRST_RIGHT_TOP_WALL_X;
		rightTopWallShiftArea[i - 1].y = i * FIRST_RIGHT_TOP_WALL_Y + (i - 1) * BLOCK_HEIGHT;
		rightTopWallShiftArea[i - 1].w = BLOCK_WIDTH;
		rightTopWallShiftArea[i - 1].h = BLOCK_HEIGHT;
	}

	SDL_Rect rightBottomWallShiftArea[NUMB_RIGHT_BOTTOM_WALL_SHIFT];
	for(int i = 1; i <= NUMB_RIGHT_BOTTOM_WALL_SHIFT; ++i)
	{
		rightBottomWallShiftArea[i - 1].x = FIRST_RIGHT_BOTTOM_WALL_X;
		rightBottomWallShiftArea[i - 1].y = i * FIRST_RIGHT_BOTTOM_WALL_Y + (i - 1) * BLOCK_HEIGHT;
		rightBottomWallShiftArea[i - 1].w = BLOCK_WIDTH;
		rightBottomWallShiftArea[i - 1].h = BLOCK_HEIGHT;
	}*/

	bool fluideSelect = false;
	bool chargeSelect = false;
	bool exitSelect = false;

	Vehicle v = newVehicleList();
	v = addVehicle(v, NORTH, ENTER_POINT_X, ENTER_POINT_Y, 5, LEFT, CAR, ACTIVE);
	v = addVehicle(v, NORTH, ENTER_POINT_X + 500, ENTER_POINT_Y, 5, LEFT, CAR, ACTIVE);

	Uint32 frameStart, frameTime;
	while(!SDL_QuitRequested())
	{
		frameStart = SDL_GetTicks();
		SDL_Event event;
		updateEvents(&event, &fluideSelect, &chargeSelect, &exitSelect);
		if(fluideSelect)
			moveVehicle(&v);

		printf("(%d, %d)\n", v->posx, v->posy);

		if(exitSelect) goto destroyer;

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);

		if(!fluideSelect && !chargeSelect)
			displayMenu(renderer, background, textParkingSimulator, fluideButton, chargeButton, exitButton);

		else
		{
			renderTexture(mapTexture, renderer, 0, 0, NULL);
			displayCar(v, carAllTexture, renderer, carClips);		
		}

		SDL_RenderPresent(renderer);

		frameTime = SDL_GetTicks() - frameStart;
		if(frameTime < DELAY_TIME)
			SDL_Delay((int)(DELAY_TIME - frameTime));
	}


	destroyer:
		SDL_DestroyTexture(carAllTexture);
		SDL_DestroyTexture(mapTexture);
		SDL_DestroyTexture(exitButton);
		SDL_DestroyTexture(chargeButton);
		SDL_DestroyTexture(fluideButton);
		SDL_DestroyTexture(textParkingSimulator);
		SDL_DestroyTexture(background);
		cleanUp(window, renderer);
		freeVehicleList(v);
		return EXIT_SUCCESS;
}