#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <random>
#include <ncurses.h>
#include <vector>
#include <chrono>
#include "player.hpp"


using namespace std;
class enemy_yx
{
public:
    void give_yx(int _y, int _x);
    unsigned return_y(void);
    unsigned return_x(void);
    void set_alive_flag(bool flag);
    bool return_alive_flag(void);

private:
    unsigned x, y;
    bool alive = true;
};


bool enemy_yx::return_alive_flag(void){
    return alive;
}

unsigned enemy_yx::return_x(void){
    return x;
}

unsigned enemy_yx::return_y(void){
    return y;
}

void enemy_yx::give_yx(int _y, int _x){
    y = _y;
    x = _x;
}

void enemy_yx::set_alive_flag(bool flag){
    alive = flag;
}



class bomb_data
{
public:
    //function's
    void set_new_bomb(unsigned curr_bomb_y, unsigned curr_bomb_x);
    int increment_bomb(std::vector<bomb_data*>::iterator me);
    void init_bomb_data(const int y, const int x, const unsigned *player_y, const unsigned *player_x);



    //object's
    unsigned scr_y, scr_x;
    std::chrono::steady_clock::time_point tp_bomb_data_start, tp_bomb_data_end;
    std::chrono::steady_clock::duration dur_bomb_data;
    int bomb_speed = 120;
    unsigned curr_pos_bomb_y, curr_pos_bomb_x;
    const unsigned *curr_player_y, *curr_player_x;

private:

};

#define MVRIGHT 0
#define MVLEFT 1


class enemy
{
public:
    void set_enemy(const unsigned &wide_x, const unsigned &height_y, const unsigned *curr_y, const unsigned *curr_x);

    bool is_collision(const int &curr_pos_bullet_y, const int &curr_pos_bullet_x);
    bool enemy_move(void);

    //bomb



    bool bomb_func(void);



private:
    void draw_enemy(void);
    unsigned wide_x_scr, height_y_scr;
    vector<enemy_yx> ene_yx_vec;
    unsigned lenght, height;
    std::vector<enemy_yx>::iterator begin, mem_begin;
    std::vector<enemy_yx>::iterator end;
    std::chrono::steady_clock::time_point tp_enemy_start, tp_enemy_end;
    std::chrono::steady_clock::duration ene_dur;
    bool enemy_move_flag = false;
    bool curr_movement = false; // [false] to move to the right side. [true] to move to the left side.
    unsigned curr_max_posx, curr_min_posx;
    const unsigned *curr_player_y, *curr_player_x;
    unsigned enemy_speed = 100;
    bool set_enemy_down = false;
    bool any_enemy_alive;
    vector<enemy_yx> vec_alive_enemys;



    //bomb
    std::default_random_engine dre;
    vector<bomb_data*> bomb_data_ptr_vec;
    std::chrono::steady_clock::time_point tp_bomb_intervall_start, tp_bomb_intervall_end;
    std::chrono::steady_clock::duration dur_bomb_intervall;
    bool bool_bomb_intervall_running = false;
    unsigned y_bomb, x_bomb;
    unsigned choose;

    //player
    unsigned player_live = 3;
};


#define ALL_OK 0
#define OPEN_MENUE 1

