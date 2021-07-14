#include "TextObject.h"


BaseObject::BaseObject() {
    rect_.x = 0;
    rect_.y = 0;
    p_object_ = NULL;
}

BaseObject::~BaseObject() {
    if (p_object_ != NULL) {
        SDL_FreeSurface(p_object_);
    }
}

bool BaseObject::LoadImg(std::string filename) {
    p_object_ = SDLCommonFunction::LoadImg(filename);
    if (p_object_ == NULL)
        return false;
    return true;
}

void BaseObject::show(SDL_Surface* des) {
    if (p_object_ != NULL) {
        rect_ = SDLCommonFunction::ApplySurface(p_object_, des, rect_.x, rect_.y);
    }
}

int TextObject::InitFont() {
    //==================Font==================
    if (TTF_Init() == -1) {
        cout << "\n=================Khong the Init Font=================\n";
        return false;
    }
    return 1;
    //Import font to Programming
}



void TextObject::setColor(const int& type) {
    if (type == RED_TEXT) {
        SDL_Color color = { 255, 0,0 };
        text_color = color;

    }
    else if (type == WHITE_TEXT) {
        SDL_Color color = { 255, 255, 255 };
        text_color = color;
    }
    else {
        SDL_Color color = { 0, 0,0 };
        text_color = color;
    }
}

void TextObject::createText(TTF_Font* font, SDL_Surface* des) {
    p_object_ = TTF_RenderText_Solid(font, str.c_str(), text_color);
    show(des);
}
