#pragma once
#include <SDL_ttf.h>
#include <string>
class UIGen
{
public:
	void uiInit();
	void quitUI();
	TTF_Font* openFont(const char* _file, int _size);
	enum textQuaility {SOLID, SHADED, BLENDED};
	SDL_Surface* renderUI(TTF_Font* _font, char _Red, char _Green, char _blue, char _alpha, char _backREd, char _backGreen, char _backBlue, char _backAlpha, std::string _message);
	void renderText(SDL_Window* _window, TTF_Font* _font, char _Red, char _Green, char _blue, char _alpha, char _backREd, char _backGreen, char _backBlue, char _backAlpha, std::string _message, SDL_Renderer* _ren);
};

