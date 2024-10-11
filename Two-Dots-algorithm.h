#ifndef Two-Dots-algorithm
#define Two-Dots-algorithm
#include "SDL2/SDL.h"
#include <iostream>
#include <cmath>
#include "graphics.h"
#include <cstdlib>
#include "SDL2/SDL_image.h"
#include <cstdlib>
#include <ctime>
#include "SDL2/SDL_ttf.h"
using namespace std;

//Button constants
const int BUTTON_WIDTH = 48;
const int BUTTON_HEIGHT = 48;
const int TOTAL_BUTTONS = 26;
extern int screen;
// extern int
//Key press surfaces constants

extern TTF_Font *gFont = NULL;

class dot{
	private:
	    int color; // 0 = red, 1 = yellow, 2 = green, 3 = blue
		int x,y; // position
	public:

		bool bright;
		dot(){
			color = 1;
			x = -1;
			y = -1;
			bright = false;
		}

		dot(int a, int b, int c){
			if( a>3 || a<0 || b>5 || b<1 || c>5 || c<1 ){
				cout << "invalid declaration" << endl;
				x = -1;
				y = -1;
			}

			else{
				color = a;
				x = b;
				y = c;
				bright = false;
			}
		}

		int getColor(){
			return color;
		}

		// revise position
		void revisePos(int a, int b){
			if( a>5 || a<1 || b>5 || b<1 )
				cout << "invalid revise" << endl;
			else{
				x = a;
				y = b;
			}
		}

		// revise color
		void reviseColor(int a){
			if( a>3 || a<0 )
				cout << "invalid revise";
			else
				color = a;
		}

		void click(){
			if(bright)
				return;
			else
				bright = true;
		}


        friend ostream & operator << (ostream &output, const dot& a){
            switch(a.color){
                case 0:
                    output << "red";
                    break;
                case 1:
                    output << "yellow";
                    break;
                case 2:
                    output << "green";
                    break;
                case 3:
                    output << "blue";
                    break;
            }

            if( a.x>0 && a.x <=5 && a.y>0 && a.y<=5 )
                output << "(" << a.x << "," << a.y << ")";
            return output;
        };
        friend class LButton;

};


enum LButtonSprite
{
//	BUTTON_SPRITE_MOUSE_OUT = 0,
	red = 0, yellow = 1, green = 2, blue = 3,
	redBright = 4,
	yellowBright = 5,
	greenBright = 6,
	blueBright = 7,
	white = 8,
	quitbutton = 9,
	BUTTON_SPRITE_TOTAL = 10
};

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

//ifdef
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
//endif
		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

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

LTexture gTextTexture,gTextTexture2;
LTexture gText,gText2;

//The mouse button
class LButton
{
	public:
		//Initializes internal variables
		LButton();

		//Sets top left position
		void setPosition( int x, int y );

		void drop(dot (&dot_ref)[6][6],LButtonSprite cycleColor);

		//Handles mouse event
		void handleEvent( SDL_Event* e, dot (&dot_ref)[6][6] );

		//Shows button sprite
		void render();

        // set color
        void setColor(int color){

            if(color == 9)
            {
                 mCurrentSprite = quitbutton;
            }

            else
            switch( color )
			{
				case red:
				mCurrentSprite = red;
				break;

				case yellow:
				mCurrentSprite = yellow;
				break;

				case green:
				mCurrentSprite = green;
				break;

				case blue:
                mCurrentSprite = blue;

			}
        }

	private:
		//Top left
		SDL_Point mPosition;

		//Currently used global sprite
		LButtonSprite mCurrentSprite;

		double mVelY,g,e,screen_period;
};

//Loads media
bool loadMedia();

//Frees media and shuts down SDL

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Mouse button sprites
SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
LTexture gButtonSpriteSheetTexture;

//Buttons objects
LButton gButtons[ TOTAL_BUTTONS ];

//LEVELS sprites
extern LTexture gLEVELSSpriteSheetTexture[LEVELS_SURFACE_TOTAL];