bool enemy::enemy_move(void){
    mvprintw(0,0, "PLAYER Lives: %d", player_live);
    refresh();
    if(!enemy_move_flag){
        tp_enemy_start = std::chrono::steady_clock::now();
        enemy_move_flag = true;
        return ALL_OK;
    }
    if(enemy_move_flag){
        tp_enemy_end = std::chrono::steady_clock::now();
        ene_dur = tp_enemy_end - tp_enemy_start;
        if(bomb_func()){
            werase(stdscr);
            mvprintw(0,0, "GAME OVER!");
            mvprintw(1,0,"Press Return to quit the process!");
            refresh();
            nodelay(stdscr, FALSE);
            int a;
            while(a = getch())
                if(a == 0x0a)
                    break;
            return OPEN_MENUE;
        }

        if(std::chrono::duration_cast<std::chrono::milliseconds>(ene_dur).count() >= enemy_speed){
            curr_max_posx = 0;
            curr_min_posx = wide_x_scr-1;


            for(begin = ene_yx_vec.begin();begin != ene_yx_vec.end();++begin){
                if(begin->return_alive_flag()){ //checks alive_flag
                    any_enemy_alive = true;
                    if(begin->return_x() > curr_max_posx){
                        curr_max_posx = begin->return_x();
                    }
                    if(begin->return_x() < curr_min_posx){
                        curr_min_posx = begin->return_x();
                    }
                }

            }



            if(!any_enemy_alive){ //if all enemys are dead
                werase(stdscr);
                mvprintw(0,0, "GAME OVER!");
                mvprintw(1,0, "You Win!");
                mvprintw(2,0,"Press Return to quit the process!");
                refresh();
                nodelay(stdscr, FALSE);
                int a;
                while(a = getch())
                    if(a == 0x0a)
                        break;
                return OPEN_MENUE;
            }

            else
                any_enemy_alive = false; //reset


            while(true){
                if(curr_movement == MVLEFT){ //move to the left ; "if(curr_movement == MVLEFT)" equal "if(curr_movement)"
                    if(curr_min_posx == 0){ //if the block is on the left side
                        curr_movement = MVRIGHT; //set movement to the right
                        set_enemy_down = true; //define the value that will say that the enemy must be set down a layer
                        continue;
                    }
                    //move the enemys to the left side
                    begin = ene_yx_vec.begin(); end = ene_yx_vec.end();
                    for(;begin != end; ++begin){
                        mvaddch(begin->return_y(), begin->return_x(), ' ');     //delete all enemys
                        if(set_enemy_down){
                            begin->give_yx(begin->return_y()+2, begin->return_x()); //set new enemy position down
                            //if begin->return_y()+2 returns curr_y of the player, Game Over!
                            if(begin->return_y() >= *curr_player_y && begin->return_alive_flag() == 1){
                                werase(stdscr);
                                mvprintw(0,0, "GAME OVER!");
                                mvprintw(1,0,"Press Return to quit the process!");
                                refresh();
                                nodelay(stdscr, FALSE);
                                int a;
                                while(a = getch())
                                    if(a == 0x0a)
                                        break;
                                return OPEN_MENUE;
                                //open menu

                            }
                        }
                        else
                            begin->give_yx(begin->return_y(), begin->return_x()-1); //set new enemy position


                    }
                    set_enemy_down = false;

                    draw_enemy();


                    tp_enemy_start = tp_enemy_end;
                }

                else{ //move to the right ; equivalent to if(curr_movement == MVRIGHT)
                    if(curr_max_posx == wide_x_scr-1){
                        curr_movement = MVLEFT; //set it to the left
                        set_enemy_down = true;
                        continue;
                    }
                    //increment the enemys to the right side.
                    begin = ene_yx_vec.begin(); end = ene_yx_vec.end();
                    for(;begin != end; ++begin){
                        mvaddch(begin->return_y(), begin->return_x(), ' '); //delete enemy
                        if(set_enemy_down){
                            begin->give_yx(begin->return_y()+2, begin->return_x()); //set new enemy position down
                            //enemyif begin->return_y()+2 returns curr_y of the player, Game Over!
                            if(begin->return_y() == *curr_player_y && begin->return_alive_flag() == 1){
                                werase(stdscr);
                                mvprintw(0,0, "GAME OVER!");
                                mvprintw(1,0,"Press Return to quit the process!");
                                refresh();
                                nodelay(stdscr, FALSE);
                                int a;
                                while(a = getch())
                                    if(a == 0x0a)
                                        break;
                                return OPEN_MENUE;

                            }
                        }
                        else
                            begin->give_yx(begin->return_y(), begin->return_x()+1); //set new enemy position
                    }
                    set_enemy_down = false;
                    draw_enemy();


                    tp_enemy_start = tp_enemy_end;
                }
                break;
            }

        }
    }
    return ALL_OK;
}







