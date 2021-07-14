#include "CommonFunction.h"

string image_background = "./background/background2.jpg";
string image_history = "./background/history_background2.jpg";
string history_path = "filepath.txt";
string editing_path = ".\\..\\..\\Manage_treem\\Children\\data.txt";

// NHOM HAM CHUC NANG
void SDLCommonFunction::menuInit() {
    g_bkground = NULL;
    menu_b = NULL;
    g_font_text = NULL;
    g_screen = NULL;
}

SDL_Surface* SDLCommonFunction::LoadImg(std::string file_path)
{
    SDL_Surface* load_image = load_image = IMG_Load(file_path.c_str());
    SDL_Surface* optimize_image = NULL;


    if (load_image != NULL) {
        optimize_image = SDL_DisplayFormat(load_image);
        SDL_FreeSurface(load_image);

        if (optimize_image != NULL)
        {
            UINT32 color_key = SDL_MapRGB(optimize_image->format, 0, 0x00, 0x00);	// ma mau xanh lam: 0,255,255
            SDL_SetColorKey(optimize_image, SDL_SRCCOLORKEY, color_key);
        }
    }

    return optimize_image;
}

SDL_Rect SDLCommonFunction::ApplySurface(SDL_Surface* src, SDL_Surface* des, int x, int y)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(src, NULL, des, &offset);

    return offset;

}

void SDLCommonFunction::CleanUp()
{
    SDL_FreeSurface(g_screen);
    SDL_FreeSurface(g_bkground);
    SDL_FreeSurface(menu_b);
}

bool SDLCommonFunction::checkfocuswithRect(const int& x, const int& y, const SDL_Rect& _rect) {
    if ((x >= _rect.x) && (x <= _rect.x + _rect.w) && (y >= _rect.y) && (y <= _rect.y + _rect.h))
        return true;
    return false;
}
// THAO TAC VOI FILE
void SDLCommonFunction::getFilePath() {
    //std::string path = "./history";
    std::string path = ".\\..\\..\\data\\history";
    DIR* directory = opendir(path.c_str());
    struct dirent* direntStruct;
    std::ofstream f;
    f.open(history_path);

    if (directory != NULL) {
        while (direntStruct = readdir(directory)) {
            //printf("File Name: %s\n", direntStruct->d_name); //If you are using <stdio.h>
            f << std::endl << direntStruct->d_name; //If you are using <iostream>

        }
    }
    closedir(directory);
    f.close();
}

vector<string> SDLCommonFunction::getFile(string filename) {
    vector<string>  temp;
    ifstream f;
    f.open(filename);

    string s;
    getline(f, s);
    getline(f, s);
    getline(f, s);

    while (!f.eof()) {
        getline(f, s);
        if (s[s.size() - 3] == 'j') {
            temp.push_back(s);
        }
    }

    f.close();
    return temp;
}

vector<string> SDLCommonFunction::getEditFile(string filename) {
    vector<string>  temp;
    ifstream f;
    f.open(filename);

    string s;

    while (!f.eof()) {
        getline(f, s);
        temp.push_back(s);
    }

    f.close();
    return temp;
}

void SDLCommonFunction::updateEditFile(string filename, vector<string> data) {
    ofstream f;
    f.open(filename, ios::out);

    f << data[0];
    for (int i = 1; i < data.size(); i++)
        f << endl << data[i];

    f.close();
}


// HAM KIEM TRA DUNG DO
void SDLCommonFunction::checkFlag(int& flag) {
    fstream flag_file;
    flag_file.open(".\\..\\..\\data\\flag.txt", ios::in);
    if (flag_file.is_open()) {
        string temp;
        getline(flag_file, temp);
        flag = stoi(temp);
        flag_file.close();
    }
}

void SDLCommonFunction::setFlag(int value) {
    fstream flag_file;
    flag_file.open(".\\..\\..\\data\\flag.txt", ios::out | ios::trunc);
    flag_file << value;
    flag_file.close();
}