void LButton::drop(dot (&dot_ref)[6][6],LButtonSprite cycleColor = white)
{
    struct dot_drop_position
    {
        bool Move = false;
        int initial_pos_y;
        int final_pos_y=0;
        int color;
    };

    dot_drop_position dot[5][5];
    for(int column=0;column<5;column++)
    {
        int space;
        for(int row=4;row>=0;row--)
        {
            if(gButtons[column*5+row].mCurrentSprite!=white)
            {
                space = 0;
                for(int i=row;i<5;i++)
                {
                    if(gButtons[column*5+i].mCurrentSprite==white)space++;
                }
                dot[column][row+space].color = dot_ref[column+1][row+1].color;
                dot[column][row+space].initial_pos_y = gButtons[(column)*5+row].mPosition.y ;
                dot[column][row+space].final_pos_y = gButtons[(column)*5+row+space].mPosition.y ;
                gButtons[(column)*5+row].mVelY = 30;
                if(space>0)dot[column][row+space].Move = true;
            }
            else if(row == 0)
            {
                space = 0;
                for(int i=row;i<5;i++)
                {
                    if(gButtons[column*5+i].mCurrentSprite==white)space++;
                }
            }
        }
        if(space>0)
        {
            for(int row=0;row<space;row++)
            {
                int i = column+1;
                int j = row+1;

                // set random color
                srand(time(0));
                int random1 = rand()*rand(), random2 = rand()*rand()*rand()+7*rand()*rand();
                if(i == 2 && (j == 1 || j == 2 || j == 0) || i == 4 && (j == 2 || j == 4 || j == 5))
                    dot[column][row].color = (abs(random1*(i*7+j)+random1*random2)%4);
                else if(i == 1 && (j == 2 || j == 3 || j == 4) || i == 1 && (j == 2 || j == 5))
                    dot[column][row].color = (abs(random2*random2*3+i*j)%4);
                else if(i == 3 && (j == 1 || j == 3 || j == 5))
                    dot[column][row].color = (abs(random2*j+random1*i*i*9+1)%4);
                else
                    dot[column][row].color = (abs(random1*i*j+random2*i+random2*j+3)%4);


                dot[column][row].initial_pos_y = gButtons[(column)*5+row].mPosition.y - 200 - 96*(space-1);
                dot[column][row].final_pos_y = gButtons[(column)*5+row].mPosition.y;
                gButtons[(column)*5+row].mVelY = 25;
                dot[column][row].Move = true;
            }
        }
    }

    for(int column=0;column<5;column++)
    {
        for(int row=4;row>=0;row--)
        {
            dot_ref[column+1][row+1].color = dot[column][row].color;
            gButtons[(column)*5+row].setColor(dot_ref[column+1][row+1].color);
            gButtons[(column)*5+row].mPosition.y = dot[column][row].initial_pos_y;
        }
    }

    //move flag
    bool move = true;
    while(move)
    {
        //set frame start time
        int tstart = SDL_GetTicks();

        int count=0;
        for(int column=0;column<5;column++)
        {
            for(int row=4;row>=0;row--)
            {
                if(dot[column][row].Move)
                {
                    gButtons[(column)*5+row].mVelY += g;
                    gButtons[(column)*5+row].mPosition.y += gButtons[(column)*5+row].mVelY;
                    if(gButtons[(column)*5+row].mPosition.y >= dot[column][row].final_pos_y )
                    {
                        gButtons[(column)*5+row].mPosition.y -= gButtons[(column)*5+row].mVelY;
                        gButtons[(column)*5+row].mVelY = (-e*gButtons[(column)*5+row].mVelY);
                        if(gButtons[(column)*5+row].mVelY >= -2*g){
                            gButtons[(column)*5+row].mPosition.y = dot[column][row].final_pos_y;
                            gButtons[(column)*5+row].mVelY = 0;
                            dot[column][row].Move = false;
                        }
                    }
                    count++;
                }
            }
        }
        //delay
        while((SDL_GetTicks()-tstart)<screen_period);

        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        switch (cycleColor)
        {
            case white:
                screen = LEVEL1;
                gLEVELSSpriteSheetTexture[LEVEL1].render( 0, 0);
                break;
            case red:
                screen = LEVEL1_RED;
                gLEVELSSpriteSheetTexture[LEVEL1_RED].render( 0, 0);
                break;
            case yellow:
                screen = LEVEL1_YELLOW;
                gLEVELSSpriteSheetTexture[LEVEL1_YELLOW].render( 0, 0);
                break;
            case green:
                screen = LEVEL1_GREEN;
                gLEVELSSpriteSheetTexture[LEVEL1_GREEN].render( 0, 0);
                break;
            case blue:
                screen = LEVEL1_BLUE;
                gLEVELSSpriteSheetTexture[LEVEL1_BLUE].render( 0, 0);
                break;

        }

        //Render buttons
        for( int i = 0; i < TOTAL_BUTTONS; ++i )
        {
            gButtons[ i ].render();
        }

        //Update screen
        SDL_RenderPresent( gRenderer );
        if(count==0){
            move=false;
        }
    }

    //Clear screen
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( gRenderer );

    screen = LEVEL1;
    gLEVELSSpriteSheetTexture[LEVEL1].render( 0, 0);

    //Render buttons
    for( int i = 0; i < TOTAL_BUTTONS; ++i )
    {
        gButtons[ i ].render();
    }

    //Update screen
    SDL_RenderPresent( gRenderer );
}

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