bool enemy::bomb_func(void){

    for(std::vector<bomb_data*>::iterator begin = bomb_data_ptr_vec.begin(); begin != bomb_data_ptr_vec.end();){ //if the vector bomb_data_ptr_vec is not empty


        (*begin)->tp_bomb_data_end = std::chrono::steady_clock::now();
        (*begin)->dur_bomb_data = (*begin)->tp_bomb_data_end - (*begin)->tp_bomb_data_start;

        if(std::chrono::duration_cast<std::chrono::milliseconds>((*begin)->dur_bomb_data).count() >= (*begin)->bomb_speed){

            switch((*begin)->increment_bomb(begin)){
                case 0: //bomb is still flying
                break; //testing

                case 1: // if the bomb fell out of the screen
                begin = bomb_data_ptr_vec.erase(begin); //code that will delete the element that is pointed by begin
                break;

                case 2: //if the bomb collidied with the player
                begin = bomb_data_ptr_vec.erase(begin); //code that will delete the element that is pointed by begin
                --player_live;
                mvprintw(0,0, "                     ");
                mvprintw(0,0, "PLAYER Lives: %d", player_live);
                refresh();
                break;


            }

            if(!player_live){ // if the player have no live
                werase(stdscr);
                mvprintw(0,0, "GAME OVER!");
                mvprintw(1,0, "You died!");
                refresh();
                nodelay(stdscr, FALSE);
                return OPEN_MENUE;

            }

            if(begin == bomb_data_ptr_vec.end()) //if we successfully parsed the vector
                break; //go out of the for loop



        }

        ++begin; //next element

    }
    if(!bool_bomb_intervall_running){ //if the timer is not set

        tp_bomb_intervall_start = std::chrono::steady_clock::now(); //set timer
        bool_bomb_intervall_running = true; //say that the bomb timer is now running
    }

    tp_bomb_intervall_end = std::chrono::steady_clock::now();
    dur_bomb_intervall = tp_bomb_intervall_end - tp_bomb_intervall_start;
    if(std::chrono::duration_cast<std::chrono::milliseconds>(dur_bomb_intervall).count() >= 150){ //150 ms = 0.15s ;
        tp_bomb_intervall_start = tp_bomb_intervall_end;
        //should be set a bomb?



        std::uniform_int_distribution<short> bin_choose(0, 1);
        if(!bin_choose(dre)) //maybe i can remove the ! that stzands before bin_choose
            return ALL_OK; // cause it should not set a bomb ; return to the enemy_move fkt.





        for(begin = ene_yx_vec.begin(); begin != ene_yx_vec.end(); ++begin){
            if(begin->return_alive_flag()) //checks alive_flag
                vec_alive_enemys.push_back(*begin); //The vector must be cleared later! //could be possible that the vector is empty if we have no enemy that is alive


        }

        unsigned max_possibilities = vec_alive_enemys.size(); //
        if(max_possibilities == 0){ //test
            ++max_possibilities;
        }
        std::uniform_int_distribution<unsigned> choose_dist(0, --max_possibilities); // max_possibilities-1 is important
        //Choose a enemy

        choose = choose_dist(dre);
        y_bomb = vec_alive_enemys[choose].return_y();
        x_bomb = vec_alive_enemys[choose].return_x();

        vec_alive_enemys.clear(); //clear the vector
        //now let the enemy set a bomb!


        bomb_data *bomb_data_ptr = new bomb_data;
        bomb_data_ptr->set_new_bomb(y_bomb, x_bomb);
        bomb_data_ptr->init_bomb_data(height_y_scr ,wide_x_scr, curr_player_y, curr_player_x);
        bomb_data_ptr_vec.push_back(bomb_data_ptr);

        return ALL_OK; //go back to the enemy_move fkt.


    }
    else
        return ALL_OK; //go back to the enemy_move fkt.



}





