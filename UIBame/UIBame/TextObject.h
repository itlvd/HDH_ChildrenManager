#ifndef _TEXTOBJECT_H_
#define _TEXTOBJECT_H_
#include "CommonFunction.h"
#include <SDL_ttf.h>

static TTF_Font* g_font_text;

class BaseObject
{
protected:
	SDL_Rect rect_;
	SDL_Surface* p_object_;
public:
	BaseObject();
	~BaseObject();

	void show(SDL_Surface* des);
	bool LoadImg(std::string filename);

	void setRect(const int& x, const int& y) { rect_.x = x, rect_.y = y; }
	SDL_Rect getRect() const { return rect_; }
	SDL_Surface* getObject() { return p_object_; }

};

class TextObject : public BaseObject
{
private:
	std::string str;
	SDL_Color text_color;

public:
	enum TextColor
	{
		RED_TEXT = 0,
		WHITE_TEXT = 1,
		BLACK_TEXT = 2,
	};

	void setText(const std::string& text) {
		str = text;
	}

	void setColor(const int& type);

	static int InitFont();

	void createText(TTF_Font* font, SDL_Surface* des);

};

#endif 