//#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}

	//Return success
	return mTexture != NULL;
}
//#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
	mVelY = 0;
	g = 7;
	e=0.5;
	screen_period=18;

//	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}



void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}

class Scoreban
{
    private:

        int score=0;
        SDL_Point score_Position;
    public:
        Scoreban();
        void score_render();
        void Set_score(int s);
        bool S()
        {
            string a = to_string(score);
            SDL_Color textColor = { 255, 0, 0,255 };
            return gTextTexture.loadFromRenderedText(a,textColor);
        }

        void get_score(int s)
        {
            score+=s;
        }
        void score_positon(int a,int b)
        {
            score_Position.x = a;
            score_Position.y = b;
        }

};
    Scoreban::Scoreban()
    {
        score_Position.x = 0;
        score_Position.y = 0;
    }

    void Scoreban::Set_score(int s)
    {
        score=s;
        string d;
        d = to_string(s);
        SDL_Color textColor = { 255, 0, 0,255 };
        if(s<10)
        {
            d="0"+d;
            gTextTexture.loadFromRenderedText(d,textColor);
        }
        else
        {
            gTextTexture.loadFromRenderedText(d,textColor);
        }
    }
    void Scoreban::score_render()
    {
        gTextTexture2.render( score_Position.x, score_Position.x );
    }



class Stepsban
{
    private:

        int step = 0;
        SDL_Point steps_Position;
    public:
        Stepsban();
        void steps_render();
        void Set_steps(int s);
        bool S()
        {
            string a = to_string(step);
            SDL_Color textColor = { 255, 0, 0,255 };
            return gTextTexture2.loadFromRenderedText(a,textColor);
        }


        void steps_positon(int a,int b)
        {
            steps_Position.x = a;
            steps_Position.y = b;
        }

};
    Stepsban::Stepsban()
    {
        steps_Position.x = 0;
        steps_Position.y = 0;
    }

    void Stepsban::Set_steps(int s)
    {
        step = s;
        string d;
        d = to_string(s);
        SDL_Color textColor = { 255, 0, 0,255 };
        if(s<10)
        {
            d="0"+d;
            gTextTexture2.loadFromRenderedText(d,textColor);
        }
        else
        {
            gTextTexture2.loadFromRenderedText(d,textColor);
        }
    }
    void Stepsban::steps_render()
    {
        gTextTexture2.render( steps_Position.x, steps_Position.y );
    }
// for mouse event
bool mouseIsDown = false;
int clickNum = 0;
int previousX = 0;
int previousY = 0;
int initX = 0;
int initY = 0;
LButtonSprite cycleColor = white;
bool cycle = false;
int previousColor = 9; // set color to null = 9 / red = 0, yellow = 1, green = 2, blue = 3
int steps = 20;
int totalScore = 0;
bool minusSteps = false;

int point_transform(int s)
    {
        double a=s;
        double b;
        b = s + pow(1.2, a)-1;
        int c=b;

        return c*10;

    }

