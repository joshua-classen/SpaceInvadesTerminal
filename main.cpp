#include <ncurses.h>
#include "player.hpp"



void start_game(void){
    player std_player;
    std_player.mv_player();
    return;
}


int main()
{

    initscr();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);



    start_game();

    nodelay(stdscr, FALSE);


    endwin();

    return 0;
}
