#include<iostream>
#include<stdio.h>
#include<algorithm>
#include<SDL.h>
#include<cstdlib>
#include<SDL_image.h>
using namespace std;
int STEP = 1;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const string WINDOW_TITLE = "Game ran hay vl luon";


void logSDLError(std::ostream& os,const std::string &msg, bool fatal=false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
SDL_Texture* loadTexture(string path,SDL_Renderer* renderer);
    SDL_Texture* background ;
    SDL_Texture* head ;
    SDL_Texture* apple ;
    SDL_Texture* body;
 struct Position{
    int x;
    int y;
 };
 Position operator+ (const Position& a, const Position& b) {
	return Position{ a.x + b.x, a.y + b.y };
}
void operator+= (Position& a, const Position& b) {
	a.x += b.x;
	a.y += b.y;
}
 struct Food {
	int x, y;
	void move() {
		x = rand() % (800 / 16);
		y = rand() % (800 / 16);
	}
	void draw(SDL_Renderer* renderer) {
		 apple = loadTexture("apple.png" , renderer);
		 SDL_Rect appleRect;
		 SDL_QueryTexture(apple, NULL, NULL, &appleRect.w, &appleRect.h);
		 appleRect.x=16*x;
		 appleRect.y=16*y;
        SDL_RenderCopy(renderer, apple, NULL, &appleRect);

	}
};
const int maxOfTail = 200;
struct Snake{
    Position snakePos;
    Position snakeDirection;
    Position snakeNewDirection;
    Position tail[maxOfTail];

	int start, last;
	int length = 20;

	int temp;
    void update(int delta_time, struct Food& food) {
	    temp += delta_time;
		if (temp > 90 - (length)) {
			temp = 0;

			snakeDirection = snakeNewDirection;

			tail[last % maxOfTail] = snakePos;

			start++;
			last++;

			snakePos.x += snakeDirection.x;
			snakePos.y += snakeDirection.y;
			snakePos.x = (snakePos.x + 50) % 50;
			snakePos.y = (snakePos.y + 50) % 50;

			if (snakePos.x == food.x && snakePos.y == food.y) {
				length += 1;
				start -= 1;
				food.move();
			}
			for (int i = 0; i < length; i++) {
				Position& tailPos = tail[(start + i) % maxOfTail];
				if (tailPos.x == snakePos.x && tailPos.y == snakePos.y) {
					//tu dam vao dit
					length = 0;
					start = last;
				}
			}
		}
	}
	void draw(SDL_Renderer* renderer) {

		for (int i = 0; i < length; i++) {
            SDL_SetRenderDrawColor(renderer,rand()%255, rand()%255, rand()%255, rand()%255);
			Position& tailPos = tail[(start + i) % maxOfTail];
			SDL_Rect r{ 16 * tailPos.x, 16 * tailPos.y, 16, 16 };
			SDL_RenderFillRect(renderer, &r);
		}
		 head = loadTexture("head.png" , renderer);
		 SDL_Rect headRect;
		 SDL_QueryTexture(head, NULL, NULL, &headRect.w, &headRect.h);
		 headRect.x=16*snakePos.x;
		 headRect.y=16*snakePos.y;
        SDL_RenderCopy(renderer, head, NULL, &headRect);
        SDL_RenderPresent(renderer);
	}

};


 void pop_up_gameover(SDL_Window* &window,SDL_Renderer* renderer,SDL_Event e);
 void loadEverything(SDL_Renderer* renderer);


int main(int argc, char* argv[]){

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event e;
	initSDL(window, renderer);
    background = loadTexture("grass.jpg" , renderer);
    loadEverything(renderer);
	bool moving = true;
	Snake snake = {};
	Food food = {};
	food.move();

	int current_time = 0, previous_time, delta_time;
	while (moving) {
		previous_time = current_time;
		current_time = SDL_GetTicks();
		delta_time = current_time - previous_time;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				moving = false;
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				    case SDLK_ESCAPE:
					moving = false;
                break;
                    case SDLK_w: if (snake.snakeDirection.y != STEP) snake.snakeNewDirection = { 0, -STEP };
                break;
                    case SDLK_s: if (snake.snakeDirection.y != -STEP) snake.snakeNewDirection = { 0, STEP };
                break;
                    case SDLK_a: if (snake.snakeDirection.x != STEP) snake.snakeNewDirection = { -STEP, 0 };
                break;
                    case SDLK_d: if (snake.snakeDirection.x != -STEP) snake.snakeNewDirection = { STEP, 0 };
                break;
                    default:
                break;
				}
			}
		}
		snake.update(delta_time, food);
        //SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background , 0, 0);
		snake.draw(renderer);
		food.draw(renderer);
		SDL_RenderPresent(renderer);
	}
	waitUntilKeyPressed();
	quitSDL(window,renderer);
}

void loadEverything(SDL_Renderer *renderer) {
    background = loadTexture("grass.jpg" , renderer);
    apple = loadTexture("apple.png" , renderer);
    SDL_RenderCopy(renderer, background , 0,0);
    SDL_RenderPresent(renderer);

}

void pop_up_gameover(SDL_Window* &window, SDL_Renderer* renderer,SDL_Event e){
    SDL_Texture* gameover = loadTexture("gameover.png" , renderer);
    SDL_RenderCopy(renderer, gameover , 0,0);
    SDL_RenderPresent(renderer);
}
void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
      // SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        // printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        logSDLError(std::cout, "IMG_Init da chet :v", true);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
SDL_Texture* loadTexture( string path, SDL_Renderer* renderer )
{
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == nullptr)
        cout << "Unable to load image " << path << " SDL_image Error: "
             << IMG_GetError() << endl;
    else {
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == nullptr)
            cout << "Unable to create texture from " << path << " SDL Error: "
                 << SDL_GetError() << endl;
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}

