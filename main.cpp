#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
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

class LText
{
    public:
        //Init variables
        LText();
        //Dealloc memory
        ~LText();

        //Create text
        bool createText(std::string fontPath, SDL_Color color, int size, std::string text);

        void free();

        //Renders font at given point
        void render(int x, int y);

        int getWidth();
        int getHeight();
    
    private:
        //The hardware font texture
        SDL_Texture* mText;
        int mWidth;
        int mHeight;
};



//Starts SDL and creates window
bool init();
//Loads media
bool loadMedia();
//Frees media and shuts down SDL
void close();

//The window to render to
SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;

//Image textures
LTexture gRockTexture;
LTexture gPaperTexture;
LTexture gScissorsTexture;

//Text textures
LText gWelcome;


//
//LText funcitons
//


LText::LText()
{
    //Init
    mText = NULL;
    mWidth = 0;
    mHeight = 0;
}

LText::~LText()
{
    //Dealloc
    free();
}

bool LText::createText(std::string fontPath, SDL_Color color, int size, std::string text)
{
    free();

    SDL_Texture* finalTexture = NULL;
    TTF_Font* gFont = TTF_OpenFont(fontPath.c_str(), size);

    if (gFont == NULL)
    {
        printf("Font could not be opened from %s. SDL_ttf Error: %s\n", fontPath.c_str(), TTF_GetError());
    }
    else
    {
        SDL_Surface* fontSurface = TTF_RenderText_Solid(gFont, text.c_str(), color);
        if (fontSurface == NULL)
        {
            printf("Unable to create surface from font. SDL_ttf Error: %s\n", TTF_GetError());
        }
        else
        {
            finalTexture = SDL_CreateTextureFromSurface(gRenderer, fontSurface);
            if (finalTexture == NULL)
            {
                printf("Unable to create texture from font surface. SDL Error: %s\n", SDL_GetError());
            }
            else
            {
                SDL_QueryTexture(finalTexture, NULL, NULL, &mWidth, &mHeight);
            }

            SDL_FreeSurface(fontSurface);
        }
    }

    mText = finalTexture;
    return mText != NULL; 
}

void LText::free()
{
    if (mText != NULL)
    {
        SDL_DestroyTexture(mText);
        mText = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LText::render(int x, int y)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    SDL_RenderCopy(gRenderer, mText, NULL, &renderQuad);
}

int LText::getWidth()
{
    return mWidth;
}

int LText::getHeight()
{
    return mHeight;
}

//
// LTexture functions
//


LTexture::LTexture()
{
    //Init
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    //Dealloc
    free();
}

bool LTexture::loadFromFile(std::string path)
{
    //Get rid of preexisting texture
    free();
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s. SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free()
{
    //Free texture if it exists
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render(int x, int y)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

bool init()
{
    //Init flag
    bool success = true;
    //Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled.\n");
        }

        //Create window
        gWindow = SDL_CreateWindow("Epic Rock Paper Scissors", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Wdinwo could not be created. SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created. SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Init renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Init PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize. SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }

                //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load rock texture
    if (!gRockTexture.loadFromFile("media/rock.png"))
    {
        printf("Failed to load rock texture image.\n");
        success = false;
    }

    //Load paper texture
    if (!gPaperTexture.loadFromFile("media/paper.png"))
    {
        printf("Failed to load paper texture.\n");
        success = false;
    }

    //Load scissors texture
    if (!gScissorsTexture.loadFromFile("media/scissors.png"))
    {
        printf("Failed to load scissors texture.\n");
        success = false;
    }

    if (!gWelcome.createText("media/ComicSansMS.ttf", { 0, 0, 0 }, 20, "Use your keyboard: 1 - rock,  2 - paper, 3 - scissors."))
    {
        printf("Failed to create welcome message texture.\n");
        success = false;
    }

    return success;
}

void close()
{
    gRockTexture.free();
    gWelcome.free();

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}

int checkWin(int p, int c)
{
    if (p == 1)
    {
        switch (c)
        {
        case 1:
            return 3;
            break;
        
        case 2:
            return 2;
            break;
        
        case 3:
            return 1;
            break;
        
        default:
            break;
        }
    }

    if (p == 2)
    {
        switch (c)
        {
        case 1:
            return 1;
            break;
        
        case 2:
            return 3;
            break;
        
        case 3:
            return 2;
            break;
        
        default:
            break;
        }
    }

    if (p == 3)
    {
        switch (c)
        {
        case 1:
            return 2;
            break;
        
        case 2:
            return 1;
            break;
        
        case 3:
            return 3;
            break;
        
        default:
            break;
        }
    }

    return 0;
}

int main(int argc, char* args[])
{
    int pChoice;
    int cChoice;
    int winner = 0;

    //Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize.\n");
    }
    else
    {
        //Load media
        if (!loadMedia())
        {
            printf("Failed to load media.\n");
        }
        else
        {
            //Main loop flag
            bool quit = false;
            //Event handler
            SDL_Event e;

            while (!quit)
            {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0)
                {
                    //User requests quit
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }

                    if (winner == 0)
                    {
                        if (e.type == SDL_KEYDOWN)
                        {
                            switch (e.key.keysym.sym)
                            {
                            case SDLK_1:
                                pChoice = 1;
                                cChoice = rand() % 3 + 1;
                                winner = checkWin(pChoice, cChoice);
                                break;

                            case SDLK_2:
                                pChoice = 2;
                                cChoice = rand() % 3 + 1;
                                winner = checkWin(pChoice, cChoice);
                                break;

                            case SDLK_3:
                                pChoice = 3;
                                cChoice = rand() % 3 + 1;
                                winner = checkWin(pChoice, cChoice);
                                break;
                            
                            default:
                                break;
                            }
                        }
                    }
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                //Fill background
                SDL_Rect bgRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
                SDL_SetRenderDrawColor(gRenderer, 172, 202, 250, 0xFF);
                SDL_RenderFillRect(gRenderer, &bgRect);

                gWelcome.render(20, 20);

                switch (pChoice)
                {
                case 1:
                    gRockTexture.render(80, 200);
                    break;
                
                case 2:
                    gPaperTexture.render(80, 200);
                    break;

                case 3:
                    gScissorsTexture.render(80, 200);
                    break;
                
                default:
                    break;
                }

                switch (cChoice)
                {
                case 1:
                    gRockTexture.render(400, 200);
                    break;
                
                case 2:
                    gPaperTexture.render(400, 200);
                    break;

                case 3:
                    gScissorsTexture.render(400, 200);
                    break;
                
                default:
                    break;
                }

                //Update screen
                SDL_RenderPresent(gRenderer);
            }
        
        }
    }

    close();

    return 0;
}