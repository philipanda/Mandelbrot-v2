#pragma once
#include <SDL.h>
#include "Complex.h"
#include <thread>
#include <vector>
#include <math.h>
#include <chrono>
#include <algorithm>
#include <execution>
using namespace std;

static unsigned int renderWidth = 620;
static unsigned int renderHeight = 480;
static vector<unsigned int> textureArray;

int colors[] = {0x000764FF, 0x206BCBFF, 0xEDFFFFFF, 0xFFAA00FF, 0x000200FF};

unsigned char mixColors(unsigned char color1, unsigned char color2, int amount1, int amount2)
{
	unsigned char r = (color1 * (amount2-amount1) + color2 * amount1) / (amount2);
	return r;
}

unsigned int mixColorsRGB(unsigned int color1, unsigned int color2, int amount1, int amount2)
{
	amount2 = amount2 > 0 ? amount2: 1;
	unsigned int result = mixColors(color1, color2, amount1, amount2);

	result += mixColors(color1 >> 8, color2 >> 8, amount1, amount2) << 8;
	result += mixColors(color1 >> 16, color2 >> 16, amount1, amount2) << 16;
	result += mixColors(color1 >> 24, color2 >> 24, amount1, amount2) << 24;
	return result;
}


void setDimensions(unsigned int w, unsigned int h)
{
	renderWidth = w;
	renderHeight = h;
}

vector<unsigned int>& createTextureArray(unsigned int w, unsigned int h)
{
	setDimensions(w, h);
	textureArray = vector<unsigned int>();
	textureArray.reserve((unsigned long long)renderWidth * renderHeight);
	return textureArray;
}


int mandelbrot(const Complex& c, unsigned int max_iter)
{
	Complex z;
	unsigned int n = 0;
	
	while (z.modulusSquared() <= 4 && n < max_iter)
	{
		z.square();
		z += c;
		n++;
	}
	return n + 1 - log2(log2(z.modulusSquared()) / 2);
}


unsigned int getColorMix(int n, int maxIter)
{
	unsigned int color = 0;
	if (n < maxIter * 0.16)
		color = mixColorsRGB(colors[0], colors[1], n, maxIter *0.16);
	else if (n < maxIter * 0.42)
		color = mixColorsRGB(colors[1], colors[2], n, maxIter * 0.42);
	else if (n < maxIter * 0.6425)
		color = mixColorsRGB(colors[2], colors[3], n, maxIter * 0.6425);
	else if (n < maxIter * 0.8575)
		color = mixColorsRGB(colors[3], colors[4], n, maxIter * 0.8575);
	else
		color = colors[4];
	return color;
}

unsigned int getColorFast(unsigned int n, unsigned int max_iter)
{
	unsigned int color = n * 255 / max_iter;
	if (n == max_iter)
		color = 0;
	return color + color * 0xFF + color * 0xFFFF + color * 0xFFFFFF;
}

void renderMandelbrotBlock(unsigned int w, unsigned int h, unsigned int* textureArray, unsigned int startX, unsigned int startY, Complex start, Complex end, unsigned int max_iter)
{
	for (double y = 0; y < h; y++)
		for (double x = 0; x < w; x++)
		{
			Complex delta(x * (end.real - start.real) / w, y * (end.imaginary - start.imaginary) / h);
			int n = mandelbrot(start+delta, max_iter);
			//unsigned int color = getColorFast(n, max_iter);
			unsigned int color = getColorMix(n, max_iter);
			//unsigned int color = getColor(n, max_iter);
		
			textureArray[(int)((y + startY) * renderWidth + x + startX)] = color;
		}
}

void renderMandelbrotParallel3(unsigned int w, unsigned int h, vector<unsigned int>& textureArray, Complex& start, Complex& end, unsigned int max_iter = 64)
{
	Complex delta((end.real - start.real) / w, (end.imaginary - start.imaginary) / h);
	vector<thread> threads(w*h);

	for(int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			int i = y * w + x;
			unsigned int* val = &textureArray[i];
			threads.push_back(thread(
				[x, y, start, delta, max_iter, val]()
				{
					(*val) = getColorFast(mandelbrot(Complex(start.real + delta.real * x, start.imaginary + delta.imaginary * y), max_iter), max_iter);
				}));
		}
	for (int i = 0; i < threads.size(); i++)
		threads[i].join();
}
void renderMandelbrotParallel2(unsigned int w, unsigned int h, vector<unsigned int>& textureArray, Complex& start, Complex& end, unsigned int max_iter = 64)
{
	Complex delta((end.real - start.real) / w, (end.imaginary - start.imaginary) / h);

	
	for_each(std::execution::par_unseq, begin(textureArray), std::end(textureArray), [max_iter, start, idx=0, delta](unsigned int& n) mutable {
		Complex c(start.real + delta.real*(idx % renderWidth), start.imaginary + delta.imaginary*(idx / renderWidth));
		n = getColorFast(mandelbrot(c, max_iter), max_iter);
		idx++;
		});
}

void renderMandelbrotParallel(unsigned int w, unsigned int h, vector<unsigned int>& textureArray, Complex& start, Complex& end, unsigned int max_iter = 64)
{
	int d = 2;
	std::vector<std::thread> threads;
	threads.reserve(d * d);
	Complex delta = (end - start) / d;

	for(int y = 0; y < d; y ++)
		for (int x = 0; x < d; x++)
		{
			Complex s(start.real + (delta.real * x), start.imaginary + (delta.imaginary * y));
			Complex e = s + delta;
			threads.push_back(std::thread(renderMandelbrotBlock, (w / d), (h / d), 
				&textureArray[0], x * (w / d), y * (h / d),
				 s, e, max_iter));
		}
	for (int i = 0; i < threads.size(); i++)
		threads[i].join();

}

void renderMandelbrot(unsigned int w, unsigned int h, unsigned int* textureArray, Complex& start, const Complex& end, unsigned int max_iter = 64)
{
	for (double y = 0; y < h; y++)
		for (double x = 0; x < w; x++)
		{
			Complex delta(x * (end.real - start.real) / w, y * (end.imaginary - start.imaginary) / h);
			int n = mandelbrot(start + delta, max_iter);
			unsigned int color = (n * 255) / max_iter;
			color = color == 255 ? 0 : color;
			textureArray[(int)(y*w + x)] = color+color*0xFF + color*0xFFFF + color*0xFFFFFF;

		}
}

void drawMandelbrot(SDL_Texture* texture, Complex& start, Complex& end, unsigned int max_iter=64)
{
	//unsigned int t1 = SDL_GetTicks();
	renderMandelbrotParallel(renderWidth, renderHeight, textureArray, start, end, max_iter);
	//printf("Render time: %dms\n", SDL_GetTicks() - t1);
	//t1 = SDL_GetTicks();
	SDL_UpdateTexture(texture, NULL, &textureArray[0], renderWidth * 4);
	//printf("Update texture time: %dms\n\n", SDL_GetTicks() - t1);
}