// HAM THAO TAC VOI PHIM
void SDLCommonFunction::PrintModifiers(SDLMod mod) {
    printf("Modifers: ");

    /* If there are none then say so and return */
    if (mod == KMOD_NONE) {
        printf("None\n");
        return;
    }

    /* Check for the presence of each SDLMod value */
    /* This looks messy, but there really isn't    */
    /* a clearer way.                              */
    if (mod & KMOD_NUM) printf("NUMLOCK ");
    if (mod & KMOD_CAPS) printf("CAPSLOCK ");
    if (mod & KMOD_LCTRL) printf("LCTRL ");
    if (mod & KMOD_RCTRL) printf("RCTRL ");
    if (mod & KMOD_RSHIFT) printf("RSHIFT ");
    if (mod & KMOD_LSHIFT) printf("LSHIFT ");
    if (mod & KMOD_RALT) printf("RALT ");
    if (mod & KMOD_LALT) printf("LALT ");
    if (mod & KMOD_CTRL) printf("CTRL ");
    if (mod & KMOD_SHIFT) printf("SHIFT ");
    if (mod & KMOD_ALT) printf("ALT ");
    printf("\n");
}

string SDLCommonFunction::PrintKeyInfo(SDL_KeyboardEvent* key) {
    /* Is it a release or a press? */
    if (key->type == SDL_KEYUP)
        printf("Release:- ");
    else
        printf("Press:- ");

    /* Print the hardware scancode first */
    printf("Scancode: 0x%02X", key->keysym.scancode);
    /* Print the name of the key */
    string name = SDL_GetKeyName(key->keysym.sym);
    printf(", Name: %s", SDL_GetKeyName(key->keysym.sym));
    /* We want to print the unicode info, but we need to make */
    /* sure its a press event first (remember, release events */
    /* don't have unicode info                                */
    if (key->type == SDL_KEYDOWN) {
        /* If the Unicode value is less than 0x80 then the    */
        /* unicode value can be used to get a printable       */
        /* representation of the key, using (char)unicode.    */
        printf(", Unicode: ");
        if (key->keysym.unicode < 0x80 && key->keysym.unicode > 0) {
            printf("%c (0x%04X)", (char)key->keysym.unicode,
                key->keysym.unicode);
        }
        else {
            printf("? (0x%04X)", key->keysym.unicode);
        }
    }
    printf("\n");
    /* Print modifier info */
    PrintModifiers(key->keysym.mod);
    return name;
}

// NHOM HAM SHOW MENU