void LButton::handleEvent( SDL_Event* e,dot (&dot_ref)[6][6] )
{
	//If mouse event happened
	//if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )

	if( e->type == SDL_MOUSEBUTTONDOWN )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + BUTTON_WIDTH*2 )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + BUTTON_HEIGHT*2 )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			//mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
		    if( mCurrentSprite == quitbutton)
            {
                screen = START; // start = 0
                cout << "back";
                totalScore = 0;
                steps = 20;
            }
		}
	}

	if( e->type == SDL_MOUSEBUTTONDOWN && steps != 0 )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + BUTTON_WIDTH )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + BUTTON_HEIGHT )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			//mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
            previousX = mPosition.x;
            previousY = mPosition.y;
            initX = mPosition.x;
            initY = mPosition.y;
        //    cout << previousX << " " << previousY << endl;
            switch( mCurrentSprite )
            {
				case red:
                {
                    mCurrentSprite = redBright;
                    previousColor = 0;
                    break;
                }

				case yellow:
                {
                    mCurrentSprite = yellowBright;
                    previousColor = 1;
                    break;
                }

				case green:
                {
                    mCurrentSprite = greenBright;
                    previousColor = 2;
                    break;
                }


				case blue:
                {
                    mCurrentSprite = blueBright;
                    previousColor = 3;
                    break;
                }

			}
            clickNum++;
			mouseIsDown = true;
		//	cout << previousColor << endl;
		}


	}
        else if( e->type == SDL_MOUSEMOTION && mouseIsDown && steps != 0)
        {
                    //Get mouse position
                int x, y;
                SDL_GetMouseState( &x, &y );

                //Check if mouse is in button
                bool inside = true;

                //Mouse is left of the button
                if( x < mPosition.x )
                {
                    inside = false;
                }
                //Mouse is right of the button
                else if( x > mPosition.x + BUTTON_WIDTH )
                {
                    inside = false;
                }
                //Mouse above the button
                else if( y < mPosition.y )
                {
                    inside = false;
                }
                //Mouse below the button
                else if( y > mPosition.y + BUTTON_HEIGHT )
                {
                    inside = false;
                }

                //Mouse is outside button
                if( !inside )
                {
                    //mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
                }
                //Mouse is inside button
                else
                {
                    // check cycle
                    if( (abs(mPosition.x - initX) == 96 && abs(mPosition.y - initY) == 0 || abs(mPosition.x - initX) == 0 && abs(mPosition.y - initY) == 96) && clickNum >= 3 && cycle == false)
                    {

                        if(mCurrentSprite == previousColor)
                        {
                            cycleColor = mCurrentSprite;
                            cycle = true;
                            clickNum = 0;
                            switch( mCurrentSprite )
                            {
                                case red:
                                {
                                    if(previousColor == 0)
                                    {
                                        for(int i=0;i<TOTAL_BUTTONS;i++)
                                        {
                                            if(gButtons[i].mCurrentSprite == red || gButtons[i].mCurrentSprite == redBright)
                                            {
                                                gButtons[i].mCurrentSprite = redBright;
                                                clickNum++;
                                             //   cout << " click: " << clickNum;
                                            }

                                        }
                                    }
                                }


                                case yellow:
                                {
                                    if(previousColor == 1)
                                    {
                                        for(int i=0;i<TOTAL_BUTTONS;i++)
                                        {
                                            if(gButtons[i].mCurrentSprite == yellow || gButtons[i].mCurrentSprite == yellowBright)
                                             {
                                                gButtons[i].mCurrentSprite = yellowBright;
                                                clickNum++;
                                              //  cout << " click: " << clickNum;
                                             //   cout << "steps: " << steps;
                                             }
                                        }
                                    }
                                }

                                case green:
                                {
                                    if(previousColor == 2)
                                    {
                                        for(int i=0;i<TOTAL_BUTTONS;i++)
                                        {
                                            if(gButtons[i].mCurrentSprite == green || gButtons[i].mCurrentSprite == greenBright)
                                            {
                                                gButtons[i].mCurrentSprite = greenBright;
                                                clickNum++;
                                              //  cout << " click: " << clickNum;
                                            }
                                        }
                                    }
                                }


                                case blue:
                                {
                                    if(previousColor == 3)
                                    {
                                        for(int i=0;i<TOTAL_BUTTONS;i++)
                                        {
                                            if(gButtons[i].mCurrentSprite == blue || gButtons[i].mCurrentSprite == blueBright)
                                            {
                                                gButtons[i].mCurrentSprite = blueBright;
                                                clickNum++;
                                             //   cout << " click: " << clickNum;
                                            }
                                        }
                                    }
                                }

                            }
                        }

                    }

                  //  cout << abs(mPosition.x - previousX) << " " << abs(mPosition.y - previousY) << endl;
                    if( abs(mPosition.x - previousX) == 96 && abs(mPosition.y - previousY) == 0 || abs(mPosition.x - previousX) == 0 && abs(mPosition.y - previousY) == 96 && cycle == false)
                    {
                        switch( mCurrentSprite )
                        {
                            case red:
                            {
                                if(previousColor == 0)
                                {
                                    mCurrentSprite = redBright;
                                    clickNum++;
                                    mouseIsDown = true;
                                    previousX = mPosition.x;
                                    previousY = mPosition.y;
                                   break;
                                }
                                else
                                {
                                    previousX = 0;
                                    previousY = 0;
                                    previousColor = 9;
                                    mouseIsDown = false;
                                    clickNum = 0;
                                    break;
                                }
                            }


                            case yellow:
                            {
                                if(previousColor == 1)
                                {
                                    mCurrentSprite = yellowBright;
                                    clickNum++;
                                    mouseIsDown = true;
                                    previousX = mPosition.x;
                                    previousY = mPosition.y;
                                    break;
                                }
                                else
                                {
                                    previousX = 0;
                                    previousY = 0;
                                    previousColor = 9;
                                    mouseIsDown = false;
                                    clickNum = 0;
                                    break;
                                }
                            }

                            case green:
                            {
                                if(previousColor == 2)
                                {
                                    mCurrentSprite = greenBright;
                                    clickNum++;
                                    mouseIsDown = true;
                                    previousX = mPosition.x;
                                    previousY = mPosition.y;
                                    break;
                                }
                                else
                                {
                                    previousX = 0;
                                    previousY = 0;
                                    previousColor = 9;
                                    mouseIsDown = false;
                                    clickNum = 0;
                                    break;
                                }
                            }

                            case blue:
                            {
                                if(previousColor == 3)
                                {
                                    mCurrentSprite = blueBright;
                                    clickNum++;
                                    mouseIsDown = true;
                                    previousX = mPosition.x;
                                    previousY = mPosition.y;
                                    break;
                                }
                                else
                                {
                                    previousX = 0;
                                    previousY = 0;
                                    previousColor = 9;
                                    mouseIsDown = false;
                                    clickNum = 0;
                                    break;
                                }
                            }
                        }
                    }
                    else if( abs(mPosition.x - previousX) != 0 && abs(mPosition.y - previousY) != 0 && !cycle )
                    {
                        // reset all status
                        previousX = 0;
                        previousY = 0;
                        previousColor = 9;
                        mouseIsDown = false;
                        clickNum = 0;
                    }

                }
        }

        if( e->type == SDL_MOUSEBUTTONUP && clickNum >= 2 && steps != 0 )
        {
            for(int i=0;i<TOTAL_BUTTONS;i++)
            {
                if(gButtons[i].mCurrentSprite == redBright || gButtons[i].mCurrentSprite == yellowBright || gButtons[i].mCurrentSprite == greenBright || gButtons[i].mCurrentSprite == blueBright)
                {
                    gButtons[i].mCurrentSprite = white;
                }
                minusSteps = true;
            }

            if(!cycle)
            {
                drop(dot_ref);
                cout << "cycleColor:" << cycleColor << endl;
            }
            else
            {
                drop(dot_ref,cycleColor);
            }

            cycle = false;

            steps--;
            minusSteps = false;

            if( steps == 0 )
            {
                // calculate total score

            }
            // calculate score

            totalScore += point_transform(clickNum);

            cout << "total score: " << totalScore << endl;
            cout << "steps: " << steps << endl;

            // reset all status
            previousX = 0;
            previousY = 0;
            initX = 0;
            initY = 0;
            previousColor = 9;
            mouseIsDown = false;
            clickNum = 0;
        }

        else if( e->type == SDL_MOUSEBUTTONUP && clickNum < 2 )
        {
            // reset all status
            previousX = 0;
            previousY = 0;
            previousColor = 9;
            mouseIsDown = false;

            for(int i=0;i<TOTAL_BUTTONS;i++)
            {

                if(gButtons[i].mCurrentSprite == redBright)
                {
                    gButtons[i].mCurrentSprite = red;

                }

                else if(gButtons[i].mCurrentSprite == yellowBright)
                {
                    gButtons[i].mCurrentSprite = yellow;

                }

                else if(gButtons[i].mCurrentSprite == greenBright)
                {
                    gButtons[i].mCurrentSprite = green;

                }

                else if(gButtons[i].mCurrentSprite == blueBright)
                {
                    gButtons[i].mCurrentSprite = blue;

                }
            }

            // calculate score
            clickNum = 0;

            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            screen = LEVEL1;
            gLEVELSSpriteSheetTexture[LEVEL1].render( 0, 0);

            //Render buttons
            for( int i = 0; i < TOTAL_BUTTONS; ++i )
            {
                gButtons[ i ].render();
            }

            //Update screen
            SDL_RenderPresent( gRenderer );
        }
}

