#include <bits/stdc++.h>
#include <ctime>
#include <cstdlib>

#include "headers/commonFunc.h"
#include "headers/baseObject.h"
#include "headers/gameMap.h"
#include "headers/mainProtagonist.h"
#include "headers/impTimer.h"
#include "headers/threatsObject.h"
#include "headers/deathAnimation.h"
#include "headers/textObject.h"
#include "headers/geometry.h"
#include "headers/attackAction.h"

baseObject g_background;
TTF_Font* game_font = NULL;
TTF_Font* menu_font = NULL;
TTF_Font* pause_font = NULL;
TTF_Font* help_font = NULL;
TTF_Font* victory_font = NULL;

int getRandom(int min,int max){
    return min + (int)(rand()*(max-min)/(1.0+RAND_MAX));
}

bool initData(){
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if(ret<0){
        return false;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("CRISPYLEMON", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                screen_width, screen_height, SDL_WINDOW_SHOWN);//SDL_WINDOW_RESIZABLE
    if(g_window == NULL){
        success = false;
    } else {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if(g_screen == NULL){
            success = false;
        } else {
            SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) && imgFlags)){
                success = false;
            }
        }
        if(TTF_Init() == -1){
            success = false;
        }
        game_font = TTF_OpenFont("font//ThaleahFat.ttf", 40);
        if(game_font == NULL){
            success = false;
        }
        menu_font = TTF_OpenFont("font//DungeonFont.ttf", 150);
        if(menu_font == NULL){
            success = false;
        }
        pause_font = TTF_OpenFont("font//DungeonFont.ttf", 60);
        if(pause_font == NULL){
            success = false;
        }
        help_font = TTF_OpenFont("font//DungeonFont.ttf", 40);
        if(help_font == NULL){
            success = false;
        }
        victory_font = TTF_OpenFont("font//DungeonFont.ttf", 90);
        if(victory_font == NULL){
            success = false;
        }

        if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1){
            success = false;
        } else {
            g_background_music[0] = Mix_LoadMUS("sounds//background_music//main_menu_music.wav");
            g_background_music[1] = Mix_LoadMUS("sounds//background_music//in_game_music.wav");
            g_background_music[2] = Mix_LoadMUS("sounds//background_music//victory.wav");
            g_background_music[3] = Mix_LoadMUS("sounds//background_music//game_lose.wav");
            g_sound_effects[0] = Mix_LoadWAV("sounds//sound_effects//bow_shoot.wav");
            g_sound_effects[1] = Mix_LoadWAV("sounds//sound_effects//flesh_hit.wav");
            g_sound_effects[2] = Mix_LoadWAV("sounds//sound_effects//Roblox-death-sound.wav");
            g_sound_effects[3] = Mix_LoadWAV("sounds//sound_effects//step_grass.wav");
            g_sound_effects[4] = Mix_LoadWAV("sounds//sound_effects//pause.wav");
            g_sound_effects[5] = Mix_LoadWAV("sounds//sound_effects//hover.wav");
            g_sound_effects[6] = Mix_LoadWAV("sounds//sound_effects//confirm.wav");
            g_sound_effects[7] = Mix_LoadWAV("sounds//sound_effects//p_impact_flesh.wav");
            for(int sound = 0; sound<4/*size of bgmus*/; sound++){
                if(g_background_music[sound] == NULL){
                    std::cout << "fail to load music: " << Mix_GetError() << std::endl;
                    success = false;
                }
            }
            for(int sound = 0; sound<8/*size of sfx*/; sound++){
                if(g_sound_effects[sound] == NULL){
                    std::cout << "fail to load sound: " << Mix_GetError() << std::endl;
                    success = false;
                }
            }
        }
    }
    return success;
}

// bool loadBackground(){
//     bool ret = g_background.loadIMG("img//level1_background_quadter.png", g_screen);
//     if(ret == false){
//         return false;
//     }
//     return true;
// }

