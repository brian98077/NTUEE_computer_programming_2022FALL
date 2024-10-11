//Using SDL, SDL_image, standard IO, and strings
#define START 0
#define LEVEL1 1
#define LEVEL1_RED 2
#define LEVEL1_YELLOW 3
#define LEVEL1_GREEN 4
#define LEVEL1_BLUE 5
#define LEVELS_SURFACE_TOTAL 6
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>
#include"Two-Dots-algorithm.h"


using namespace std;
int screen;
//Screen dimension constants
extern const int SCREEN_WIDTH = 1080;
extern const int SCREEN_HEIGHT = 720;

//Key press surfaces constants
extern LTexture gTextTexture;
extern LTexture gText;

//LEVELS sprites
LTexture gLEVELSSpriteSheetTexture[LEVELS_SURFACE_TOTAL];

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
        //Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "TWO DOTS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

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
void close()
{
    //Free loaded images
    gTextTexture.free();

    //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;
    //Free loaded images
	gButtonSpriteSheetTexture.free();
	for(int i=0;i<LEVELS_SURFACE_TOTAL;i++){
        gLEVELSSpriteSheetTexture[i].free();
	}

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

Scoreban Score;
Stepsban Steps;

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//screen status flag
            screen = START;

            //creat dots
            dot dots[6][6];

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
                    Score.Set_score(totalScore);
                    Steps.Set_steps(steps);
				    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
				    //select screen
				    else if( e.type != SDL_QUIT && screen == START )
                    {
                        //Clear screen
                        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                        SDL_RenderClear( gRenderer );
                        //screen status
                        gLEVELSSpriteSheetTexture[START].render(0, 0);
                        //Update screen
                        SDL_RenderPresent( gRenderer );

                        if( e.type == SDL_MOUSEBUTTONDOWN )
                        {
                            //create dot color
                            for(int i=1;i<=5;i++){
                                for(int j=1;j<=5;j++){
                                    srand(time(0));
                                    int random1 = rand()*rand(), random2 = rand()*rand()*rand()+7*rand()*rand();
                                    dots[i][j].revisePos(i,j);

                                    // set random color
                                    if(i == 2 && (j == 1 || j == 2 || j == 0) || i == 4 && (j == 2 || j == 4 || j == 5))
                                    dots[i][j].reviseColor(abs(random1*(i*7+j)+random1*random2)%4);
                                    else if(i == 1 && (j == 2 || j == 3 || j == 4) || i == 1 && (j == 2 || j == 5))
                                        dots[i][j].reviseColor(abs(random2*random2*3+i*j)%4);
                                    else if(i == 3 && (j == 1 || j == 3 || j == 5))
                                        dots[i][j].reviseColor(abs(random2*j+random1*i*i*9+1)%4);
                                    else
                                        dots[i][j].reviseColor(abs(random1*i*j+random2*i+random2*j+3)%4);
//		                    	        gButtons[i-1][j-1].setColor(dots[i][j].color);
                                    switch(dots[i][j].getColor())
                                    {
                                        case 0:
                                        gButtons[(i-1)*5+j-1].setColor(0);
                                        break;

                                        case 1:
                                        gButtons[(i-1)*5+j-1].setColor(1);
                                        break;

                                        case 2:
                                        gButtons[(i-1)*5+j-1].setColor(2);
                                        break;

                                        case 3:
                                        gButtons[(i-1)*5+j-1].setColor(3);
                                        break;
                                    }
                                }
                            }
                            gButtons[25].setColor(9); // set quit button

                            for(int i=1;i<=5;i++)
                            {
                                for(int j=1;j<=5;j++)
                                    cout << dots[j][i] << " ";
                                    cout << endl;
                            }
                            //Clear screen
                            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                            SDL_RenderClear( gRenderer );

                            screen = LEVEL1;
                            gLEVELSSpriteSheetTexture[LEVEL1].render( 0, 0);

                            //Update screen
                            SDL_RenderPresent( gRenderer );

                        }
                    }
                    else if( e.type != SDL_QUIT && screen == LEVEL1)
                    {
                        gTextTexture.render( 860, 236 );
                        gText.render( 860, 190 );
                        gTextTexture2.render( 120, 236 );
                        gText2.render( 120, 190 );
                        for(int i=0;i<TOTAL_BUTTONS;i++)
                            gButtons[ i ].handleEvent( &e,dots );
                        //Render buttons
                        for( int i = 0; i < TOTAL_BUTTONS; ++i )
                        {
                            gButtons[ i ].render();
                        }
                        //Update screen
                        SDL_RenderPresent( gRenderer );
                    }

				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
