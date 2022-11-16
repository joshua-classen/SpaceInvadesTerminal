#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <ncurses.h>
#include "bullet.hpp"

class player
{
public:
    void mv_player(void);
    unsigned curr_y, curr_x;
    unsigned *curr_y_ptr, *curr_x_ptr;
    //bool is_player_collision(const int &curr_pos_bomb_y, const int &curr_pos_bomb_x);
    unsigned live = 3;
};










void player::mv_player(void){
    unsigned height_y , wide_x;
    getmaxyx(stdscr, height_y, wide_x);

    if(height_y < 20 || wide_x < 55){ // if the terminal window is to small
        mvprintw(0,0, "Enlarge the terminal!");
        refresh();
        nodelay(stdscr, FALSE);
        getch();
        return; //quit the game
    }

    curr_y = height_y-3;
    curr_x = wide_x/2;
    curr_y_ptr = &curr_y;
    curr_x_ptr = &curr_x;
    mvaddch(curr_y, curr_x, 'X');
    //refresh(); //the refresh will do the getch in the for loop

    bullet std_bullet;
    enemy std_enemy;
    enemy *enemy_pointer = &std_enemy;
    std_bullet.bullet_scrinit(height_y, wide_x, enemy_pointer);
    std_enemy.set_enemy(wide_x, height_y, curr_y_ptr, curr_x_ptr);





    for(int i;;){


        std_bullet.bullet_func(curr_y, curr_x, false);

        if(std_enemy.enemy_move())
            return;
        mvaddch(curr_y, curr_x, 'X');
        refresh();

        while((i = getch()) != ERR){

            switch(i) {

                case KEY_LEFT:
                if(curr_x > 0){
                    mvaddch(curr_y, curr_x--, ' ');
                    mvaddch(curr_y, curr_x, 'X');
                    std_bullet.bullet_func(curr_y, curr_x, false);
                    break;
                }
                else
                    break;

                case KEY_RIGHT:
                if(curr_x+1 < wide_x){
                    mvaddch(curr_y, curr_x++, ' ');
                    mvaddch(curr_y, curr_x, 'X');
                    std_bullet.bullet_func(curr_y, curr_x, false);
                    break;
                }
                else
                    break;

                case ' ':
                std_bullet.bullet_func(curr_y, curr_x, true);
                    break;

            }
        }
    }
}






#endif // PLAYER_HPP
