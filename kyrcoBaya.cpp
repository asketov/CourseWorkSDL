#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <ctime>
#include <SDL_ttf.h>
#include <fstream>
#include <string>
using namespace std;

const int WIDTH = 600, HEIGHT = 800; //площадь экрана
struct gamer //один игрок
{
    char nick[10];
    double timeGame;
};

SDL_Window* window1 = SDL_CreateWindow("kyrcoBaya", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
SDL_Renderer* renderer = SDL_CreateRenderer(window1, 0, 0);

class Hostess //домохозяйка
{
public:
    SDL_Rect hostessPosition;
    SDL_Texture* hostess_texture;
    bool turn;
    bool left;
    bool upper;
    Hostess()
    {
        upper = false;
        turn = false;
        left = false;
        hostessPosition = { 250, 250, 64, 146 };
        SDL_Surface* hostes = IMG_Load("hostess.png");
        if (!hostes) cout << "image not open(hostess)\n";
        hostess_texture = SDL_CreateTextureFromSurface(renderer, hostes);
        SDL_RenderCopy(renderer, hostess_texture, NULL, &hostessPosition);
    }
};

class Chair //стул
{
public:
    SDL_Rect chairPosition;
    SDL_Texture* chair_texture;
    bool turn;
    Chair(int x=400, int y=600,const char image[] = "chair.png")
    {
        chairPosition.w = 200; chairPosition.x = x; chairPosition.y = y; chairPosition.h = 200;
        SDL_Surface* chairr = IMG_Load(image);
        if (!chairr) cout << "image not open(chair)\n";
        chair_texture = SDL_CreateTextureFromSurface(renderer, chairr);
        SDL_RenderCopy(renderer, chair_texture, NULL, &chairPosition);
        turn = false;
    }
    void flip()
    {
        if (turn == false) turn = true;
        else turn = false;
    }
    void introduce()
    {
        if (turn == true)
        {
            SDL_RenderCopyEx(renderer, chair_texture, 0, &chairPosition, 90, NULL, SDL_FLIP_NONE);
        }
        else SDL_RenderCopy(renderer, chair_texture, NULL, &chairPosition);
    }
};

class Cupboard //шкаф
{
public:
    SDL_Rect cupboardPosition;
    SDL_Texture* cupboard_texture;
    const char* img;
    bool opened;
    Cupboard(const char* img2 = "cupboard_close.png",int x=30,int y=195,int w=125,int h=104)
    {
        cupboardPosition = { x,y,w,h };
        cupboardPosition.x = x; cupboardPosition.y = y;
        SDL_Surface* Cupboard = IMG_Load(img2);
        if (!Cupboard) cout << "image not open(cupboard_cl)\n";
        cupboard_texture = SDL_CreateTextureFromSurface(renderer, Cupboard);
        SDL_RenderCopy(renderer, cupboard_texture, NULL, &cupboardPosition);
        opened = false;
        img = img2;
    }
    void open(const char* img1)
    {
        if (opened == false)
        {
            opened = true;
            if (img1 == "cupboard_open1.png") cupboardPosition.x -= 22;
            else
            {
                cupboardPosition.x += 8;
                cupboardPosition.y += 6;
            }
            SDL_Surface* Cupboard = IMG_Load(img1);
            if (!Cupboard) cout << "image not open(cupboard_cl)\n";
            cupboard_texture = SDL_CreateTextureFromSurface(renderer, Cupboard);
        }
        else
        {
            if (img == "cupboard_close1.png") cupboardPosition.x += 22;
            else 
            { 
                cupboardPosition.x -= 8;
                cupboardPosition.y -= 6;
            }
            SDL_Surface* Cupboard = IMG_Load(img);
            if (!Cupboard) cout << "image not open(cupboard_cl)\n";
            cupboard_texture = SDL_CreateTextureFromSurface(renderer, Cupboard);
            opened = false;
        }
    }
    
};

class Window //окно
{
public:
    SDL_Rect windowPosition;
    SDL_Texture* window_texture;
    bool opened;
    Window()
    {
        windowPosition = { 425, 70, 110, 130 };
        SDL_Surface* Window = IMG_Load("win_close.png");
        if (!Window) cout << "image not open(window_cl)\n";
        window_texture = SDL_CreateTextureFromSurface(renderer, Window);
        SDL_RenderCopy(renderer, window_texture, NULL, &windowPosition);
        opened = false;
    }
    void open()
    {
        if (opened == false)
        {
            SDL_Surface* Window = IMG_Load("win_open.png");
            if (!Window) cout << "image not open(window_cl)\n";
            window_texture = SDL_CreateTextureFromSurface(renderer, Window);
            opened = true;
        }
        else
        {
            SDL_Surface* Window = IMG_Load("win_close.png");
            if (!Window) cout << "image not open(window_cl)\n";
            window_texture = SDL_CreateTextureFromSurface(renderer, Window);
            opened = false;
        }
    }
};


int main(int argc, char* argv[])
{
    system("cls");
    setlocale(0, "Russian");
    unsigned short int x;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Color blackColor = { 0, 0, 0, 255 };
    struct gamer Gamer,introduce;
    long double time, n = 7, all, timeGameBegin;
    int levelup = 10;
    SDL_Rect menuPosition = { 0,0,600,800 };
    SDL_Event event;
    TTF_Init();
    TTF_Font* Roboto = TTF_OpenFont("Roboto-Regular.ttf", 96);
    SDL_Surface* menu = IMG_Load("menu.png");
    SDL_Surface* zactavka = IMG_Load("zactavka.png");
    SDL_Surface* input = IMG_Load("inpute.png");
    SDL_Surface* rools = IMG_Load("rools.png");
    SDL_Surface* chart = IMG_Load("chart.png");
    SDL_Texture* chart_t = SDL_CreateTextureFromSurface(renderer, chart);
    SDL_Texture* rools_t = SDL_CreateTextureFromSurface(renderer, rools);
    SDL_Texture* input_t = SDL_CreateTextureFromSurface(renderer, input);
    SDL_Texture* zactavka_t = SDL_CreateTextureFromSurface(renderer, zactavka);
    SDL_Texture* menu_texture = SDL_CreateTextureFromSurface(renderer, menu);
    const SDL_Rect bedPosition = { 475,350,140,237 };
    SDL_Surface* bed = IMG_Load("bed.png");
    SDL_Texture* bed_texture = SDL_CreateTextureFromSurface(renderer, bed);
    const SDL_Rect tablePosition = { 250,640,200,200 };
    SDL_Surface* table = IMG_Load("table.png");
    SDL_Texture* table_texture = SDL_CreateTextureFromSurface(renderer, table);
    SDL_Rect fonPosition = { 0,0,800,800 };
    SDL_Surface* fon = IMG_Load("fon.png");
    SDL_Texture* fon_texture = SDL_CreateTextureFromSurface(renderer, fon);
    SDL_RenderCopy(renderer, zactavka_t, NULL, &menuPosition);
    SDL_RenderPresent(renderer);
    while ((int)(clock()/1000.0) < 3)
    {
    }
    SDL_RenderCopy(renderer, input_t, NULL, &menuPosition);
    SDL_RenderPresent(renderer);
    cout << "Введите ник, не больше 10 символов:";
    cin >> Gamer.nick;
    SDL_bool done = SDL_TRUE;
    while (done)
    {
        while (SDL_PollEvent(&event)) 
        {
            SDL_RenderCopy(renderer, menu_texture, NULL, &menuPosition);
            SDL_RenderPresent(renderer);
            switch (event.type)
            {
            case SDL_QUIT:
                return 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    int x = event.button.x;
                    int y = event.button.y;
                    if (x > 167 && x < 410 && y>150 && y < 210) //играть
                    {
                        done = SDL_FALSE;
                    }
                    if (x > 167 && x < 410 && y>545 && y < 605) //выход
                    {
                        return 0;
                    }
                    if (x > 167 && x < 410 && y>344 && y < 404) //таблица
                    {
                        SDL_RenderCopy(renderer, chart_t, NULL, &menuPosition);
                        ifstream fout("results.bin", istream::binary); //открытие потока для чтения
                        if (fout) //если открылся поток
                        {
                            SDL_Rect dstrect = { 53,29,87,46 };
                            for (int i = 0; i < 10 && fout.read((char*)&introduce, sizeof(introduce));i++) //читаем файл
                            {
                                SDL_Surface* surfaceMessageNick = TTF_RenderText_Solid(Roboto, introduce.nick, blackColor);
                                SDL_Surface* surfaceMessageTime = TTF_RenderText_Solid(Roboto, to_string(introduce.timeGame).c_str(), blackColor);
                                SDL_Texture* nickname = SDL_CreateTextureFromSurface(renderer, surfaceMessageNick);
                                SDL_Texture* timegame = SDL_CreateTextureFromSurface(renderer, surfaceMessageTime);
                                SDL_RenderCopy(renderer, nickname, NULL, &dstrect);
                                dstrect.x += 250;
                                SDL_RenderCopy(renderer, timegame, NULL, &dstrect);
                                dstrect.x -= 250;
                                dstrect.y += 40;
                            }
                            fout.close();
                        }
                        SDL_RenderPresent(renderer);
                        done = SDL_FALSE;
                        while(!done)
                        { 
                            while (SDL_PollEvent(&event))
                            {
                                switch (event.type)
                                {
                                case SDL_QUIT:
                                    return 0;
                                    break;
                                case SDL_MOUSEBUTTONDOWN:
                                    switch (event.button.button)
                                    {
                                    case SDL_BUTTON_LEFT:
                                        int x = event.button.x;
                                        int y = event.button.y;
                                        if (y > 521 && y < 563 && x>0 && x < 127) done = SDL_TRUE; //выход из таблицы
                                        if (y > 533 && y < 578 && x>130 && x < 305) //очистка файла
                                        {
                                            ofstream fout("results.bin", ios::trunc);
                                            fout.close();
                                            SDL_RenderCopy(renderer, chart_t, NULL, &menuPosition);
                                            SDL_RenderPresent(renderer);
                                        }
                                        break;
                                    }
                                    break;
                                }
                            }
                        }

                    }
                    if (x > 167 && x < 410 && y>240 && y < 300) //ввод ника
                    {
                        system("cls");
                        SDL_RenderCopy(renderer, input_t, NULL, &menuPosition); 
                        SDL_RenderPresent(renderer);
                        cout << "Введите ник, не больше 10 символов:";
                        cin >> Gamer.nick;
                    }
                    if (x > 167 && x < 410 && y < 515 && y>455) //правила
                    {
                        SDL_RenderCopy(renderer, rools_t, NULL, &menuPosition); 
                        SDL_RenderPresent(renderer);
                        time = clock()/1000.0;
                        done = SDL_FALSE;
                        while (!done)
                        {
                            while (SDL_PollEvent(&event))
                            {
                                switch (event.type)
                                {
                                case SDL_QUIT:
                                    return 0;
                                    break;
                                case SDL_MOUSEBUTTONDOWN:
                                    switch (event.button.button)
                                    {
                                    case SDL_BUTTON_LEFT:
                                        int x = event.button.x;
                                        int y = event.button.y;
                                        if (y > 533 && y < 578 && x>189 && x < 291) done = SDL_TRUE;
                                        break;   
                                    }
                                    break;
                                }
                            }
                        }
                    }

                }
            }
        }

    }
    SDL_RenderCopy(renderer, fon_texture, NULL, &fonPosition); //фон
    class Chair chairLeft(100,640), chairRight(400,640,"chair1.png");
    class Window window;
    class Cupboard cupboard,cupboard1("cupboard_close1.png",190,90,200,200);
    SDL_RenderCopy(renderer, table_texture, NULL, &tablePosition);
    SDL_RenderCopy(renderer, bed_texture, NULL, &bedPosition);
    class Hostess hostess;
    SDL_RenderPresent(renderer);
    timeGameBegin = clock() / 1000.0;
    all = clock() / 1000.0 + n;
    while (!done) //сама игра
    {
        time = clock() / 1000.0;
        if (clock() / 1000.0 > all)
        {
            all = clock() / 1000.0 + n;
            x = rand() % 32767;
            if (x < 6553) if (chairLeft.turn == false) chairLeft.flip();
            if (x > 6553 && x < 13106) if (chairRight.turn == false) chairRight.flip();
            if (x > 13106 && x < 19659) if (window.opened == false) window.open();
            if (x > 19659 && x < 26000) if (cupboard.opened == false) cupboard.open("cupboard_open.png");
            if (x > 26000 && x < 32767) if (cupboard1.opened == false) cupboard1.open("cupboard_open1.png");
        }
        if ((int)time > levelup) //увеличение уровня с каждым разом
        {
            levelup += 30;
             if(n!=1) n--;
        }
        SDL_RenderCopy(renderer, fon_texture, NULL, &fonPosition);
        SDL_RenderCopy(renderer, cupboard.cupboard_texture, NULL, &cupboard.cupboardPosition);
        SDL_RenderCopy(renderer, cupboard1.cupboard_texture, NULL, &cupboard1.cupboardPosition);
        SDL_RenderCopy(renderer, window.window_texture, NULL, &window.windowPosition);
        if (hostess.upper == true) SDL_RenderCopy(renderer, bed_texture, NULL, &bedPosition);
        if (hostess.left == false)
        {
            SDL_RenderCopy(renderer, hostess.hostess_texture, NULL, &hostess.hostessPosition);
        }
        else
        {
            SDL_RenderCopyEx(renderer, hostess.hostess_texture, 0, &hostess.hostessPosition, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
        if (hostess.upper == false) SDL_RenderCopy(renderer, bed_texture, NULL, &bedPosition);
        chairLeft.introduce();
        chairRight.introduce();
        SDL_RenderCopy(renderer, table_texture, NULL, &tablePosition);
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&event)) {
            switch (event.type)
            { 
             case SDL_QUIT:
                done = SDL_TRUE;
                break;
             case SDL_KEYDOWN:
                 SDL_RenderCopy(renderer, fon_texture, NULL, &fonPosition);
                 switch (event.key.keysym.sym)
                 {
                 case SDLK_SPACE:
                     if (hostess.hostessPosition.x > 50 && hostess.hostessPosition.x < 240 && hostess.hostessPosition.y < 800 && hostess.hostessPosition.y >400)
                         chairLeft.flip();
                     if (hostess.hostessPosition.x > 300 && hostess.hostessPosition.x < 550 && hostess.hostessPosition.y < 800 && hostess.hostessPosition.y >400)
                         chairRight.flip();
                     if (hostess.hostessPosition.x < 500 && hostess.hostessPosition.x > 400 && hostess.hostessPosition.y > 100 && hostess.hostessPosition.y < 175)
                     {
                         window.open();
                         SDL_RenderCopy(renderer, window.window_texture, NULL, &window.windowPosition);
                     }
                     else SDL_RenderCopy(renderer, window.window_texture, NULL, &window.windowPosition);
                     if (hostess.hostessPosition.x < 300 && hostess.hostessPosition.x > 165 && hostess.hostessPosition.y > 100 && hostess.hostessPosition.y < 175)
                     {
                         cupboard1.open("cupboard_open1.png");
                         SDL_RenderCopy(renderer, cupboard1.cupboard_texture, NULL, &cupboard1.cupboardPosition);
                     }
                     else SDL_RenderCopy(renderer, cupboard1.cupboard_texture, NULL, &cupboard1.cupboardPosition);
                     if (hostess.hostessPosition.x < 100 && hostess.hostessPosition.x > 30 && hostess.hostessPosition.y > 100 && hostess.hostessPosition.y < 175)
                     {
                         cupboard.open("cupboard_open.png");
                         SDL_RenderCopy(renderer, cupboard.cupboard_texture, NULL, &cupboard.cupboardPosition);
                     }
                     else SDL_RenderCopy(renderer, cupboard.cupboard_texture, NULL, &cupboard.cupboardPosition);
                     if (hostess.hostessPosition.y > 400 && hostess.hostessPosition.y < 800 && hostess.hostessPosition.x < 550 && hostess.hostessPosition.x > 425)
                     {
                         SDL_RenderCopy(renderer, bed_texture, NULL, &bedPosition);
                         hostess.upper = true;
                     }
                     else hostess.upper = false;
                     if (hostess.left == false)
                     {
                         SDL_RenderCopy(renderer, hostess.hostess_texture, NULL, &hostess.hostessPosition);
                     }
                     else
                     {
                         SDL_RenderCopyEx(renderer, hostess.hostess_texture, 0, &hostess.hostessPosition, 0, NULL, SDL_FLIP_HORIZONTAL);
                     }
                     break;
                 case SDLK_UP:
                     if (hostess.hostessPosition.y > 155) hostess.hostessPosition.y -= 8;
                     if (hostess.hostessPosition.y > 400 && hostess.hostessPosition.y < 800 && hostess.hostessPosition.x < 550 && hostess.hostessPosition.x > 425)
                         hostess.upper = true;
                     else hostess.upper = false;
                     if (hostess.hostessPosition.y > 400 && hostess.hostessPosition.y < 450 && hostess.hostessPosition.x < 550 && hostess.hostessPosition.x > 425)
                         hostess.hostessPosition.y += 8;
                     SDL_RenderCopy(renderer, cupboard.cupboard_texture, NULL, &cupboard.cupboardPosition);
                     SDL_RenderCopy(renderer, cupboard1.cupboard_texture, NULL, &cupboard1.cupboardPosition);
                     SDL_RenderCopy(renderer, window.window_texture, NULL, &window.windowPosition);
                     if (hostess.upper == true) SDL_RenderCopy(renderer, bed_texture, NULL, &bedPosition);
                     if (hostess.left == false)
                     {
                         SDL_RenderCopy(renderer, hostess.hostess_texture, NULL, &hostess.hostessPosition);
                     }
                     else
                     {
                         SDL_RenderCopyEx(renderer, hostess.hostess_texture, 0, &hostess.hostessPosition, 0, NULL, SDL_FLIP_HORIZONTAL);
                     }
                     break;
                   case SDLK_DOWN:
                       SDL_RenderCopy(renderer, cupboard.cupboard_texture, NULL, &cupboard.cupboardPosition);
                       SDL_RenderCopy(renderer, cupboard1.cupboard_texture, NULL, &cupboard1.cupboardPosition);
                       SDL_RenderCopy(renderer, window.window_texture, NULL, &window.windowPosition);
                       if (hostess.hostessPosition.y < 600) hostess.hostessPosition.y += 8;
                       else if (hostess.hostessPosition.y < 650 && hostess.hostessPosition.x < 90) hostess.hostessPosition.y += 8;
                       if(hostess.hostessPosition.y > 295 && hostess.hostessPosition.y < 375 && hostess.hostessPosition.x < 550 && hostess.hostessPosition.x > 425) hostess.hostessPosition.y -= 8;
                       if (hostess.hostessPosition.y > 400 && hostess.hostessPosition.y < 800 && hostess.hostessPosition.x < 550 && hostess.hostessPosition.x > 425)
                           hostess.upper = true;
                       else hostess.upper = false;
                     if (hostess.upper == true) SDL_RenderCopy(renderer, bed_texture, NULL, &bedPosition);
                     if (hostess.left == false)
                     {
                         SDL_RenderCopy(renderer, hostess.hostess_texture, NULL, &hostess.hostessPosition);
                     }
                     else
                     {
                         SDL_RenderCopyEx(renderer, hostess.hostess_texture, 0, &hostess.hostessPosition, 0, NULL, SDL_FLIP_HORIZONTAL);
                     }
                     break;
                   case SDLK_RIGHT:
                       SDL_RenderCopy(renderer, cupboard.cupboard_texture, NULL, &cupboard.cupboardPosition);
                       SDL_RenderCopy(renderer, cupboard1.cupboard_texture, NULL, &cupboard1.cupboardPosition);
                       SDL_RenderCopy(renderer, window.window_texture, NULL, &window.windowPosition);
                       if (hostess.hostessPosition.x < 540) hostess.hostessPosition.x += 8;
                       if (hostess.hostessPosition.x > 80 && hostess.hostessPosition.x < 90 && hostess.hostessPosition.y < 700 && hostess.hostessPosition.y > 610)
                                   hostess.hostessPosition.x -= 8;
                       if (hostess.hostessPosition.x > 420 && hostess.hostessPosition.x < 540 && hostess.hostessPosition.y < 450 && hostess.hostessPosition.y > 295)
                                   hostess.hostessPosition.x -= 8;
                       if (hostess.hostessPosition.y > 400 && hostess.hostessPosition.y < 800 && hostess.hostessPosition.x < 550 && hostess.hostessPosition.x > 425)
                                   hostess.upper = true;
                       else hostess.upper = false;
                        if(hostess.upper == true) SDL_RenderCopy(renderer, bed_texture, NULL, &bedPosition);
                       hostess.left = false;
                       if (hostess.turn == true)
                       {
                           SDL_RenderCopyEx(renderer, hostess.hostess_texture, 0, &hostess.hostessPosition, 0, NULL, SDL_FLIP_HORIZONTAL);
                           hostess.turn == false;
                       }
                       else SDL_RenderCopy(renderer, hostess.hostess_texture, NULL, &hostess.hostessPosition);
                       break;
                   case SDLK_LEFT:
                       SDL_RenderCopy(renderer, cupboard.cupboard_texture, NULL, &cupboard.cupboardPosition);
                       SDL_RenderCopy(renderer, cupboard1.cupboard_texture, NULL, &cupboard1.cupboardPosition);
                       SDL_RenderCopy(renderer, window.window_texture, NULL, &window.windowPosition);
                       if(hostess.hostessPosition.x > 0) hostess.hostessPosition.x -= 8;
                       if (hostess.hostessPosition.y > 400 && hostess.hostessPosition.y < 800 && hostess.hostessPosition.x < 550 && hostess.hostessPosition.x > 425)
                           hostess.upper = true;
                       else hostess.upper = false;
                       if (hostess.upper == true) SDL_RenderCopy(renderer, bed_texture, NULL, &bedPosition);
                       hostess.left = true;
                       if (hostess.turn == false)
                       {
                           SDL_RenderCopyEx(renderer, hostess.hostess_texture, 0, &hostess.hostessPosition, 0, NULL, SDL_FLIP_HORIZONTAL);
                           hostess.turn == true;
                       }
                       else SDL_RenderCopy(renderer, hostess.hostess_texture, NULL, &hostess.hostessPosition);
                       break;
                   default:
                       SDL_RenderCopy(renderer, cupboard.cupboard_texture, NULL, &cupboard.cupboardPosition);
                       SDL_RenderCopy(renderer, cupboard1.cupboard_texture, NULL, &cupboard1.cupboardPosition);
                       SDL_RenderCopy(renderer, window.window_texture, NULL, &window.windowPosition);
                       if (hostess.upper == true) SDL_RenderCopy(renderer, bed_texture, NULL, &bedPosition);
                       if (hostess.left == false)
                       {
                           SDL_RenderCopy(renderer, hostess.hostess_texture, NULL, &hostess.hostessPosition);
                       }
                       else
                       {
                           SDL_RenderCopyEx(renderer, hostess.hostess_texture, 0, &hostess.hostessPosition, 0, NULL, SDL_FLIP_HORIZONTAL);
                       }
                       if (hostess.upper == true) SDL_RenderCopy(renderer, bed_texture, NULL, &bedPosition);
                       SDL_RenderPresent(renderer);
                       break;
                 }
                 if (hostess.upper == false) SDL_RenderCopy(renderer, bed_texture, NULL, &bedPosition);
                 chairLeft.introduce();
                 chairRight.introduce();
                 SDL_RenderCopy(renderer, table_texture, NULL, &tablePosition);
                 SDL_RenderPresent(renderer);
                 break;
                 default:
                   break;
            }
        }
        if (chairLeft.turn && chairRight.turn && cupboard.opened && cupboard1.opened && window.opened) //если все открыто и перевернуто
        {
            Gamer.timeGame = clock() / 1000.0 - timeGameBegin;
            int i = 0;
            struct gamer gamers[10];
            ifstream fout("results.bin", ostream::binary); //открытие потока для вывода
            for (; i < 10 && fout.read((char*)&introduce, sizeof(introduce)); i++) //читаем файл
            {
                strcpy_s(gamers[i].nick, strlen(introduce.nick) + 1, introduce.nick);
                gamers[i].timeGame = introduce.timeGame;
            }
            fout.close();
            ofstream f("results.bin", ostream::binary); //открытие потока для вывода
            double min;
            int minIndex = 0;
            if (i < 10)
            {
                strcpy_s(gamers[i].nick, strlen(Gamer.nick) + 1, Gamer.nick);
                gamers[i].timeGame = Gamer.timeGame;
            }
            else 
            {
                min = gamers[0].timeGame;
                for (int j = 0; j < i; j++)
                {
                    if (gamers[j].timeGame < min)
                    {
                        min = gamers[j].timeGame;
                        minIndex = j;
                    }
                }
                strcpy_s(gamers[minIndex].nick, strlen(Gamer.nick) + 1, Gamer.nick);
                gamers[minIndex].timeGame = Gamer.timeGame;
            } 
            for (int j = 0; j < i+1; j++) //запись в файл
            {
                f.write((char*)&gamers[j], sizeof(gamers[j]));
            }
            f.close();
            done = SDL_TRUE;
        }
    }
    main(argc, argv); //рекурсия
}