int  SDLCommonFunction::showMenu(SDL_Surface* des) {
    menu_b = SDLCommonFunction::LoadImg(image_background);
    if (menu_b == NULL) {
        std::cout << "\n==============Khong the load anh Menu. Kiem tra lai ten file va duong dan==============\n";
        return 0;
    }

    if (TTF_Init() == -1) {
        cout << "\n=================Khong the Init Font=================\n";
        return false;
    }

    g_font_text = TTF_OpenFont(font_local.c_str(), 40);
    if (g_font_text == NULL) {
        std::cout << "\n=================Loi font Menu=================\n";
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return 0;
    }


    SDL_Rect pos_Arr[kMenu];
    pos_Arr[0].x = x_menu_pixel;
    pos_Arr[0].y = y_menu_pixel;

    pos_Arr[1].x = x_menu_pixel;
    pos_Arr[1].y = y_menu_pixel + 40 + 10;

    pos_Arr[2].x = x_menu_pixel;
    pos_Arr[2].y = pos_Arr[1].y + 40 + 10;

    TextObject text_menu[kMenu];

    text_menu[0].setColor(TextObject::BLACK_TEXT);
    text_menu[0].setText("XEM LICH SU");
    text_menu[0].setRect(pos_Arr[0].x, pos_Arr[0].y);

    text_menu[1].setColor(TextObject::BLACK_TEXT);
    text_menu[1].setText("CHINH SUA");
    text_menu[1].setRect(pos_Arr[1].x, pos_Arr[1].y);

    text_menu[2].setColor(TextObject::BLACK_TEXT);
    text_menu[2].setText("THOAT");
    text_menu[2].setRect(pos_Arr[2].x, pos_Arr[2].y);

    bool selected[kMenu] = { 0, 0, 0 };
    int xm = 0;
    int ym = 0;
    SDL_Event m_event;
    while (true) {
        SDLCommonFunction::ApplySurface(menu_b, des, 0, 0);

        for (int i = 0; i < kMenu; i++) {
            text_menu[i].createText(g_font_text, des);
        }


        while (SDL_PollEvent(&m_event)) {
            switch (m_event.type)
            {
            case SDL_QUIT: {
                return kMenu - 1;
            }
            case SDL_MOUSEMOTION:
            {
                xm = m_event.motion.x;
                ym = m_event.motion.y;

                for (int i = 0; i < kMenu; i++) {
                    SDL_Rect rectmenu = text_menu[i].getRect();
                    if (SDLCommonFunction::checkfocuswithRect(xm, ym, rectmenu)) {
                        if (selected[i] == false) {
                            selected[i] = 1;
                            text_menu[i].setColor(TextObject::RED_TEXT);
                        }
                    }
                    else
                    {
                        if (selected[i] == true) {
                            selected[i] = 0;
                            text_menu[i].setColor(TextObject::BLACK_TEXT);
                        }
                    }
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                xm = m_event.button.x;
                ym = m_event.button.y;
                int i;
                for (i = 0; i < kMenu; i++) {

                    if (SDLCommonFunction::checkfocuswithRect(xm, ym, text_menu[i].getRect()) == true) {
                        return i;


                    }
                }

                break;

            }
            case SDL_KEYDOWN: {
                if (m_event.key.keysym.sym == SDLK_ESCAPE)
                    return kMenu - 1;
                break;
            }
            default:
                break;
            }
            SDL_Flip(des);
        }

    }
    return 0;
}

int SDLCommonFunction::showHistory(SDL_Surface* des) {
    if (TTF_Init() == -1) {
        cout << "\n=================Khong the Init Font=================\n";
        return false;
    }

    TTF_Font* font;
    font = TTF_OpenFont("./font/arial.ttf", 15);
    SDL_Surface* g_image_history;
    vector<string> file = SDLCommonFunction::getFile(history_path);
    int size = file.size();

    g_image_history = SDLCommonFunction::LoadImg(image_history);
    if (g_image_history == NULL)
    {
        cout << "Khong the tai hinh";
        return 0;
    }

    SDL_Rect rect;
    rect.x = 1000;
    rect.y = 550;

    TextObject* history = new TextObject[size + 1];
    history[0].setColor(TextObject::BLACK_TEXT);
    history[0].setText("QUAY LAI");
    history[0].setRect(rect.x, rect.y);


    history[1].setColor(TextObject::BLACK_TEXT);
    history[1].setText(file[0]);
    history[1].setRect(x_menu_history, y_menu_history);
    for (int i = 2; i < size + 1; i++) {
        history[i].setColor(TextObject::BLACK_TEXT);
        history[i].setText(file[i - 1]);
        if (history[i - 1].getRect().y + 24 <= 550)
            history[i].setRect(history[i - 1].getRect().x, history[i - 1].getRect().y + 20 + 4);
        else {
            history[i].setRect(history[i - 21].getRect().x + 200, history[i - 21].getRect().y);
        }

    }

    bool selected[100] = { 0 };
    int xm = 0;
    int ym = 0;
    SDL_Event m_event;
    while (true)
    {
        SDLCommonFunction::ApplySurface(g_image_history, des, 0, 0);
        history[0].createText(g_font_text, des);
        for (int i = 1; i < size + 1; i++) {
            history[i].createText(font, des);
        }
        while (SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
            case SDL_QUIT:
            {
                return size - 1;
            }
            case SDL_MOUSEMOTION:
            {
                xm = m_event.motion.x;
                ym = m_event.motion.y;

                for (int i = 0; i < size + 1; i++) {
                    SDL_Rect rectmenu = history[i].getRect();
                    if (SDLCommonFunction::checkfocuswithRect(xm, ym, rectmenu)) {
                        if (selected[i] == false) {
                            selected[i] = 1;
                            history[i].setColor(TextObject::RED_TEXT);
                        }
                    }
                    else {
                        if (selected[i] == true) {
                            selected[i] = 0;
                            history[i].setColor(TextObject::BLACK_TEXT);
                        }
                    }
                }

                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                xm = m_event.button.x;
                ym = m_event.button.y;
                for (int i = 0; i < size + 1; i++) {
                    SDL_Rect rectmenu = history[i].getRect();
                    if (SDLCommonFunction::checkfocuswithRect(xm, ym, rectmenu)) {
                        return i;
                    }
                }
                break;
            }
            case SDL_KEYDOWN:
            {
                if (m_event.key.keysym.sym == SDLK_ESCAPE) return size - 1;
                break;
            }
            default:
                break;
            }
            SDL_Flip(des);
        }


    }

    return 0;
}

