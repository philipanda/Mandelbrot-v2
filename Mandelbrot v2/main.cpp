#include <stdio.h>
#include <stdlib.h>
#include "mandelbrot.h"
#include <SDL.h>
#include <SDL_image.h>

#define double long double

int max(int a, int b)
{
	return a > b ? a : b;
}

unsigned int WIDTH = 620;
unsigned int HEIGHT = 480;


#undef main
int main()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Error initializing\n");
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, renderWidth, renderHeight);


	SDL_Rect dest;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	dest.w = renderWidth;
	dest.h = renderHeight;
	dest.x = 0;
	dest.y = 0;
	bool quit = false;
	int speed = 300;
	double depth = 64;

	bool dirty = true;

	SDL_Event e;
	Complex start(-2, -2);
	Complex end(2, 2);

	Complex delta;
	double moveSpeed = 0.01;

	enum Keys {Right, Left, Up, Down, In, Out, Plus, Minus, ResUp, ResDown};
	int pressed[] = { 0, 0, 0, 0 , 0, 0, 0, 0, 0, 0};
	bool screenshot = false;

	int t = 0;
	int lastFrameTime = 0;
	createTextureArray(renderWidth, renderHeight);
	while(!quit)
	{
		while(SDL_PollEvent(&e))
			switch(e.type)
			{
				case SDL_QUIT:
				{
					quit = true;
					break;
				}
			
				case SDL_KEYDOWN:
				{
					if (e.key.keysym.sym == SDLK_RIGHT)
						pressed[Right] = 1;
					else if (e.key.keysym.sym == SDLK_LEFT)
						pressed[Left] = 1;
					else if (e.key.keysym.sym == SDLK_UP)
						pressed[Up] = 1;
					else if (e.key.keysym.sym == SDLK_DOWN)
						pressed[Down] = 1;
					else if (e.key.keysym.sym == SDLK_w)
						pressed[In] = 1;
					else if (e.key.keysym.sym == SDLK_s)
						pressed[Out] = 1;
					else if (e.key.keysym.sym == SDLK_e)
						pressed[Plus] = 1;
					else if (e.key.keysym.sym == SDLK_d)
						pressed[Minus] = 1;
					else if (e.key.keysym.sym == SDLK_q)
						pressed[ResUp] = 1;
					else if (e.key.keysym.sym == SDLK_a)
						pressed[ResDown] = 1;
					else if (e.key.keysym.sym == SDLK_p)
						screenshot = true;
					break;
				}
				case SDL_KEYUP:
				{
					if (e.key.keysym.sym == SDLK_RIGHT)
						pressed[Right] = 0;
					else if (e.key.keysym.sym == SDLK_LEFT)
						pressed[Left] = 0;
					else if (e.key.keysym.sym == SDLK_UP)
						pressed[Up] = 0;
					else if (e.key.keysym.sym == SDLK_DOWN)
						pressed[Down] = 0;
					else if (e.key.keysym.sym == SDLK_w)
						pressed[In] = 0;
					else if (e.key.keysym.sym == SDLK_s)
						pressed[Out] = 0;
					else if (e.key.keysym.sym == SDLK_e)
						pressed[Plus] = 0;
					else if (e.key.keysym.sym == SDLK_d)
						pressed[Minus] = 0;
					else if (e.key.keysym.sym == SDLK_q)
						pressed[ResUp] = 0;
					else if (e.key.keysym.sym == SDLK_a)
						pressed[ResDown] = 0;
					break;
				}
			}
		
		if (screenshot)
		{
			screenshot = false;
			int mult = 10;
			SDL_Surface* shot = nullptr;
			SDL_Texture* tex = nullptr;
			int w, h;
			do {
				try {
					w = renderWidth * mult;
					h = renderHeight * mult;
					createTextureArray(w, h);
					shot = SDL_CreateRGBSurface(0, w, h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
					tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
				}
				catch(exception e) {
					printf("could not alocate memory, trying smaller size...\n");
				}
				mult--;
			} while (!shot || !tex);
			
			 //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			//SDL_RenderClear(renderer);
				printf("\nTaking highres screenshot, will take approximately %fs\n", lastFrameTime*mult*mult/1000.0);
				drawMandelbrot(tex, start, end, depth);
				printf("Done...\n");
				SDL_SetRenderTarget(renderer, tex);

				SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, shot->pixels, shot->pitch);
				//SDL_SaveBMP(shot, "screenshot.bmp");
				IMG_SaveJPG(shot, "screenshot.jpg", 100);
				SDL_FreeSurface(shot);
				SDL_DestroyTexture(tex);
				createTextureArray(w / mult, h / mult);
			

		}
		if ((pressed[Right] - pressed[Left]) || (pressed[Down] - pressed[Up]) || (pressed[In] - pressed[Out]) || 
			(pressed[Plus] - pressed[Minus]) || (pressed[ResUp]-pressed[ResDown]))
			dirty = true;

		if (dirty)
		{
			dirty = false;
			Complex d = (end - start) * moveSpeed;
			start += Complex(d.real * (pressed[Right] - pressed[Left]), d.imaginary * (pressed[Down] - pressed[Up]));
			end += Complex(d.real * (pressed[Right] - pressed[Left]), d.imaginary * (pressed[Down] - pressed[Up]));
			start += d * (pressed[In] - pressed[Out]);
			end -= d * (pressed[In] - pressed[Out]);

			depth = depth + depth * 0.1 *(pressed[Plus] - pressed[Minus]);
			depth = depth > 0 ? depth : 1;

			int newRes= renderWidth + renderWidth * 0.1 * (pressed[ResUp] - pressed[ResDown]);
			renderWidth = newRes > WIDTH ? WIDTH : (newRes > 10 ? newRes : 10);

			newRes = renderHeight + renderHeight * 0.1 * (pressed[ResUp] - pressed[ResDown]);
			renderHeight = newRes > HEIGHT ? HEIGHT : (newRes > 10 ? newRes : 10);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);

			SDL_SetRenderTarget(renderer, texture);
			drawMandelbrot(texture, start, end, depth);
			SDL_SetRenderTarget(renderer, NULL);

			SDL_RenderClear(renderer);
			SDL_Rect rect;
			rect.x = 0;
			rect.y = 0;
			rect.w = renderWidth;
			rect.h = renderHeight;
			SDL_RenderCopy(renderer, texture, &rect, NULL);
			SDL_RenderPresent(renderer);
			lastFrameTime = SDL_GetTicks() - t;
		}
		
		
		t = SDL_GetTicks() - t;
		
		printf("   \b\b\b\b\b\b\b\b\b\b\b\bfps: %d", 1000 / (t > 0 ? t : 1));
		t = SDL_GetTicks();
		unsigned int slowDownCowboy = (1000 / 60) - lastFrameTime;
		slowDownCowboy > 0 ? SDL_Delay(slowDownCowboy): (void)(0);

	}
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}