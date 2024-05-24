#include "main.h"
#include "main.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2_image/SDL_image.h>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int IMAGE_WIDTH = SCREEN_WIDTH + 300;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
int speed = 1;
int CameraX = 0;
int textureWidthDiff = IMAGE_WIDTH - SCREEN_WIDTH;

class Ball {
public:
    float x, y;
    int velocityX, velocityY;
    SDL_Rect spriteClip;
    int spriteWidth, spriteHeight;
    int currentSprite;

    Ball(float posX, float posY, int velX, int velY, int spriteW, int spriteH) : x(posX), y(posY), velocityX(velX), velocityY(velY), spriteWidth(spriteW), spriteHeight(spriteH), currentSprite(0) {
        spriteClip.x = 0;
        spriteClip.y = 0;
        spriteClip.w = spriteWidth;
        spriteClip.h = spriteHeight;
    };

    void animate() {

        spriteClip.x = currentSprite * spriteWidth;
    }

    void update() {

        x += velocityX * speed;
        y += velocityY * speed;

        if (x > SCREEN_WIDTH - spriteWidth) {
            x = SCREEN_WIDTH - spriteWidth;
            velocityX *= -1;
        }
        else if (x < 0) {
            x = 0;
            velocityX *= -1;
        }


        if (y > SCREEN_HEIGHT - spriteHeight) {
            y = SCREEN_HEIGHT - spriteHeight;
            velocityY *= -1;
        }
        else if (y < 0) {
            y = 0;
            velocityY *= -1;
        }

        animate();
    }
};

SDL_Texture* loadTexture(const std::string& path) {

    SDL_Texture* newTexture = NULL;


    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {

        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }


        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

bool initSDL()
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }


    gWindow = SDL_CreateWindow("2D Rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    };

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void closeSDL()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!initSDL()) {
        printf("Failed to initialize SDL!\n");
        return -1;
    }

    SDL_Texture* ballTexture = loadTexture("./ball.png");


    bool quit = false;

    SDL_Event e;

    srand(time(NULL));

    std::vector<Ball> balls;

    int ballSpriteWidth = 100;
    int ballSpriteHeight = 100;
    balls.push_back(Ball(800, SCREEN_HEIGHT - 155, 4, 4, ballSpriteWidth, ballSpriteHeight));

    while (!quit) {

        while (SDL_PollEvent(&e) != 0) {

            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }


        SDL_RenderClear(gRenderer);


        for (Ball& ball : balls) {
            ball.update();
            SDL_Rect ballDest = { static_cast<int>(ball.x), static_cast<int>(ball.y), ball.spriteWidth, ball.spriteHeight };
            SDL_RenderCopy(gRenderer, ballTexture, &ball.spriteClip, &ballDest);
        }

        SDL_RenderPresent(gRenderer);

        SDL_Delay(10);
    }

    SDL_DestroyTexture(ballTexture);


    closeSDL();

    return 0;
}