#include "CommonFunction.h"

#undef main




bool Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}
	g_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	if (g_screen == NULL)
	{
		return false;
	}


	return true;
}


int main(int arc, char* argv[])
{
	bool is_quit = false;
	if (Init() == false)
		return 0;
	SDLCommonFunction::getFilePath();
	int flag;


Menu:
	int choice = SDLCommonFunction::showMenu(g_screen);

	if (choice == kMenu - 1) {
		return 0;
	}
	else if (choice == 0) {
		// Xem lich su
		SDLCommonFunction::getFilePath();
		goto History;
	}
	else if (choice == 1) {
		// Chinh sua
		SDLCommonFunction::checkFlag(flag);
		if (flag == 0) {
			SDLCommonFunction::setFlag(1);
			goto Edit;
		}
		else
			goto Waiting;

	}
	else return 0;



History:
	{
		int history_choice = SDLCommonFunction::showHistory(g_screen);
		if (history_choice == 0)
			goto Menu;
		else {
			int history_image_choice = SDLCommonFunction::showImage(g_screen, history_choice);
			if (history_image_choice == 0)
				goto History;
		}
	}


Edit:
	{
		int edit_choice = SDLCommonFunction::showEdit(g_screen);
		if (edit_choice == 0) {
			SDLCommonFunction::setFlag(0);
			goto Menu;
		}
		else if (edit_choice == 1) {
			int update_choice = SDLCommonFunction::update(g_screen);
			if (update_choice == 0)
				goto Edit;
		}
		else {
			int edit_choice_menu = SDLCommonFunction::Edit(g_screen, edit_choice);
			if (edit_choice_menu == 0)
				goto Edit;
		}


	}

Waiting:
	{
		int waiting_choice = SDLCommonFunction::isWaiting(g_screen);
		if (waiting_choice == 0)
			goto Menu;
	}


End:
	{
		SDLCommonFunction::CleanUp();
		SDL_Quit();
		return 1;
	}
}