int SDLCommonFunction::showImage(SDL_Surface* des, int i) {
    if (TTF_Init() == -1) {
        cout << "\n=================Khong the Init Font=================\n";
        return false;
    }

    SDL_Surface* g_image_history;
    vector<string> file = SDLCommonFunction::getFile(history_path);
    int size = file.size();

    g_image_history = SDLCommonFunction::LoadImg(image_history);
    if (g_image_history == NULL)
    {
        cout << "Khong the tai hinh";
        return 0;
    }

    SDL_Surface* image;
    string path = ".\\..\\..\\data\\history\\" + file[i - 1];
    image = SDLCommonFunction::LoadImg(path);


    SDL_Rect rect;
    rect.x = 1000;
    rect.y = 550;

    TextObject back;
    back.setColor(TextObject::BLACK_TEXT);
    back.setText("QUAY LAI");
    back.setRect(rect.x, rect.y);

    bool selected = 0;
    int xm = 0;
    int  ym = 0;
    SDL_Event m_event;
    while (true)
    {
        SDLCommonFunction::ApplySurface(g_image_history, des, 0, 0);
        back.createText(g_font_text, des);
        SDLCommonFunction::ApplySurface(image, des, 50, 50);

        while (SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
            case SDL_QUIT:
            {
                return size - 1;
            }
            case SDL_MOUSEMOTION:
            {
                xm = m_event.motion.x;
                ym = m_event.motion.y;


                SDL_Rect rectmenu = back.getRect();
                if (SDLCommonFunction::checkfocuswithRect(xm, ym, rectmenu)) {
                    if (selected == false) {
                        selected = 1;
                        back.setColor(TextObject::RED_TEXT);
                    }
                }
                else {
                    if (selected == true) {
                        selected = 0;
                        back.setColor(TextObject::BLACK_TEXT);
                    }
                }


                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                xm = m_event.button.x;
                ym = m_event.button.y;

                SDL_Rect rectmenu = back.getRect();
                if (SDLCommonFunction::checkfocuswithRect(xm, ym, rectmenu)) {
                    return 0;
                }

                break;
            }
            case SDL_KEYDOWN:
            {
                if (m_event.key.keysym.sym == SDLK_ESCAPE) return size - 1;
                break;
            }
            default:
                break;
            }
            SDL_Flip(des);
        }


    }
}

