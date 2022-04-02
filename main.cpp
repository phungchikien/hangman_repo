#include <iostream>
#include<SDL.h>
#include "SDL_utils.h"
#include "SDL_utils.cpp"

using namespace std;
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const string WINDOW_TITLE = "Test animation";
int STEP_DELAY = 5;
struct Box{
    int x;
    int y;
    int size = 10;
    int stepX=2;
    int stepY=2;

    Box(int _x,int _y) :x(_x), y(_y){}

    void render(SDL_Renderer* renderer){
        SDL_Rect filled_rect;
        filled_rect.x = x;
        filled_rect.y = y;
        filled_rect.w = size;
        filled_rect.h = size*5;
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &filled_rect);
    }
    void move(){
        x += stepX;
        y += stepY;
        cout<<stepX<<" , "<<stepY<<endl;
    }
    void turnLeft(){
        stepX = -2;
        stepY=0;
    }
    void turnRight(){
        stepX = 2;
        stepY=0;
    }
    void turnUp(){
        stepY = -2;
        stepX=0;
    }
    void turnDown(){
        stepY = 2;
        stepX=0;
    }
    bool inside(int minX, int minY, int maxX, int maxY){
        return (minX<=x&&minY<=y&&x+size<=maxX&&y+size<=maxY);
    }
};
int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event e;
    initSDL(window,renderer,WINDOW_TITLE,SCREEN_WIDTH,SCREEN_HEIGHT);
    Box box(10,10);

    while(box.inside(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)){
            box.move();
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        box.render(renderer);
        SDL_RenderPresent(renderer);
    //waitUntilKeyPressed();
    //SDL_Delay(100);
    //box.move();
            SDL_Delay(STEP_DELAY);
                if ( SDL_PollEvent(&e) == 0) continue;
                if (e.type == SDL_QUIT) break;
                if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_ESCAPE: break;
                        case SDLK_LEFT: box.turnLeft();
        			break;
            	       case SDLK_RIGHT: box.turnRight();
            		break;
                        case SDLK_DOWN: box.turnDown();
					break;
                        case SDLK_UP: box.turnUp();
            		break;
                        default: break;
                    }
            }
}
    //waitUntilKeyPressed();
    quitSDL(window,renderer);
    return 0;
}

