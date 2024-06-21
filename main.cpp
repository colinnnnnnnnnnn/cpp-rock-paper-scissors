#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#define main SDL_main

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture
{
    public:
        //Init variables
        LTexture();
        //Dealloc memory
        ~LTexture();

        //Loads image
        bool loadFromFile(std::string path);
        
        //Dealloc texture
        void free();

        //Renders texture at given point
        void render(int x, int y);

        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //The actual hardware texture
        SDL_Texture* mTexture;

        //Image dimensions
        int mWidth;
        int mHeight;
};

//Starts SDL and creates window
bool init();
//Loads media
bool loadMedia();
//Frees media and shuts down SDL
void close();

int main(int argc, char* args[])
{

    return 0;
}