void LButton::render()
{
	//Show current button sprite
	gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

        //Open the font
    gFont = TTF_OpenFont( "16_true_type_fonts/lazy.ttf", 40 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Render text
        SDL_Color textColor = { 0, 0, 0 };
        Scoreban Score;
        Scoreban Steps;////
        if( !Score.S() )
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
        if(!gText.loadFromRenderedText( "Score", textColor ))
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
        ////
        if( !Steps.S() )
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
        if(!gText2.loadFromRenderedText( "Steps", textColor ))
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
        ////
    }
	//Load sprites
	if( !gButtonSpriteSheetTexture.loadFromFile( "image/all dots 48.png" ) )
	{
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	else
	{
		//Set sprites
		for( int i = 0; i < BUTTON_SPRITE_TOTAL-2; ++i )
        {
			gSpriteClips[ i ].x = 0;
			gSpriteClips[ i ].y = i*48;
			gSpriteClips[ i ].w = BUTTON_WIDTH;
			gSpriteClips[ i ].h = BUTTON_HEIGHT;
		}

			gSpriteClips[ 8 ].x = 0;
			gSpriteClips[ 8 ].y = 8*48+1;
			gSpriteClips[ 8 ].w = BUTTON_WIDTH;
			gSpriteClips[ 8 ].h = BUTTON_HEIGHT;

			gSpriteClips[ 9 ].x = 0;
			gSpriteClips[ 9 ].y = 9*48+1;
			gSpriteClips[ 9 ].w = BUTTON_WIDTH*2;
			gSpriteClips[ 9 ].h = BUTTON_HEIGHT*2;
      //  for(int i=0;i<25;i++)
      //      gButtons[i].setColor(i%4);
		//Set buttons
		for(int i=0;i<5;i++)
            for(int j=0;j<5;j++)
                gButtons[ 5*i+j ].setPosition( 324+96*i, 144+96*j );
	}

	gButtons[25].setPosition(114,520);

		//Load LEVELS sprites
	if( !gLEVELSSpriteSheetTexture[START].loadFromFile( "image/START_BG.png" ) )
	{
		printf( "Failed to load START_BG sprite texture!\n" );
		success = false;
	}
	if( !gLEVELSSpriteSheetTexture[LEVEL1].loadFromFile( "image/LEVEL1_BG.png" ) )
	{
		printf( "Failed to load LEVEL1_BG sprite texture!\n" );
		success = false;
	}
	if( !gLEVELSSpriteSheetTexture[LEVEL1_RED].loadFromFile( "image/LEVEL1_RED_BG.png" ) )
	{
		printf( "Failed to load LEVEL1_RED_BG sprite texture!\n" );
		success = false;
	}
	if( !gLEVELSSpriteSheetTexture[LEVEL1_YELLOW].loadFromFile( "image/LEVEL1_YELLOW_BG.png" ) )
	{
		printf( "Failed to load LEVEL1_YELLOW_BG sprite texture!\n" );
		success = false;
	}
	if( !gLEVELSSpriteSheetTexture[LEVEL1_GREEN].loadFromFile( "image/LEVEL1_GREEN_BG.png" ) )
	{
		printf( "Failed to load LEVEL1_GREEN_BG sprite texture!\n" );
		success = false;
	}
	if( !gLEVELSSpriteSheetTexture[LEVEL1_BLUE].loadFromFile( "image/LEVEL1_BLUE_BG.png" ) )
	{
		printf( "Failed to load LEVEL1_BLUE_BG sprite texture!\n" );
		success = false;
	}
	return success;
}

#endif // Two_Dots_algorithm
