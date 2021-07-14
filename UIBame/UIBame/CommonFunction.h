#ifndef _COMMONFUNCTION_H_
#define _COMMONFUNCTION_H_



#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>


#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "dirent.h"
#include "TextObject.h"

using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

static SDL_Surface* g_screen = NULL;
static SDL_Surface* g_bkground = NULL;
static SDL_Event g_even;

static SDL_Surface* menu_b;


//Menu
const string font_local = "./font/menu.ttf";
const int kMenu = 3;
const int x_menu_pixel = 500;
const int y_menu_pixel = 200;

const int x_menu_history = 50;
const int y_menu_history = 50;

namespace SDLCommonFunction {
	void menuInit();
	SDL_Rect ApplySurface(SDL_Surface* src, SDL_Surface* des, int x, int y);
	void CleanUp();
	bool checkfocuswithRect(const int& x, const int& y, const SDL_Rect& _rect);
	SDL_Surface* LoadImg(std::string file_path);
	int showMenu(SDL_Surface* des);

	void getFilePath();
	// Show Menu
	int showHistory(SDL_Surface* des);
	int showImage(SDL_Surface* des, int i);
	int showEdit(SDL_Surface* des);
	int Edit(SDL_Surface* des, int i);
	string handleInput(SDL_Surface* des);
	int update(SDL_Surface* des);
	int isWaiting(SDL_Surface* des);
	// Check collision
	void checkFlag(int& flag);
	void setFlag(int value);

	void PrintModifiers(SDLMod mod);
	string PrintKeyInfo(SDL_KeyboardEvent* key);

	// File
	vector<string> getFile(string filename);
	vector<string> getEditFile(string filename);
	void updateEditFile(string filename, vector<string> data);
}

#endif 