int SDLCommonFunction::showEdit(SDL_Surface* des) {
    if (TTF_Init() == -1) {
        cout << "\n=================Khong the Init Font=================\n";
        return false;
    }

    TTF_Font* font;
    font = TTF_OpenFont("./font/arial.ttf", 15);
    SDL_Surface* g_image_history;
    vector<string> file = SDLCommonFunction::getEditFile(editing_path);
    int size = file.size();

    g_image_history = SDLCommonFunction::LoadImg(image_history);
    if (g_image_history == NULL)
    {
        cout << "Khong the tai hinh";
        return 0;
    }

    SDL_Rect rect;
    rect.x = 1000;
    rect.y = 550;

    TextObject heading;
    heading.setColor(TextObject::BLACK_TEXT);
    heading.setText("BAM VAO DE CHINH SUA");
    heading.setRect(20, 20);


    TextObject* history = new TextObject[size + 2];
    history[0].setColor(TextObject::BLACK_TEXT);
    history[0].setText("QUAY LAI");
    history[0].setRect(rect.x, rect.y);

    history[1].setColor(TextObject::BLACK_TEXT);
    history[1].setText("THEM MOI");
    history[1].setRect(800, 550);


    history[2].setColor(TextObject::BLACK_TEXT);
    history[2].setText(file[0]);
    history[2].setRect(x_menu_history, y_menu_history);
    int j = 0;
    for (int i = 3; i < size + 2; i++) {
        history[i].setColor(TextObject::BLACK_TEXT);
        history[i].setText(file[i - 2]);
        if (history[i - 1].getRect().y + 24 <= 550)
            history[i].setRect(history[i - 1].getRect().x, history[i - 1].getRect().y + 20 + 4);
        else {
            history[i].setRect(history[i - 21].getRect().x + 200, history[i - 21].getRect().y);
        }

    }

    bool selected[100] = { 0 };
    int xm = 0;
    int  ym = 0;
    SDL_Event m_event;
    while (true)
    {
        SDLCommonFunction::ApplySurface(g_image_history, des, 0, 0);
        heading.createText(g_font_text, des);
        history[0].createText(g_font_text, des);
        history[1].createText(g_font_text, des);

        for (int i = 2; i < size + 2; i++) {
            history[i].createText(font, des);
        }
        while (SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
            case SDL_QUIT:
            {
                return size - 1;
            }
            case SDL_MOUSEMOTION:
            {
                xm = m_event.motion.x;
                ym = m_event.motion.y;

                for (int i = 0; i < size + 2; i++) {
                    SDL_Rect rectmenu = history[i].getRect();
                    if (SDLCommonFunction::checkfocuswithRect(xm, ym, rectmenu)) {
                        if (selected[i] == false) {
                            selected[i] = 1;
                            history[i].setColor(TextObject::RED_TEXT);
                        }
                    }
                    else {
                        if (selected[i] == true) {
                            selected[i] = 0;
                            history[i].setColor(TextObject::BLACK_TEXT);
                        }
                    }
                }

                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                xm = m_event.button.x;
                ym = m_event.button.y;
                for (int i = 0; i < size + 2; i++) {
                    SDL_Rect rectmenu = history[i].getRect();
                    if (SDLCommonFunction::checkfocuswithRect(xm, ym, rectmenu)) {
                        return i;
                    }
                }
                break;
            }
            case SDL_KEYDOWN:
            {
                if (m_event.key.keysym.sym == SDLK_ESCAPE) return size - 1;
                break;
            }
            default:
                break;
            }
            SDL_Flip(des);
        }


    }

    return 0;

}

