#include "headers/commonFunc.h"
#include "headers/textObject.h"
#include "headers/impTimer.h"
#include <fstream>



bool SDLCommonFunc::checkTileCollision(int tile){
    if(tile == object_tile3 || tile == object_tile4 || tile == object_tile5
    || tile == object_tile7 || tile == object_tile8 || tile == object_tile9
    || tile == object_tile10){
        return true;
    }
    return false;
}

bool SDLCommonFunc::checkCollision(const SDL_Rect& object1, const SDL_Rect& object2){
    int left_a = object1.x;
    int right_a = object1.x + object1.w;
    int top_a = object1.y;
    int bottom_a = object1.y + object1.h;

    int left_b = object2.x;
    int right_b = object2.x + object2.w;
    int top_b = object2.y;
    int bottom_b = object2.y + object2.h;

    //case 1:size object 1 < size object 2
    if(left_a > left_b && left_a < right_b){
        if(top_a > top_b && top_a < bottom_b){
            return true;
        }
    }

    if(left_a > left_b && left_a < right_b){
        if(bottom_a > top_b && bottom_a < bottom_b){
            return true;
        }
    }
    
    if(right_a > left_b && right_a < right_b){
        if(top_a > top_b && top_a < bottom_b){
            return true;
        }
    }

    if(right_a > left_b && right_a < right_b){
        if(bottom_a > top_b && bottom_a < bottom_b){
            return true;
        }
    }

    // case 2: size object 1 < size object 2
    if(left_b > left_a && left_b < right_a){
        if(top_b > top_a && top_b < bottom_a){
            return true;
        }
    }

    if(left_b > left_a && left_b < right_a){
        if(bottom_b > top_a && bottom_b < bottom_a){
            return true;
        }
    }

    if(right_b > left_a && right_b < right_a){
        if(top_b > top_a && top_b < bottom_a){
            return true;
        }
    }

    if(right_b > left_a && right_b < right_a){
        if(bottom_b > top_a && bottom_b < bottom_a){
            return true;
        }
    }

    // case 3: size object 1 = size object 2
    if(top_a == top_b && right_a == right_b && bottom_a == bottom_b){
        return true;
    }

    return false;
}

bool SDLCommonFunc::checkFocusWithRect(const int& xx, const int& yy, const SDL_Rect& rect){
    if(xx >= rect.x && xx <= rect.x + rect.w && yy >= rect.y && yy <= rect.y + rect.h){
        return true;
    }
    return false;
}

float SDLCommonFunc::calAngle(const int &xm,const int &ym,const int &x_screen_pos,const int &y_screen_pos){
    int xLength = xm - x_screen_pos;
    int yLength = ym - y_screen_pos;
    float angle = 0;
    if(xLength == 0){
        angle = yLength/abs(yLength) * 90;
    } else if(yLength == 0){
        angle = (xLength > 0? 0 : 180);
    } else {
        float fraction = abs(1.00 * yLength)/abs(1.00 * xLength);
        float goc = atan(fraction) * (1.00 * 180/M_PI);
        if (xLength > 0 && yLength > 0){//goc phan tu thu tu
            angle = goc;
        } else if (xLength < 0 && yLength > 0){//goc phan tu thu ba
            angle = 180 - goc;
        } else if (xLength < 0 && yLength < 0){//goc phan tu thu hai
            angle = 180 + goc;
        } else if (xLength > 0 && yLength < 0){//goc phan tu thu nhat
            angle = 360 - goc;
        }
    }
    return angle;
}

