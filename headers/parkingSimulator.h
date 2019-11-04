#ifndef PARKINGSIMULATOR_H
#define PARKINGSIMULATOR_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "vehicle.h"
#include "const.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define PIXELIZED_MAP_X 1920
#define PIXELIZED_MAP_Y 805

#define MAP_CELL_X 26
#define MAP_CELL_Y 11

#define BUTTON_WIDTH 374
#define BUTTON_HEIGHT 135

#define BLOCK_WIDTH 64
#define BLOCK_HEIGHT 64

#define FPS 50
#define DELAY_TIME 1000.0f/FPS

#define WHITE {255, 255, 255, 0xFF}

int SDLnIMGnTTFInitialize();
SDL_Window * createWindow(const char * windowName, int posx, int posy, int width, int height, Uint32 flags);
SDL_Renderer * createRenderer(SDL_Window * window, int index, Uint32 flags);
SDL_Texture * loadTexture(const char * file, SDL_Renderer * renderer);
SDL_Texture * renderText(const char * text, const char * fontName, SDL_Color color, int fontSize, SDL_Renderer * renderer);
void renderTextureClip(SDL_Texture * texture, SDL_Renderer * renderer, SDL_Rect dest, SDL_Rect * clip);
void renderTexture(SDL_Texture * texture, SDL_Renderer * renderer, int x, int y, SDL_Rect * clip);
void cleanUp(SDL_Window * window, SDL_Renderer * renderer);
void updateEvents(SDL_Event * event, bool * fluideSelect, bool * chargeSelect, bool * exitSelect);
void handleMenuButtonsEvent(SDL_Event * event, bool * fluideSelect, bool * chargeSelect, bool * exitSelect);
void displayMenu(SDL_Renderer * renderer, SDL_Texture * background, SDL_Texture * textParkingSimulator, SDL_Texture * fluideButton, SDL_Texture * chargeButton, SDL_Texture * exitButton);
void displayCar(Vehicle v, SDL_Texture * carAllTexture, SDL_Renderer * renderer, SDL_Rect carClips[CAR_CLIPS]);
int simulate();

#endif