int SDLCommonFunction::Edit(SDL_Surface* des, int i) {
    if (TTF_Init() == -1) {
        cout << "\n=================Khong the Init Font=================\n";
        return false;
    }

    SDL_Surface* g_image_history;
    vector<string> file = SDLCommonFunction::getEditFile(editing_path);
    int size = file.size();

    g_image_history = SDLCommonFunction::LoadImg(image_history);
    if (g_image_history == NULL)
    {
        cout << "Khong the tai hinh";
        return 0;
    }



    TextObject str;
    str.setColor(TextObject::BLACK_TEXT);
    str.setRect(600, 200);

    TextObject back;
    back.setColor(TextObject::BLACK_TEXT);
    back.setText("QUAY LAI");
    back.setRect(1000, 550);

    TextObject text;
    text.setColor(TextObject::BLACK_TEXT);
    text.setText("NHAP NOI DUNG CHINH SUA: ");
    text.setRect(100, 200);

    bool selected = 0;
    int xm = 0;
    int  ym = 0;
    SDL_Event m_event;
    string temp;
    SDLCommonFunction::ApplySurface(g_image_history, des, 0, 0);
    back.createText(g_font_text, des);
    text.createText(g_font_text, des);
    string s = "";
    while (true)
    {
        while (SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
            case SDL_QUIT:
            {
                return size - 1;
            }
            case SDL_MOUSEMOTION:
            {
                xm = m_event.motion.x;
                ym = m_event.motion.y;


                SDL_Rect rectmenu = back.getRect();
                if (SDLCommonFunction::checkfocuswithRect(xm, ym, rectmenu)) {
                    if (selected == false) {
                        selected = 1;
                        back.setColor(TextObject::RED_TEXT);
                    }
                }
                else {
                    if (selected == true) {
                        selected = 0;
                        back.setColor(TextObject::BLACK_TEXT);
                    }
                }


                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                xm = m_event.button.x;
                ym = m_event.button.y;

                SDL_Rect rectmenu = back.getRect();
                if (SDLCommonFunction::checkfocuswithRect(xm, ym, rectmenu)) {
                    return 0;
                }

                break;
            }
            case SDL_KEYUP: {
                temp = SDLCommonFunction::PrintKeyInfo(&m_event.key);

                if (temp == "backspace") {
                    if (s.size() != 0)
                        s.pop_back();
                }
                else if (temp == "space") {
                    s += " ";
                }
                else if (temp == "return") {
                    file[i - 2] = s;
                    SDLCommonFunction::updateEditFile(editing_path, file);
                    return 0;
                }
                else if (temp == ";") {
                    s += ':';
                }
                else s += toupper(temp[0]);
                SDLCommonFunction::ApplySurface(g_image_history, des, 0, 0);
                back.createText(g_font_text, des);
                text.createText(g_font_text, des);
                str.setText(s);
                str.createText(g_font_text, des);
                break;
            }
            case SDL_KEYDOWN:
            {
                if (m_event.key.keysym.sym == SDLK_ESCAPE) return size - 1;
                break;
            }
            default:
                break;
            }
            SDL_Flip(des);
        }


    }
    return 0;
}

