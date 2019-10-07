#ifndef PARKINGSIMULATOR_H
#define PARKINGSIMULATOR_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "vehicle.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define BUTTON_WIDTH 374
#define BUTTON_HEIGHT 135
#define BLOCK_WIDTH 64
#define BLOCK_HEIGHT 64
#define FPS 25
#define DELAY_TIME 1000.0f/FPS

#define WHITE {255, 255, 255, 0xFF}

int SDLnIMGnTTFInitialize();
bool isInRegion(int x, int y, int xInf, int xSup, int yInf, int ySup);
SDL_Window * createWindow(const char * windowName, int posx, int posy, int width, int height, Uint32 flags);
SDL_Renderer * createRenderer(SDL_Window * window, int index, Uint32 flags);
SDL_Texture * loadTexture(const char * file, SDL_Renderer * renderer);
SDL_Texture * renderText(const char * text, const char * fontName, SDL_Color color, int fontSize, SDL_Renderer * renderer);
void renderTextureClip(SDL_Texture * texture, SDL_Renderer * renderer, SDL_Rect dest, SDL_Rect * clip);
void renderTexture(SDL_Texture * texture, SDL_Renderer * renderer, int x, int y, SDL_Rect * clip);
void cleanUp(SDL_Window * window, SDL_Renderer * renderer);
int simulate();

#endif