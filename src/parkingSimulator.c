#include "../headers/parkingSimulator.h"

int SDLnIMGnTTFInitialize()
{
	int flags = IMG_INIT_PNG;
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

bool isInRegion(int x, int y, int xInf, int xSup, int yInf, int ySup)
{
	if((x >= xInf && x <= xSup) && (y >= yInf && y <= ySup))
		return true;
	return false;
}

void cleanUp(SDL_Window * window, SDL_Renderer * renderer)
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
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

int simulate()
{
	const char * fontPath = "../Fonts/colour_brush/colour.ttf";
	if(SDLnIMGnTTFInitialize() == EXIT_FAILURE)
		return EXIT_FAILURE;

	SDL_Window * window = createWindow("Parking Simulator", SDL_WINDOWPOS_CENTERED, 
										SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, 
										SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	SDL_Renderer * renderer = createRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture * background = loadTexture("../Images/Backgrounds/MainMenu.png", renderer);
	if(!background)
	{
		cleanUp(window, renderer);
		return EXIT_FAILURE;
	}

	SDL_Color white = WHITE;
	SDL_Texture * textParkingSimulator = renderText("Parking Simulator", fontPath, white, 140, renderer);
	if(!textParkingSimulator)
	{
		SDL_DestroyTexture(background);
		cleanUp(window, renderer);
		return EXIT_FAILURE;
	}
	int xTextParkingSimulator = 450, yTextParkingSimulator = 270;

	SDL_Texture * fluideButton = loadTexture("../Images/Backgrounds/FluideButton.png", renderer);
	if(!fluideButton)
	{
		SDL_DestroyTexture(textParkingSimulator);
		SDL_DestroyTexture(background);
		cleanUp(window, renderer);
		return EXIT_FAILURE;
	}
	int xFluideButton = 765, yFluideButton = 550;

	SDL_Texture * chargeButton = loadTexture("../Images/Backgrounds/ChargeButton.png", renderer);
	if(!chargeButton)
	{
		SDL_DestroyTexture(fluideButton);
		SDL_DestroyTexture(textParkingSimulator);
		SDL_DestroyTexture(background);
		cleanUp(window, renderer);
		return EXIT_FAILURE;
	}
	int xChargeButton = 765, yChargeButton = 655;

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
	int xExitButton = 765, yExitButton = 760;

	SDL_Texture * mapTexture = loadTexture("../Images/Photoshop/terrain.png", renderer);
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

	bool fluideSelect = false;
	bool chargeSelect = false;
	bool exitSelect = false;

	Uint32 frameStart, frameTime;
	while(!SDL_QuitRequested())
	{
		frameStart = SDL_GetTicks();
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_MOUSEBUTTONDOWN:
					if(event.button.button == SDL_BUTTON_LEFT)
					{
						if(!fluideSelect && !chargeSelect && !exitSelect)
						{
							if(isInRegion(event.button.x, event.button.y,
														xExitButton, xExitButton + BUTTON_WIDTH,
														yExitButton, yExitButton + BUTTON_HEIGHT))
							{
								fluideSelect = false;
								chargeSelect = false;
								exitSelect = true;
							}

							if(isInRegion(event.button.x, event.button.y,
														xFluideButton, xFluideButton + BUTTON_WIDTH,
														yFluideButton, yFluideButton + BUTTON_HEIGHT))
							{
								fluideSelect = true;
								chargeSelect = false;
								exitSelect = false;
							}

							if(isInRegion(event.button.x, event.button.y,
														xChargeButton, xChargeButton + BUTTON_WIDTH,
														yChargeButton, yChargeButton + BUTTON_HEIGHT))
							{
								fluideSelect = false;
								chargeSelect = true;
								exitSelect = false;
							}
						}
					}
					break;

				case SDL_MOUSEBUTTONUP:
					break;

				case SDL_MOUSEMOTION:
					break;

				case SDL_KEYDOWN:
					break;
			}
		}

		if(exitSelect) goto destroyer;

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);

		if(!fluideSelect && !chargeSelect)
		{
			renderTexture(background, renderer, 0, 0, NULL);
			renderTexture(textParkingSimulator, renderer, xTextParkingSimulator, yTextParkingSimulator, NULL);
			renderTexture(fluideButton, renderer, xFluideButton, yFluideButton, NULL);
			renderTexture(chargeButton, renderer, xChargeButton, yChargeButton, NULL);
			renderTexture(exitButton, renderer, xExitButton, yExitButton, NULL);
		}

		else
		{
			renderTexture(mapTexture, renderer, 0, 0, NULL);
		}

		SDL_RenderPresent(renderer);

		frameTime = SDL_GetTicks() - frameStart;
		if(frameTime < DELAY_TIME)
			SDL_Delay((int)(DELAY_TIME - frameTime));
	}


	destroyer:
	SDL_DestroyTexture(mapTexture);
	SDL_DestroyTexture(exitButton);
	SDL_DestroyTexture(chargeButton);
	SDL_DestroyTexture(fluideButton);
	SDL_DestroyTexture(textParkingSimulator);
	SDL_DestroyTexture(background);
	cleanUp(window, renderer);
	return EXIT_SUCCESS;
}