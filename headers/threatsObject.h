#pragma once
#include <vector>

#include "baseObject.h"
#include "commonFunc.h"
#include "projectile.h"


#define THREAT_FRAME_NUM 8
#define THREAT_SPEED 2
#define BULLET_SPEED 10

class threatsObject : public baseObject{
    public:
        threatsObject();
        ~threatsObject();

        enum enemyType{
            PUMPKIN = 0,
            FROG = 1,
        };

        enum moveDir{
            move_left = 1,
            move_right = 0,
        };

        void set_x_val(const float& xVal) {x_val = xVal;}
        void set_y_val(const float& yVal) {y_val = yVal;}
        void set_x_pos(const float& xp) { x_pos = xp;}
        void set_y_pos(const float& yp) { y_pos = yp;}
        float get_x_pos() const {return x_pos;}
        float get_y_pos() const {return y_pos;}
        void set_x_des(const float& xDes) {x_des = xDes;}
        void set_y_des(const float& yDes) {y_des = yDes;}
        void set_spawn_x_pos(const float& s_x_pos) {spawn_x_pos = s_x_pos;}
        void set_spawn_y_pos(const float& s_y_pos) {spawn_y_pos = s_y_pos;}
        void set_health(const int& he) { health = he;}
        int get_health() {return health;}
        void hit_enemy();

        void setMapXY(const int& mp_x, const int& mp_y) {map_x = mp_x; map_y = mp_y;}
        void set_clip();
        bool loadIMG(std::string path, SDL_Renderer* screen);
        void show(SDL_Renderer* des);
        int get_width_frame() const {return width_frame;}
        int get_height_frame() const {return height_frame;}
        SDL_Rect getRectFrame();

        void doThreat(map& gMap);
        void checkToMap(map& map_data);

        int get_enemy_type() const {return enemy_type;}
        void set_enemy_type(const int etype) {enemy_type = etype;}
        //void setAnimationPos(const int& pos_a, const int& pos_b) {animation_a = pos_a; animation_b = pos_b;}
        //void set_input_left(const int& ipLeft) {input_type.left = ipLeft;};
        void impIMGType(SDL_Renderer* screen);
        int get_move_dir() {return input_type.left;}

        std::vector<projectile*> get_bullet_list() const {return t_bullet_list;}
        void set_bullet_list(const std::vector<projectile*>& bl_list) {t_bullet_list = bl_list;}

        void initBullet(projectile* p_bullet, SDL_Renderer* screen);
        void makeBullet(SDL_Renderer* screen, float pl_x_pos, float pl_y_pos);
        void removeBullet(const int& idx);

        int frame_delay = 0;
        void frame_delayer();
        
        void idling(SDL_Renderer* screen, int& desX, int& desY, int pl_pos_x, int pl_pos_y);
        bool alerted;
        // void pursuit(map& map_data, int pl_pos_x, int pl_pos_y);
        int latest;
        int randDelay;
        int etime = 0;
        int restTime = 0;
    private:
        int frame;
        int width_frame;
        int height_frame;
        SDL_Rect frame_clip[THREAT_FRAME_NUM];

        float x_pos;
        float y_pos;
        float x_val;
        float y_val;
        float x_des;
        float y_des;

        int map_x;
        int map_y;

        float spawn_x_pos;
        float spawn_y_pos;
        int health;

        int enemy_type;
        input input_type;

        std::vector<projectile*> t_bullet_list;
        int recentlyHit;
};