int bomb_data::increment_bomb(std::vector<bomb_data*>::iterator me){
    if(!((*me)->curr_pos_bomb_y >= scr_y-1)){ //if bomb is not out of the screen
        mvaddch((*me)->curr_pos_bomb_y, (*me)->curr_pos_bomb_x, ' ');

        ++(*me)->curr_pos_bomb_y;

        //if we have a collision

        if((*curr_player_y == (*me)->curr_pos_bomb_y) && (*curr_player_x == (*me)->curr_pos_bomb_x)){
            mvaddch(((*me)->curr_pos_bomb_y) -1, (*me)->curr_pos_bomb_x, ' '); //clear the position where the bomb was last seen


            refresh();
            return 2; //Cause we have a collision with the player
        }


        //if we have no collision
        mvaddch((*me)->curr_pos_bomb_y, (*me)->curr_pos_bomb_x, 'B');
        refresh();
        (*me)->tp_bomb_data_start = (*me)->tp_bomb_data_end;
        return 0; //Cause we have no collision
    }
    else{
        mvaddch((*me)->curr_pos_bomb_y, (*me)->curr_pos_bomb_x, ' ');
        refresh();
        return 1; //return 1 cause we the bomb fell out of the screen
    }
}

void bomb_data::set_new_bomb(unsigned curr_bomb_y, unsigned curr_bomb_x){
    curr_pos_bomb_x = curr_bomb_x;
    curr_pos_bomb_y = curr_bomb_y+1; // set bomb under the current enemy

    tp_bomb_data_start = std::chrono::steady_clock::now();//set "start timer"


    mvaddch(curr_pos_bomb_y, curr_pos_bomb_x, 'B'); //draw the first bomb under the enemy
    refresh();

    return;
}

void bomb_data::init_bomb_data(const int y, const int x, const unsigned *player_y, const unsigned *player_x){ //add player information
    scr_y = y;
    scr_x = x;
    curr_player_y = player_y;
    curr_player_x = player_x;
}




void enemy::set_enemy(const unsigned &wide_x, const unsigned &height_y, const unsigned *curr_y, const unsigned *curr_x){
    curr_player_y = curr_y;
    curr_player_x = curr_x;
    height_y_scr = height_y;
    wide_x_scr = wide_x;
    lenght = 40; height = 5;
    enemy_yx *ene_arr = new enemy_yx;
    for(unsigned x = wide_x_scr/2 - lenght/2 ; x != wide_x_scr/2 + lenght/2; ++x){
        for(unsigned y(1); y != height+1; ++y){
            ene_arr->give_yx(y,x);
            ene_yx_vec.push_back(*ene_arr);
        }
    }
    begin = ene_yx_vec.begin(); end = ene_yx_vec.end();
    draw_enemy();
}





void enemy::draw_enemy(void){
    begin = ene_yx_vec.begin(); end = ene_yx_vec.end();
    for(; begin != end; ++begin){
        if(begin->return_alive_flag())
            mvaddch(begin->return_y(), begin->return_x(), 'Y');
    }
    refresh();

}



bool enemy::is_collision(const int &curr_pos_bullet_y, const int &curr_pos_bullet_x){
    begin = ene_yx_vec.begin(); end = ene_yx_vec.end();


    for(int y, x; begin != end; ++begin){
        y = begin->return_y();
        x = begin->return_x();



        if(y == curr_pos_bullet_y && x == curr_pos_bullet_x && begin->return_alive_flag()){ // begin->return_alive_flag()   returns true if the enemy is alive.
            begin->set_alive_flag(false);
            mvaddch(y,x, ' ');
            refresh();
            return true; //deallocate the memory of the bullet object that hit the enemy
        }

    }


    return false;
}




#endif // ENEMY_HPP

