#include "UIGen.h"
#include "pch.h"
#include "Log.h"
#include <iostream>

void UIGen::uiInit()
{
	if (TTF_Init() < 0) // initiailise SDL_TTF
	{
		LOG_DEBUG("CAnnot initiallise TTF"); // if SDL_TTF returns -1 it is not initialised log error
	}
	else
	{
		LOG_DEBUG("TTF Initialised");
	}
}

void UIGen::quitUI()
{
	TTF_Quit(); // quit SDL_TTF 
}

TTF_Font* UIGen::openFont(const char* _file, int _size)
{
	TTF_Font* uiFont;
	uiFont = TTF_OpenFont(_file , 50); // open font file
	if (uiFont == NULL) // if it hasn't opened
	{ 
		//log error
		LOG_DEBUG("Cannot open font"); 
		std::cout << TTF_GetError();
	}
	return uiFont;
}

SDL_Surface* UIGen::renderUI(TTF_Font* _font, char _Red, char _Green, char _blue, char _alpha, char _backREd, char _backGreen, char _backBlue, char _backAlpha, std::string _message)
{
	SDL_Color fontCol = { _Red, _Green, _blue, _alpha }; // creat SDL_colour
	SDL_Color bckgCol = { _backREd, _backGreen, _backBlue, _backAlpha };
	SDL_Surface* resultText = nullptr; // create an SDL_Surface
	
	resultText = TTF_RenderText_Solid(_font, _message.c_str(), fontCol); // create an SDL_Surface using the font and message
	std::cout << TTF_GetError();
	if (resultText == NULL)
	{
		LOG_DEBUG("Unable to render"); // if surface can't be created log error
		std::cout << TTF_GetError();
	}

	return resultText;
}

void UIGen::renderText(SDL_Window* _window, TTF_Font* _font, char _Red, char _Green, char _blue, char _alpha, char _backREd, char _backGreen, char _backBlue, char _backAlpha, std::string _message, SDL_Renderer* _ren)
{
	SDL_Texture* tex = nullptr; // create SDL_Texture

	tex = SDL_CreateTextureFromSurface(_ren, renderUI(_font, _Red, _Green, _blue, _alpha, _backREd, _backGreen, _backBlue, _backAlpha, _message)); // create a texture from a surface

	SDL_Rect textPos; // create a rect
	textPos.x = 0;
	textPos.y = 0;
	textPos.w = 100;
	textPos.h = 100;

	SDL_QueryTexture(tex, NULL, NULL, &textPos.w, &textPos.h); // query the atribs of the texture
	std::cout << SDL_GetError();

	SDL_RenderClear(_ren); // clear render target
	SDL_RenderCopy(_ren, tex, NULL, &textPos); // add texture to render target
	SDL_RenderPresent(_ren); // render target
	//SDL_SetRenderTarget(_ren, tex);

	SDL_DestroyTexture(tex); // clear texture
}