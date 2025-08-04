#pragma once
#include "Complex.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <execution>
#include <math.h>
#include <thread>
#include <vector>
using namespace std;

unsigned char mixColors(unsigned char color1, unsigned char color2, int amount1,
                        int amount2);

unsigned int mixColorsRGB(unsigned int color1, unsigned int color2, int amount1,
                          int amount2);

void setDimensions(unsigned int w, unsigned int h);

unsigned int *createTextureArray(unsigned int w, unsigned int h);

int mandelbrot(const Complex &c, unsigned int max_iter);

unsigned int getColorMix(int n, int maxIter);

unsigned int getColorFast(unsigned int n, unsigned int max_iter);

void renderMandelbrotBlock(unsigned int w, unsigned int h,
                           unsigned int *textureArray, unsigned int startX,
                           unsigned int startY, Complex start, Complex end,
                           unsigned int max_iter);

void renderMandelbrotParallel3(unsigned int w, unsigned int h,
                               unsigned int *textureArray, Complex &start,
                               Complex &end, unsigned int max_iter = 64);

void renderMandelbrotParallel(unsigned int w, unsigned int h,
                              unsigned int *textureArray, Complex &start,
                              Complex &end, unsigned int max_iter = 64);

void renderMandelbrot(unsigned int w, unsigned int h,
                      unsigned int *textureArray, Complex &start,
                      const Complex &end, unsigned int max_iter = 64);
void drawMandelbrot(SDL_Texture *texture, Complex &start, Complex &end,
                    unsigned int max_iter = 64);