int SDLCommonFunc::showMenu(SDL_Renderer* des, TTF_Font* font, TTF_Font* hfont){
    g_sound_effects[5] = Mix_LoadWAV("sounds//sound_effects//hover.wav");
    g_sound_effects[6] = Mix_LoadWAV("sounds//sound_effects//confirm.wav");
    Mix_Volume(-1, 32);
    baseObject g_img_menu;
    g_img_menu.loadIMG("img//MenuImg//Main_menu.png", des);
    bool tRet = g_img_menu.loadIMG("img//MenuImg//Main_menu.png", des);
    if(!tRet) {
        g_img_menu.free();
        return 2;
    }

    // baseObject g_img_menu;
    // // g_img_menu.loadIMG("img//Sprite.png", des);
    // // bool tRet = g_img_menu.loadIMG("img//Sprite.png", des);
    // // if(!tRet) {
    // //     g_img_menu.free();
    // //     return 1;
    // // }

    const int kMenuItemNum = 3;
    SDL_Rect pos_arr[kMenuItemNum];
    // start
    pos_arr[0].x = 90;
    pos_arr[0].y = 480;
    pos_arr[0].w = 290;
    pos_arr[0].h = 110;//can be change but screw it
    // help
    pos_arr[1].x = 400;
    pos_arr[1].y = 610;
    pos_arr[1].w = 280;
    pos_arr[1].h = 140;
    // exit
    pos_arr[2].x = 90;
    pos_arr[2].y = 610;
    pos_arr[2].w = 227;
    pos_arr[2].h = 130;
    
    textObject text_menu[kMenuItemNum];

    text_menu[0].setText("Start");
    text_menu[0].setColorType(textObject::BLACK);
    text_menu[0].setRect(pos_arr[0].x, pos_arr[0].y);
    text_menu[0].setRectWH(pos_arr[0].w, pos_arr[0].h);

    text_menu[1].setText("Help");
    text_menu[1].setColorType(textObject::BLACK);
    text_menu[1].setRect(pos_arr[1].x, pos_arr[1].y);
    text_menu[1].setRectWH(pos_arr[1].w, pos_arr[1].h);

    text_menu[2].setText("Exit");
    text_menu[2].setColorType(textObject::BLACK);
    text_menu[2].setRect(pos_arr[2].x, pos_arr[2].y);
    text_menu[2].setRectWH(pos_arr[2].w, pos_arr[2].h);

    bool selected[kMenuItemNum] = {0, 0, 0};
    int xm=0;
    int ym=0;
    SDL_Event m_event;
    while (true){
        impTimer fps_timer;
        fps_timer.start();
        SDL_RenderClear(des);
        SDL_RenderCopy(des, g_img_menu.getObject(), NULL, NULL);
        for(int i=0; i<kMenuItemNum; i++){
            text_menu[i].loadFromRenderText(font, des);
            text_menu[i].renderText(des, text_menu[i].getRect().x, text_menu[i].getRect().y);
            // problem is here, cant set w and h through rendertext
        }
        while (SDL_PollEvent(&m_event)){
            switch(m_event.type){
                case SDL_QUIT:
                    g_img_menu.free();
                    text_menu->free();
                    return 2;
                case SDL_MOUSEMOTION:
                    {
                        xm = m_event.motion.x;
                        ym = m_event.motion.y;
                        
                        //hovering
                        for(int i=0; i<kMenuItemNum; i++){
                            if(checkFocusWithRect(xm, ym, text_menu[i].getRect())){
                                if(selected[i] == false){
                                    Mix_PlayChannel(6, g_sound_effects[5], 0);
                                    // int bait = Mix_PlayChannel(6, g_sound_effects[5], 0);
                                    selected[i] = 1;
                                    text_menu[i].setColorType(textObject::RED);
                                }
                            }
                            else {
                                if(selected[i] == true){
                                    selected[i] = 0;
                                    text_menu[i].setColorType(textObject::BLACK);
                                }
                            }
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    {
                        xm = m_event.button.x;
                        ym = m_event.button.y;
                        //clicking
                        for(int i=0; i<kMenuItemNum; i++){
                            if(checkFocusWithRect(xm, ym, text_menu[i].getRect())){
                                Mix_PlayChannel(7, g_sound_effects[6], 0);
                                if(i==1){
                                    SDLCommonFunc::showHelp(des, hfont);
                                    continue;
                                } else {
                                    g_img_menu.free();
                                    text_menu->free();
                                    return i;
                                }
                            }
                        }
                    }
                    break;
                case SDL_KEYDOWN:
                    {
                        if(m_event.key.keysym.sym == SDLK_ESCAPE){
                            g_img_menu.free();
                            text_menu->free();
                            return 2;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        SDL_RenderPresent(des);

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000/FPS; //don vi ms
        if(real_imp_time<time_one_frame){
            int delay_time = time_one_frame - real_imp_time;
            if(delay_time >=0){
                SDL_Delay(delay_time);
            }
        }
    }
    g_img_menu.free();
    text_menu->free();
    
    return 2;
}

int SDLCommonFunc::handleMenu(SDL_Event events, SDL_Rect pauseIcon){
    // while (SDL_PollEvent(&events)){
        switch(events.type){
            case SDL_MOUSEBUTTONDOWN:
                {
                    int xm = events.button.x;
                    int ym = events.button.y;
                    //clicking
                    if(checkFocusWithRect(xm, ym, pauseIcon)){
                        return 1;
                    }
                }
                break;
            default:
                break;
        }
    //}
    return 0;
}

int SDLCommonFunc::showPause(SDL_Renderer* des, TTF_Font* font, TTF_Font* hfont){
    g_sound_effects[4] = Mix_LoadWAV("sounds//sound_effects//pause.wav");
    g_sound_effects[5] = Mix_LoadWAV("sounds//sound_effects//hover.wav");
    g_sound_effects[6] = Mix_LoadWAV("sounds//sound_effects//confirm.wav");
    Mix_Volume(-1, 32);
    Mix_PlayChannel(5, g_sound_effects[4], 0);
    baseObject pauseMenu;
    pauseMenu.loadIMG("img//MenuImg//pauseMenu.png", des);
    bool bRet = pauseMenu.loadIMG("img//MenuImg//pauseMenu.png", des);
    if(!bRet) {
        pauseMenu.free();
        return 0;
    }

    pauseMenu.setRect(384, 192);
    pauseMenu.setRectWH(512, 384);

    const int kMenuItemNum = 3;
    SDL_Rect pos_arr[kMenuItemNum];
    // continue
    pos_arr[0].x = 384+85+64+16;
    pos_arr[0].y = 192+48;
    pos_arr[0].w = 260;
    pos_arr[0].h = 83;//can be change but screw it
    // help
    pos_arr[1].x = 384+85+64+16;
    pos_arr[1].y = 192+48+83+27+4;
    pos_arr[1].w = 260;
    pos_arr[1].h = 83;
    // return home
    pos_arr[2].x = 384+85+64+16;
    pos_arr[2].y = 192+48+83+27+83+27+8;
    pos_arr[2].w = 260;
    pos_arr[2].h = 83;

    
    textObject text_menu[kMenuItemNum];

    text_menu[0].setText("Continue");
    text_menu[0].setColorType(textObject::BLACK);
    text_menu[0].setRect(pos_arr[0].x, pos_arr[0].y);
    text_menu[0].setRectWH(pos_arr[0].w, pos_arr[0].h);

    text_menu[1].setText("Help");
    text_menu[1].setColorType(textObject::BLACK);
    text_menu[1].setRect(pos_arr[1].x, pos_arr[1].y);
    text_menu[1].setRectWH(pos_arr[1].w, pos_arr[1].h);

    text_menu[2].setText("Main menu");
    text_menu[2].setColorType(textObject::BLACK);
    text_menu[2].setRect(pos_arr[2].x, pos_arr[2].y);
    text_menu[2].setRectWH(pos_arr[2].w, pos_arr[2].h);

    bool selected[kMenuItemNum] = {0, 0, 0};
    int xm=0;
    int ym=0;
    SDL_Event m_event;
    while (true){
        impTimer fps_timer;
        fps_timer.start();
        // SDL_RenderClear(des);
        pauseMenu.render(des, NULL);
        for(int i=0; i<kMenuItemNum; i++){
            text_menu[i].loadFromRenderText(font, des);
            text_menu[i].renderText(des, text_menu[i].getRect().x, text_menu[i].getRect().y);
            // problem is here, cant set w and h through rendertext
        }
        while (SDL_PollEvent(&m_event)){
            switch(m_event.type){
                case SDL_QUIT:
                    pauseMenu.free();
                    text_menu->free();
                    return 0;
                case SDL_MOUSEMOTION:
                    {
                        xm = m_event.motion.x;
                        ym = m_event.motion.y;
                        
                        //hovering
                        for(int i=0; i<kMenuItemNum; i++){
                            if(checkFocusWithRect(xm, ym, text_menu[i].getRect())){
                                if(selected[i] == false){
                                    Mix_PlayChannel(6, g_sound_effects[5], 0);
                                    selected[i] = 1;
                                    text_menu[i].setColorType(textObject::RED);
                                }
                            }
                            else {
                                if(selected[i] == true){
                                    selected[i] = 0;
                                    text_menu[i].setColorType(textObject::BLACK);
                                }
                            }
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    {
                        xm = m_event.button.x;
                        ym = m_event.button.y;
                        //clicking
                        for(int i=0; i<kMenuItemNum; i++){
                            if(checkFocusWithRect(xm, ym, text_menu[i].getRect())){
                                Mix_PlayChannel(7, g_sound_effects[6], 0);
                                pauseMenu.free();
                                text_menu->free();
                                if(i==1){
                                    return SDLCommonFunc::showHelp(des, hfont);
                                }
                                return i;
                            }
                        }
                    }
                    break;
                case SDL_KEYDOWN:
                    {
                        if(m_event.key.keysym.sym == SDLK_ESCAPE){
                            pauseMenu.free();
                            text_menu->free();
                            return 0;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        SDL_RenderPresent(des);

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000/FPS; //don vi ms
        if(real_imp_time<time_one_frame){
            int delay_time = time_one_frame - real_imp_time;
            if(delay_time >=0){
                SDL_Delay(delay_time);
            }
        }
    }
    pauseMenu.free();
    text_menu->free();
    
    return 0;
}

int SDLCommonFunc::showHelp(SDL_Renderer* des, TTF_Font* font){
    g_sound_effects[5] = Mix_LoadWAV("sounds//sound_effects//hover.wav");
    g_sound_effects[6] = Mix_LoadWAV("sounds//sound_effects//confirm.wav");
    Mix_Volume(-1, 32);
    baseObject helpMenu;
    helpMenu.loadIMG("img//MenuImg//helpMenu.png", des);
    bool bRet = helpMenu.loadIMG("img//MenuImg//helpMenu.png", des);
    if(!bRet) {
        helpMenu.free();
        return 1;
    }

    helpMenu.setRect(384, 192);
    helpMenu.setRectWH(512, 384);

    const int kMenuItemNum = 1;
    SDL_Rect pos_arr[kMenuItemNum];
    // back
    pos_arr[0].x = 390;
    pos_arr[0].y = 532;
    pos_arr[0].w = 100;
    pos_arr[0].h = 60;//can be change but screw it

    
    textObject text_menu[kMenuItemNum];

    text_menu[0].setText("Back");
    text_menu[0].setColorType(textObject::BLACK);
    text_menu[0].setRect(pos_arr[0].x, pos_arr[0].y);
    text_menu[0].setRectWH(pos_arr[0].w, pos_arr[0].h);

    bool selected[kMenuItemNum] = {0};
    int xm=0;
    int ym=0;
    SDL_Event m_event;
    while (true){
        impTimer fps_timer;
        fps_timer.start();
        // SDL_RenderClear(des);
        helpMenu.render(des, NULL);
        for(int i=0; i<kMenuItemNum; i++){
            text_menu[i].loadFromRenderText(font, des);
            text_menu[i].renderText(des, text_menu[i].getRect().x, text_menu[i].getRect().y);
            // problem is here, cant set w and h through rendertext
        }
        while (SDL_PollEvent(&m_event)){
            switch(m_event.type){
                case SDL_QUIT:
                    helpMenu.free();
                    text_menu->free();
                    return 1;
                case SDL_MOUSEMOTION:
                    {
                        xm = m_event.motion.x;
                        ym = m_event.motion.y;
                        
                        //hovering
                        for(int i=0; i<kMenuItemNum; i++){
                            if(checkFocusWithRect(xm, ym, text_menu[i].getRect())){
                                if(selected[i] == false){
                                    Mix_PlayChannel(6, g_sound_effects[5], 0);
                                    selected[i] = 1;
                                    text_menu[i].setColorType(textObject::RED);
                                }
                            }
                            else {
                                if(selected[i] == true){
                                    selected[i] = 0;
                                    text_menu[i].setColorType(textObject::BLACK);
                                }
                            }
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    {
                        xm = m_event.button.x;
                        ym = m_event.button.y;
                        //clicking
                        for(int i=0; i<kMenuItemNum; i++){
                            if(checkFocusWithRect(xm, ym, text_menu[i].getRect())){
                                Mix_PlayChannel(7, g_sound_effects[6], 0);
                                helpMenu.free();
                                text_menu->free();
                                return 1;
                            }
                        }
                    }
                    break;
                case SDL_KEYDOWN:
                    {
                        if(m_event.key.keysym.sym == SDLK_ESCAPE){
                            helpMenu.free();
                            text_menu->free();
                            return 1;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        SDL_RenderPresent(des);

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000/FPS; //don vi ms
        if(real_imp_time<time_one_frame){
            int delay_time = time_one_frame - real_imp_time;
            if(delay_time >=0){
                SDL_Delay(delay_time);
            }
        }
    }
    helpMenu.free();
    text_menu->free();
    
    return 1;
}

int SDLCommonFunc::showVictory(SDL_Renderer* des, TTF_Font* font, std::string name, UINT mark_value){
    g_sound_effects[5] = Mix_LoadWAV("sounds//sound_effects//hover.wav");
    g_sound_effects[6] = Mix_LoadWAV("sounds//sound_effects//confirm.wav");
    Mix_Volume(-1, 32);
    std::fstream rfile("highScore.txt");
    if (!rfile.is_open())
    {
        std:: cout << "Couldn't open file: " << name << ", specificly: " << SDL_GetError() << std::endl;
        return 0;
    }
    int hScore;
    rfile >> hScore;
    if(mark_value > hScore){
        std::fstream wfile("highScore.txt");
        wfile << mark_value;
        hScore = mark_value;
        wfile.close();
    }
    rfile.close();
    
    baseObject victoryMenu;
    victoryMenu.loadIMG("img//MenuImg//new_"+name+"_screen.png", des);
    bool bRet = victoryMenu.loadIMG("img//MenuImg//new_"+name+"_screen.png", des);
    if(!bRet) {
        victoryMenu.free();
        return 0;
    }

    // victoryMenu.setRect(320, 128);
    // victoryMenu.setRectWH(640, 512);
    victoryMenu.setRect(0, 0);
    victoryMenu.setRectWH(1280, 768);

    const int kMenuItemNum = 2;
    SDL_Rect pos_arr[kMenuItemNum];
    // play again
    pos_arr[0].x = 180;
    pos_arr[0].y = 595;
    pos_arr[0].w = 410;
    pos_arr[0].h = 90;//can be change but screw it
    // return home
    pos_arr[1].x = 815;
    pos_arr[1].y = 595;
    pos_arr[1].w = 410;
    pos_arr[1].h = 90;

    
    textObject text_menu[kMenuItemNum];

    text_menu[0].setText("Play again");
    text_menu[0].setColorType(textObject::BLACK);
    text_menu[0].setRect(pos_arr[0].x, pos_arr[0].y);
    text_menu[0].setRectWH(pos_arr[0].w, pos_arr[0].h);

    text_menu[1].setText("Main menu");
    text_menu[1].setColorType(textObject::BLACK);
    text_menu[1].setRect(pos_arr[1].x, pos_arr[1].y);
    text_menu[1].setRectWH(pos_arr[1].w, pos_arr[1].h);

    bool selected[kMenuItemNum] = {0, 0};
    int xm=0;
    int ym=0;
    SDL_Event m_event;
    while (true){
        impTimer fps_timer;
        fps_timer.start();
        SDL_RenderClear(des);
        victoryMenu.render(des, NULL);

        std::string val_str_mark = std::to_string(mark_value);
        std::string strMark("Total score: ");
        strMark += val_str_mark;
        textObject mark_game;
        mark_game.setColorType(textObject::BLACK);
        mark_game.setText(strMark);
        mark_game.loadFromRenderText(font, des);
        mark_game.renderText(des, 50, 500);

        std::string val_highScore = std::to_string(hScore);
        std::string highScore("High Score: ");
        highScore += val_highScore;
        textObject record;
        record.setColorType(textObject::BLACK);
        record.setText(highScore);
        record.loadFromRenderText(font, des);
        record.renderText(des, 680, 500);

        for(int i=0; i<kMenuItemNum; i++){
            text_menu[i].loadFromRenderText(font, des);
            text_menu[i].renderText(des, text_menu[i].getRect().x, text_menu[i].getRect().y);
            // problem is here, cant set w and h through rendertext
        }
        while (SDL_PollEvent(&m_event)){
            switch(m_event.type){
                case SDL_QUIT:
                    victoryMenu.free();
                    text_menu->free();
                    return 0;
                case SDL_MOUSEMOTION:
                    {
                        xm = m_event.motion.x;
                        ym = m_event.motion.y;
                        
                        //hovering
                        for(int i=0; i<kMenuItemNum; i++){
                            if(checkFocusWithRect(xm, ym, text_menu[i].getRect())){
                                if(selected[i] == false){
                                    Mix_PlayChannel(6, g_sound_effects[5], 0);
                                    selected[i] = 1;
                                    text_menu[i].setColorType(textObject::RED);
                                }
                            }
                            else {
                                if(selected[i] == true){
                                    selected[i] = 0;
                                    text_menu[i].setColorType(textObject::BLACK);
                                }
                            }
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    {
                        xm = m_event.button.x;
                        ym = m_event.button.y;
                        //clicking
                        for(int i=0; i<kMenuItemNum; i++){
                            if(checkFocusWithRect(xm, ym, text_menu[i].getRect())){
                                Mix_PlayChannel(7, g_sound_effects[6], 0);
                                if(i==0){
                                    return 1;
                                } else if(i==1){
                                    return 0;
                                }
                            }
                        }
                    }
                    break;
                case SDL_KEYDOWN:
                    {
                        if(m_event.key.keysym.sym == SDLK_ESCAPE){
                            victoryMenu.free();
                            text_menu->free();
                            return 0;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        SDL_RenderPresent(des);

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000/FPS; //don vi ms
        if(real_imp_time<time_one_frame){
            int delay_time = time_one_frame - real_imp_time;
            if(delay_time >=0){
                SDL_Delay(delay_time);
            }
        }
    }
    victoryMenu.free();
    text_menu->free();
    
    return 0;
}