int SDLCommonFunction::update(SDL_Surface* des) {
    if (TTF_Init() == -1) {
        cout << "\n=================Khong the Init Font=================\n";
        return false;
    }

    SDL_Surface* g_image_history;
    vector<string> file = SDLCommonFunction::getEditFile(editing_path);
    int size = file.size();

    g_image_history = SDLCommonFunction::LoadImg(image_history);
    if (g_image_history == NULL)
    {
        cout << "Khong the tai hinh";
        return 0;
    }



    TextObject str;
    str.setColor(TextObject::BLACK_TEXT);
    str.setRect(500, 200);

    TextObject back;
    back.setColor(TextObject::BLACK_TEXT);
    back.setText("QUAY LAI");
    back.setRect(1000, 550);

    TextObject text;
    text.setColor(TextObject::BLACK_TEXT);
    text.setText("NHAP NOI DUNG MOI: ");
    text.setRect(100, 200);

    bool selected = 0;
    int xm = 0;
    int  ym = 0;
    SDL_Event m_event;
    string temp;
    SDLCommonFunction::ApplySurface(g_image_history, des, 0, 0);
    back.createText(g_font_text, des);
    text.createText(g_font_text, des);
    string s = "";
    while (true)
    {
        while (SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
            case SDL_QUIT:
            {
                return size - 1;
            }
            case SDL_MOUSEMOTION:
            {
                xm = m_event.motion.x;
                ym = m_event.motion.y;


                SDL_Rect rectmenu = back.getRect();
                if (SDLCommonFunction::checkfocuswithRect(xm, ym, rectmenu)) {
                    if (selected == false) {
                        selected = 1;
                        back.setColor(TextObject::RED_TEXT);
                    }
                }
                else {
                    if (selected == true) {
                        selected = 0;
                        back.setColor(TextObject::BLACK_TEXT);
                    }
                }


                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                xm = m_event.button.x;
                ym = m_event.button.y;

                SDL_Rect rectmenu = back.getRect();
                if (SDLCommonFunction::checkfocuswithRect(xm, ym, rectmenu)) {
                    return 0;
                }

                break;
            }
            case SDL_KEYUP: {
                temp = SDLCommonFunction::PrintKeyInfo(&m_event.key);

                if (temp == "backspace") {
                    if (s.size() != 0)
                        s.pop_back();
                }
                else if (temp == "space") {                    
                    s += " ";                    
                }
                else if (temp == "return") {
                    file.push_back(s);
                    SDLCommonFunction::updateEditFile(editing_path, file);
                    return 0;
                }
                else if (temp == ";") {
                    s +=":";                    
                }
                else s += toupper(temp[0]);
                
                cout << "\n" << s << endl;
                SDLCommonFunction::ApplySurface(g_image_history, des, 0, 0);
                back.createText(g_font_text, des);
                text.createText(g_font_text, des);
                str.setText(s);
                str.createText(g_font_text, des);
                break;
            }
            case SDL_KEYDOWN:
            {
                if (m_event.key.keysym.sym == SDLK_ESCAPE) return size - 1;
                
                break;
            }
            default:
                break;
            }
            SDL_Flip(des);
        }
    }
    return 0;
}

int SDLCommonFunction::isWaiting(SDL_Surface* des) {
    if (TTF_Init() == -1) {
        cout << "\n=================Khong the Init Font=================\n";
        return false;
    }

    SDL_Surface* g_image_history;

    g_image_history = SDLCommonFunction::LoadImg(image_history);
    if (g_image_history == NULL)
    {
        cout << "Khong the tai hinh";
        return 0;
    }

    TextObject back;
    back.setColor(TextObject::BLACK_TEXT);
    back.setText("QUAY LAI");
    back.setRect(1000, 550);

    TextObject text;
    text.setColor(TextObject::BLACK_TEXT);
    text.setText("CO AI DO DANG SUA, QUAY LAI SAU!");
    text.setRect(100, 200);

    bool selected = 0;
    int xm = 0;
    int  ym = 0;
    SDL_Event m_event;


    while (true)
    {
        SDLCommonFunction::ApplySurface(g_image_history, des, 0, 0);
        text.createText(g_font_text, des);
        back.createText(g_font_text, des);
        while (SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
            case SDL_QUIT:
            {
                return  0;
            }
            case SDL_MOUSEMOTION:
            {
                xm = m_event.motion.x;
                ym = m_event.motion.y;


                SDL_Rect rectmenu = back.getRect();
                if (SDLCommonFunction::checkfocuswithRect(xm, ym, rectmenu)) {
                    if (selected == false) {
                        selected = 1;
                        back.setColor(TextObject::RED_TEXT);
                    }
                }
                else {
                    if (selected == true) {
                        selected = 0;
                        back.setColor(TextObject::BLACK_TEXT);
                    }
                }


                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                xm = m_event.button.x;
                ym = m_event.button.y;

                SDL_Rect rectmenu = back.getRect();
                if (SDLCommonFunction::checkfocuswithRect(xm, ym, rectmenu)) {
                    return 0;
                }

                break;
            }
            case SDL_KEYDOWN:
            {
                if (m_event.key.keysym.sym == SDLK_ESCAPE) return 0;
                break;
            }
            default:
                break;
            }
            SDL_Flip(des);
        }
    }
    return 0;
}