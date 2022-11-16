#ifndef BULLET_HPP
#define BULLET_HPP

#include <chrono>
#include <ncurses.h>
#include <enemy.hpp>
#include <vector>




class bullet_data
{
public:

    void set_new_bullet(int curr_y, int curr_x);
    void bullet_data_scrinit(const unsigned &y, const unsigned &x, enemy *_std_enemy);
    bool increment_bullet(std::vector<bullet_data*>::iterator me);


    std::chrono::steady_clock::time_point start, end;
    std::chrono::steady_clock::duration dur;
    enemy* std_enemy;
    unsigned scr_y, scr_x;
    unsigned curr_pos_bullet_y, curr_pos_bullet_x;
    bool bullet_flag = false;
    int bullet_speed = 100;//bullet speed. The higher, the slower the bullet will move.


private:



};





class bullet
{
public:
    void bullet_scrinit(const int y, const int x, enemy *input_enemy);
    void bullet_func(const int &curr_y, const int &curr_x, const bool is_space_pressed);

private:
    vector<bullet_data*> bullet_data_vec;
    vector<bullet_data*>::iterator begin, end;
    enemy* std_enemy;
    unsigned scr_y, scr_x;
    std::chrono::steady_clock::time_point start_tp_bullet_fire, end_tp_bullet_fire;
    //std::chrono::steady_clock::duration dur_bullet_fire = 10000000;
};



void bullet_data::bullet_data_scrinit(const unsigned &y, const unsigned &x, enemy *_std_enemy){
    scr_y = y; scr_x = x;
    std_enemy = _std_enemy;
}




void bullet::bullet_scrinit(const int y, const int x, enemy *input_enemy){
    scr_y = y;
    scr_x = x;
    std_enemy = input_enemy;

}




void bullet::bullet_func(const int &curr_y, const int &curr_x, const bool is_space_pressed){
    if(is_space_pressed /*&& std::chrono::duration_cast<std::chrono::milliseconds>(dur_bullet_fire.count()) >= 10000000*/){

            bullet_data *bullet_data_ptr = new bullet_data;
            bullet_data_vec.push_back(bullet_data_ptr);
            bullet_data_ptr->set_new_bullet(curr_y, curr_x);
            bullet_data_ptr->bullet_data_scrinit(scr_y, scr_x, std_enemy);


            //delete bullet_data_ptr; //This will make problems



            return;
    }

    else if(!is_space_pressed){ //if space is not pressed
        for(std::vector<bullet_data*>::iterator begin=bullet_data_vec.begin();begin != bullet_data_vec.end();){ //go throuht the vector


            if((*begin)->bullet_flag){


                (*begin)->end = std::chrono::steady_clock::now(); //operation of the object which belongs to a bullet_data class.
                (*begin)->dur = (*begin)->end - (*begin)->start; //difference of the time
                if(std::chrono::duration_cast<std::chrono::milliseconds>((*begin)->dur).count() >= (*begin)->bullet_speed){ //bullet_speed must be public
                    if((*begin)->increment_bullet(begin)){ // if "(*begin)->increment_bullet(begin)" returns true, *begin will be deallocated!
                        begin = bullet_data_vec.erase(begin); //bullet_data_vec.erase(begin) returns a pointer to the next element
                        if(begin == bullet_data_vec.end())
                            break;


                    }
                }
                else{
                    ++begin;
                }
            }
            else{
                ++begin;
            }



        }

    return;
    }
}







void bullet_data::set_new_bullet(int curr_y, int curr_x){

    curr_pos_bullet_y = --curr_y;
    curr_pos_bullet_x = curr_x;
    start = std::chrono::steady_clock::now();
    mvaddch(curr_pos_bullet_y, curr_pos_bullet_x, '|');
    refresh();
    bullet_flag = true;
}

bool bullet_data::increment_bullet(std::vector<bullet_data*>::iterator me){
    if((*me)->curr_pos_bullet_y){ //if it is not NULL, the block will be execute ; it means that the bullet is not in the first layer
        mvaddch((*me)->curr_pos_bullet_y, (*me)->curr_pos_bullet_x, ' ');
        --((*me)->curr_pos_bullet_y);


        if(std_enemy->is_collision((*me)->curr_pos_bullet_y, (*me)->curr_pos_bullet_x)){ //the block will be executed if a collision exist.
            //(*me)->bullet_flag = false; //not needed
            return true; //returns true to deallocate the memory of one Element from the vector bullet_data_vec
        }

        //if we have no collision
        mvaddch((*me)->curr_pos_bullet_y, (*me)->curr_pos_bullet_x, '|');
        refresh();
        (*me)->start = (*me)->end;
        return false; //returns false. Nothing will happen!
    }
    else{
        mvaddch((*me)->curr_pos_bullet_y, (*me)->curr_pos_bullet_x, ' ');
        refresh();
        //(*me)->bullet_flag = false;//not needed
        return true; //returns true to deallocate the memory of one Element from the vector bullet_data_vec //maybe return this ?
    }
}



#endif // BULLET_H
