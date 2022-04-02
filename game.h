#ifndef GAME__H
#define GAME__H

#include <iostream>
#include <SDL.h>
#include "params.h"
#include "box.h"

using namespace std;

const int X_STEP[] = {0 ,    0 , -1,  1  };
const int Y_STEP[] = {1 ,    -1,  0,  0 };

struct Game{
    Box box;
    Box box2;
    int score = 0;
    SDL_Renderer* renderer;

    Game(SDL_Renderer* _renderer) {
        if (_renderer == nullptr) {
            cerr << "error renderer not yet initialized";
            exit(1);
        }
        renderer = _renderer;

        box.x = SCREEN_WIDTH / 2;
        box.y = SCREEN_HEIGHT / 2;

        box2.x = 100;
        box2.y = 100;
     }

    bool isOver() {
        return (box.x < 0 || box.x > SCREEN_WIDTH || box.y < 0 || box.y > SCREEN_HEIGHT);
    }

    void update(Event event) {
        box.move(X_STEP[event] * STEP, Y_STEP[event] * STEP);
        box2.move(1,1);
        score++;
    }

    void draw()
    {
        box.draw(renderer);
        box2.draw(renderer);
        SDL_RenderPresent(renderer);
    }
};

#endif