void close(){
    g_background.free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    for(int i=0; i<4; i++){
        Mix_FreeMusic(g_background_music[i]);
        g_background_music[i] = NULL;
    }
    for(int i=0; i<8; i++){
        Mix_FreeChunk(g_sound_effects[i]);
        g_sound_effects[i] = NULL;
    }

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

std::vector<threatsObject*> makeThreatList(){
    std::vector<threatsObject*> list_threats;

    threatsObject* dynamic_threats = new threatsObject[24];
    std::ifstream enemyFile("enemyPos.txt");
    if(!enemyFile.is_open()){
        std::cout << "Couldn't open enemyFile: " << SDL_GetError() << std::endl;
        return list_threats; 
    }

    for(int i=0; i<24; i++){
        threatsObject* p_threat = (dynamic_threats + i);
        if (p_threat != NULL){
            if(i%2==0){
                p_threat->loadIMG("img//ToxicFrog//ToxicFrog_Hop_left.png", g_screen);
                p_threat->set_enemy_type(threatsObject::FROG);
                p_threat->set_health(40);
            } else {
                p_threat->loadIMG("img//Pumpkin//Pumpkin_Jump_left.png", g_screen);
                p_threat->set_enemy_type(threatsObject::PUMPKIN);
                p_threat->set_health(70);
            }
            p_threat->set_clip();
            
            std::string line;
            getline(enemyFile, line);
            while(line == ""){
                getline(enemyFile, line);
            }
            std::stringstream ss(line);
            std::string token;
            int cnt=0;
            int xx= 0, yy=0;
            while(ss>>token){
                if(cnt == 0){
                    xx = stoi(token);
                    cnt++;
                } else if(cnt == 1) {
                    yy = stoi(token);
                    cnt++;
                }
            }
            p_threat->set_x_pos(xx*tile_size);
            p_threat->set_y_pos(yy*tile_size);
            p_threat->set_spawn_x_pos(xx*tile_size);
            p_threat->set_spawn_y_pos(yy*tile_size);

            list_threats.push_back(p_threat);
        }
    }
    enemyFile.close();

    return list_threats;
}

enum gameState{
    quitGame = -1,
    homeMenu = 0,
    inGame = 1,
    victoryScreen = 2,
    loseScreen = 3,
};

int main(int argc, char* argv[]){
    impTimer fps_timer;
    srand((unsigned int)time(NULL));

    if(initData() == false){
        return -1;
    }
    Mix_Volume(-1, 32);
    // if(loadBackground() == false){
    //     return -1;
    // }
    int gameState = 0;
    int isRunning = 0;
    bool isGaming = false;
    UINT mark_value = 0;
    Uint32 time_val = 0;
    while(isRunning != -1){
        switch(gameState){
            case quitGame:
            {
                isRunning=-1;
            }
            case homeMenu:
            {
                Mix_VolumeMusic(32);
                Mix_PlayMusic(g_background_music[0], -1);
                int ret_menu = SDLCommonFunc::showMenu(g_screen, menu_font, help_font);
                if(ret_menu == 2){
                    isGaming = false;
                    gameState = quitGame;
                    isRunning = -1;
                } else {
                    gameState = inGame;
                    isGaming = true;
                }
            }
            break;
            case inGame:
            {
                Mix_VolumeMusic(16);
                Mix_PlayMusic(g_background_music[1], -1);
                gameMap game_map;
                game_map.loadMap("level2.txt");
                game_map.loadTiles(g_screen);

                mainProtagonist p_player;
                p_player.loadIMG("img//Main_player//Main_player_idle_right.png", g_screen);
                bool pRet = p_player.loadIMG("img//Main_player//Main_player_idle_right.png", g_screen);
                if(!pRet) std::cout << "fail to load main char: " << SDL_GetError() << std::endl;
                p_player.set_clip();
                p_player.initHealth(g_screen);

                attackAction the_bow;
                the_bow.loadIMG("img//Main_player//Main_player_charging_bow.png", g_screen);
                bool bRet = the_bow.loadIMG("img//Main_player//Main_player_charging_bow.png", g_screen);
                if(!bRet) std::cout << "fail to load bow: " << SDL_GetError() << std::endl;
                the_bow.set_clip();
                int xm, ym;

                std::vector<threatsObject*> threats_list = makeThreatList();

                deathAnimation death_threat;

                std::vector<deathAnimation*> explosion_list;

                textObject time_game;
                time_game.setColorType(textObject::WHITE);
                textObject mark_game;
                mark_game.setColorType(textObject::RED);
                textObject key;
                key.setColorType(textObject::RED);
                mark_value = 0;

                baseObject pauseIcon;
                pauseIcon.loadIMG("img//MenuImg//pauseIcon.png", g_screen);
                bool aRet = pauseIcon.loadIMG("img//MenuImg//pauseIcon.png", g_screen);
                if(!aRet) {
                    pauseIcon.free();
                    return -1;
                }

                pauseIcon.setRect(1184, 32);
                pauseIcon.setRectWH(64, 64);

                int isPaused = 0;
                
                Uint32 start_time = SDL_GetTicks()/1000;
                isGaming=true;
                while(isGaming){
                    map map_data = game_map.getMap();
                    while(SDL_PollEvent(&g_event)!=0){
                        if(g_event.type == SDL_QUIT){
                            isGaming = false;
                            gameState = quitGame;
                            isRunning = -1;
                        }
                        if(g_event.type == SDL_MOUSEMOTION){
                            SDL_GetMouseState(&xm, &ym);
                        }
                        isPaused = SDLCommonFunc::handleMenu(g_event, pauseIcon.getRect());
                        if(isPaused == 0){
                            p_player.handleInputAction(g_event, g_screen, xm, ym, map_data, gameState);
                        }
                    }
                    if(gameState == 2){
                        isGaming=false;
                    }
                    while(isPaused == 1){
                        isPaused = SDLCommonFunc::showPause(g_screen, pause_font, help_font);
                    }
                    if(isPaused == 2){
                        isGaming=false;
                        gameState=homeMenu;
                    }

                    fps_timer.start();
                    SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
                    SDL_RenderClear(g_screen);
                    //g_background.render(g_screen, NULL);

                    p_player.setMapXY(map_data.start_x, map_data.start_y);
                    p_player.doPlayer(map_data);

                    game_map.setMap(map_data);
                    game_map.drawMap(g_screen);

                    p_player.show(g_screen);
                    p_player.handleAttacksAction(g_screen, the_bow, xm, ym);
                    p_player.handleBullet(g_screen, xm, ym);

                    //draw geometry
                    // geometricFormat rectangle_size0(0, 0, 192, 80);
                    // colorData rect_color_data0(255, 166, 77);
                    // geometry::renderRectangle(rectangle_size0, rect_color_data0, g_screen);
                
                    // geometricFormat outlineSize0(6, 10, 180, 30);
                    // colorData stat_color_data0(128, 85, 0);
                    // geometry::renderOutline(outlineSize0, stat_color_data0, g_screen);
                    
                    for(int i=0; i<threats_list.size(); i++){
                        threatsObject* p_threat = threats_list.at(i);
                        if(p_threat != NULL){
                            p_threat->setMapXY(map_data.start_x, map_data.start_y);
                            p_threat->impIMGType(g_screen);

                            int dau_x = getRandom(1, 10);
                            dau_x = getRandom(1, 10);
                            int dau_y = getRandom(1, 10);
                            int desX = getRandom(0, 5);
                            int desY = getRandom(0, 5);
                            if(dau_x%2==1){
                                desX *= -1;
                            }
                            if(dau_y%2==1){
                                desY *= -1;
                            }
                            p_threat->idling(g_screen, desX, desY, p_player.get_x_pos(), p_player.get_y_pos());
                            p_threat->doThreat(map_data);
                            p_threat->makeBullet(g_screen, p_player.get_x_pos(), p_player.get_y_pos());
                            p_threat->show(g_screen);

                            // check collision between threats' bullet and player
                            if(p_player.get_come_back_time()>=100){
                                SDL_Rect rect_player = p_player.getRectFrame();
                                bool got_hit = false;
                                std::vector<projectile*> thr_bullet_list = p_threat->get_bullet_list();
                                for(int jj=0; jj<thr_bullet_list.size(); jj++){
                                    projectile* pt_bullet = thr_bullet_list.at(jj);
                                    if(pt_bullet){
                                        got_hit = SDLCommonFunc::checkCollision(pt_bullet->getRect(), rect_player);
                                        if(got_hit){
                                            p_threat->removeBullet(jj);
                                            break;
                                        }
                                    }
                                }

                                // check collision between player and threat themself
                                SDL_Rect rect_threat = p_threat->getRectFrame();
                                bool got_touch = SDLCommonFunc::checkCollision(rect_player, rect_threat);

                                if(got_touch || got_hit){
                                    if(p_player.get_health() > 0){
                                        p_player.got_hit(g_screen);
                                        p_player.set_come_back_time(0);
                                        // put hurt animation here
                                    }
                                    if(p_player.get_health() <= 0){
                                        // player death ani
                                        // for(int d=0; d<DEATH_FRAME_NUM; d++){
                                        //     int d_x_pos = p_bullet->getRect().x - frame_death_width*0.5;
                                        //     int d_y_pos = p_bullet->getRect().y - frame_death_height*0.5;
                                        //     death_threat.set_frame(d);
                                        //     death_threat.setRect(d_x_pos, d_y_pos);
                                        //     death_threat.show(g_screen);
                                        //     SDL_RenderPresent(g_screen);
                                        // }
                                        // main died
                                        gameState = loseScreen;
                                        isGaming = false;
                                    }
                                }
                            } else {
                                p_player.set_come_back_time(p_player.get_come_back_time()+1);
                            }
                        }
                    }

                    //check collision between player's bullet and threats
                    int frame_death_width = death_threat.get_frame_width();
                    int frame_death_height = death_threat.get_frame_height();

                    std::vector<projectile*> bullet_arr = p_player.get_bullet_list();
                    for(int b=0; b<bullet_arr.size(); b++){
                        projectile* p_bullet = bullet_arr.at(b);
                        if(p_bullet != NULL){
                            for(int t=0; t<threats_list.size(); t++){
                                threatsObject* obj_threat = threats_list.at(t);
                                if(obj_threat != NULL){
                                    SDL_Rect tRect;
                                    tRect.x = obj_threat->getRect().x;
                                    tRect.y = obj_threat->getRect().y;
                                    tRect.w = obj_threat->get_width_frame();
                                    tRect.h = obj_threat->get_height_frame();

                                    SDL_Rect bRect = p_bullet->getRect();

                                    bool hit_enemy = SDLCommonFunc::checkCollision(bRect, tRect);

                                    if(hit_enemy){
                                        if(obj_threat->get_health() > 0){
                                            p_player.removeBullet(b);
                                            obj_threat->hit_enemy();
                                        }
                                        if(obj_threat->get_health() <= 0){
                                            deathAnimation* newExplosion = new deathAnimation();
                                            mark_value+=100;
                                            //selecting death animation
                                            int enemy_type = obj_threat->get_enemy_type();
                                            int move_dir = obj_threat->get_move_dir();
                                            if(enemy_type == threatsObject::PUMPKIN){
                                                if(move_dir == threatsObject::move_right){
                                                    bool tRet = newExplosion->loadIMG("img//Pumpkin//Pumpkin_Death_Right.png", g_screen);
                                                    if(!tRet) return -1;
                                                } else if (move_dir == threatsObject::move_left){
                                                    bool tRet = newExplosion->loadIMG("img//Pumpkin//Pumpkin_Death_Left.png", g_screen);
                                                    if(!tRet) return -1;
                                                }
                                            } else if(enemy_type == threatsObject::FROG){
                                                if(move_dir == threatsObject::move_right){
                                                    bool tRet = newExplosion->loadIMG("img//ToxicFrog//ToxicFrog_Death_Right.png", g_screen);
                                                    if(!tRet) return -1;
                                                } else if (move_dir == threatsObject::move_left){
                                                    bool tRet = newExplosion->loadIMG("img//ToxicFrog//ToxicFrog_Death_Left.png", g_screen);
                                                    if(!tRet) return -1;
                                                }
                                            }
                                            newExplosion->set_clip();

                                            int tx_pos = obj_threat->get_x_pos();
                                            int ty_pos = obj_threat->get_y_pos();
                                            if(enemy_type == 0){
                                                newExplosion->set_x_pos(tx_pos-32);
                                                newExplosion->set_y_pos(ty_pos-8);
                                            } else {
                                                newExplosion->set_x_pos(tx_pos);
                                                newExplosion->set_y_pos(ty_pos+16);
                                            }
                                            explosion_list.push_back(newExplosion);
                                            // sound
                                            Mix_PlayChannel(3, g_sound_effects[2], 0);
                                            // free the bullet and enemy
                                            p_player.removeBullet(b);
                                            obj_threat->free();
                                            threats_list.erase(threats_list.begin() + t);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    // load death frame
                    for(int ex = 0;ex < explosion_list.size(); ex++)
                    {
                        deathAnimation* newExplosion = explosion_list.at(ex);
                        if(newExplosion != NULL)
                        {
                            newExplosion->setMapXY(map_data.start_x,map_data.start_y);
                            newExplosion->show(g_screen);
                            if(newExplosion->get_animation_loop() > 0)
                            {
                                newExplosion->free();
                                explosion_list.erase(explosion_list.begin()+ex);
                            }
                        }
                    }
                    

                    //show game time
                    std::string time = "Time: ";
                    time_val = SDL_GetTicks()/1000 - start_time;
                    std::string val = std::to_string(time_val);
                    time += val;

                    time_game.setText(time);
                    time_game.loadFromRenderText(game_font, g_screen);
                    time_game.renderText(g_screen, screen_width - 300, 15);

                    std::string val_str_mark = std::to_string(mark_value);
                    std::string strMark("Score: ");
                    strMark += val_str_mark;

                    mark_game.setText(strMark);
                    mark_game.loadFromRenderText(game_font, g_screen);
                    mark_game.renderText(g_screen, screen_width*0.5, 15);

                    pauseIcon.render(g_screen, NULL);
                    p_player.healthManagement(g_screen);

                    std::string val_key = std::to_string(p_player.getKey());
                    std::string curKey("Keys: ");
                    curKey += val_key;

                    key.setText(curKey);
                    key.loadFromRenderText(game_font, g_screen);
                    key.renderText(g_screen, 110, 15);

                    SDL_RenderPresent(g_screen);

                    int real_imp_time = fps_timer.get_ticks();
                    int time_one_frame = 1000/FPS; //don vi ms
                    if(real_imp_time<time_one_frame){
                        int delay_time = time_one_frame - real_imp_time;
                        if(delay_time >=0){
                            SDL_Delay(delay_time);
                        }
                    }
                }//ngoac cua isGaming
                for(int i=0; i< threats_list.size(); i++){
                    threatsObject* p_threat = threats_list.at(i);
                    if(p_threat){
                        p_threat->free();
                        p_threat = NULL;
                    }
                }
                threats_list.clear();
            }//ngoac cua case
            break;
            case victoryScreen:
            {
                Mix_VolumeMusic(32);
                Mix_PlayMusic(g_background_music[2], 0);
                gameState = SDLCommonFunc::showVictory(g_screen, victory_font, "victory", mark_value+(std::max((300-int(time_val))*20, 0)));
            }
            break;
            case loseScreen:
            {
                Mix_VolumeMusic(32);
                Mix_PlayMusic(g_background_music[3], 0);
                gameState = SDLCommonFunc::showVictory(g_screen, victory_font, "lose", mark_value);
            }
            break;
            default:
                break;
        }//ngoac cua switch

    }//ngoac cua isRuning
    

    close();
    return 0;
}
//To do list:
// add frog attack animation
// if possible add save progress
// if possible add sound modification
// if possible add pumpkin attack

//New stuff:
// mini map
// big boss
// key puzzle image effect

// add theme music // done
// add comebacktime // done
// add health bar and enemy health // done
// add pause menu // done
// add victory screen // done
// save score // done
// return to menu when loss or win // done