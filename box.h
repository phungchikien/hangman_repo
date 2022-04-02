#ifndef BOX__H
#define BOX__H

#include <iostream>
#include <SDL.h>

using namespace std;

const int RECT_SIZE = 30;

struct Box {
    int x;
    int y;

    void draw(SDL_Renderer* renderer) {
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = RECT_SIZE;
        rect.h = RECT_SIZE;

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_RenderDrawRect(renderer, &rect);
    }

    void move(int xStep, int yStep) {
        x += xStep;
        y += yStep;
    }
};


#endif
