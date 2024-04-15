#pragma once
#include <vector>

#include "commonFunc.h"
#include "baseObject.h"
#include "projectile.h"
#include "attackAction.h"

#define player_speed 8
#define maxium_charging 30

class mainProtagonist : public baseObject{
    public:
        mainProtagonist();
        ~mainProtagonist();

        enum actionType{
            walk_right =0,
            walk_left =1,
            hurt_right =-2,
            hurt_left =-1,
        };
        bool is_charging_bow;

        float get_x_pos() const {return x_pos;}
        float get_y_pos() const {return y_pos;}
        int get_come_back_time() const {return come_back_time;}
        void set_come_back_time(int cbt) {come_back_time = cbt;}
        bool loadIMG(std::string path, SDL_Renderer* screen);
        void show(SDL_Renderer* des);
        void handleInputAction(SDL_Event events, SDL_Renderer* screen, int& xm, int& ym, map &map_data, int& gameState);
        void set_clip();

        void doPlayer(map& map_data);
        void checkCollision(map& map_data);
        void setMapXY(const int map_xx, const int map_yy){map_x = map_xx; map_y = map_yy;}
        void centerEntityOnMap(map& map_data);
        SDL_Rect getRectFrame();

        void set_bullet_list(std::vector<projectile*> p_bullet_list){
            bullet_list = p_bullet_list;
        }
        std::vector<projectile*> get_bullet_list() const {return bullet_list;}
        void handleBullet(SDL_Renderer* des, int xm, int ym);
        void removeBullet(const int& idx);
        void handleAttacksAction(SDL_Renderer* screen, attackAction& p_bow, int& xm, int& ym);
        void initHealth(SDL_Renderer* screen);
        void healthManagement(SDL_Renderer* screen);

        int frame_delay = 0;
        void frame_delayer();
        int latest;
        int getKey(){return key;}
        int get_health(){return health;}
        void got_hit(SDL_Renderer* screen);
    private:
        float x_val;
        float y_val;

        float x_pos;
        float y_pos;

        int width_frame;
        int height_frame;

        SDL_Rect frame_clip[8];
        input input_type;
        int frame;
        int status;

        int map_x;
        int map_y;

        std::vector<projectile*> bullet_list;
        std::vector<baseObject*> bulletBar;

        int x_screen_pos;
        int y_screen_pos;
        int key;
        int health;
        int bow_charge;
        int ammo;
        int come_back_time;

        std::vector<baseObject*> healthBar;
};