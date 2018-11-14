#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define USER 1
#define WALL 2
#define TICKRATE 100
//length global so everyone can use it, collision also global scope
int SNAKEY_LENGTH = 2;
int COLLISION = 1;

//struct definition

typedef struct spart {
    int x;
    int y;
} snakeypart;

snakeypart food;
int isfood = 1;

//Function Prototypes
int move_snakey(WINDOW *win, int direction, snakeypart snakey[]);
int checkCollision(snakeypart snakey[], int x, int y);
enum direction { UP, DOWN, RIGHT, LEFT };
void spawn_food();
void print_score();
void quit(int reason);


int main(int argc, char *argv[]) {

    int i, ch;

    initscr();
    noecho();
    cbreak();
    timeout(TICKRATE);
    keypad(stdscr, TRUE);

    printw("Snake Game  -  Press x to quit...");

    refresh();

    snakeypart snakey[SNAKEY_LENGTH];

    int sbegx = (COLS - SNAKEY_LENGTH) / 2;
    int sbegy = (LINES - 1) / 2;

    for (i = 0; i < SNAKEY_LENGTH; i++) {
        snakey[i].x = sbegx + i;
        snakey[i].y = sbegy;
    }
    int cur_dir = RIGHT;


    spawn_food(); // getting food on the board
    while ((ch = getch()) != 'x') {
        move_snakey(stdscr, cur_dir, snakey);
        if(COLLISION=0)
            break;
        if(ch != ERR) {
            switch(ch) {
                case KEY_UP:
                    cur_dir = UP;
                    break;
                case KEY_DOWN:
                    cur_dir = DOWN;
                    break;
                case KEY_RIGHT:
                    cur_dir = RIGHT;
                    break;
                case KEY_LEFT:
                    cur_dir = LEFT;
                    break;
                default:
                    break;
            }

        }
    }

    quit(USER);

    return 0;

}

int move_snakey(WINDOW *stdscr, int direction, snakeypart snakey[]) {

    wclear(stdscr);

    for (int i = 0; i < SNAKEY_LENGTH - 1; i++) {
        snakey[i] = snakey[i + 1];
        mvwaddch(stdscr, snakey[i].y, snakey[i].x, '#');
    }

    int x = snakey[SNAKEY_LENGTH - 1].x;
    int y = snakey[SNAKEY_LENGTH - 1].y;
    switch (direction) {
        case UP:
            y - 1 == 0 ? y = LINES - 2 : y--;
            break;
        case DOWN:
            y + 1 == LINES - 1 ? y = 1 : y++;
            break;
        case RIGHT:
            x + 1 == COLS - 1 ? x = 1 : x++;
            break;
        case LEFT:
            x - 1 == 0 ? x = COLS - 2 : x--;
            break;
        default:
            break;
    }

    snakey[SNAKEY_LENGTH - 1].x = x;
    snakey[SNAKEY_LENGTH - 1].y = y;

    mvwaddch(stdscr, y, x, '#');
    if (isfood == 0){
        mvwaddch(stdscr, food.y, food.x, '$');
    }
    else {
        spawn_food();
    }
    wresize(stdscr, LINES, COLS);
    checkCollision(snakey, x, y);
    box(stdscr, '|' , '-');

    wrefresh(stdscr);

    return 0;
}

void print_score(){
    mvwaddch(stdscr, LINES+1, LINES+1, 'x');
}

void spawn_food(){
    srand(time(NULL));
    food.x= 1 + (rand() % (COLS-2));
    food.y= 1 + (rand() % (LINES-2));
    isfood = 0;
}

int checkCollision(snakeypart snakey[], int x, int y){

    if (x == 1 || y == 1){ // wall collision
        quit(WALL);
    }
    else if (x == food.x && y == food.y){ // food collision
        mvwdelch(stdscr, food.y, food.x );
        isfood = 1;
    }
    //Trying to get if the snake hits the snake lol
    //else if (x == x+1 || y == y+1){
    //    quit(WALL);
    //}

}

void quit(int reason){
    delwin(stdscr);
    endwin();
    refresh();

    switch( reason ){
        case USER:
            printf("\nUser hit x! \n");
            break;
        case WALL:
            printf("\nYou hit hit the wall!\n");
        default:
            printf("Goodbye!\n");
            break;
    }
    exit(EXIT_SUCCESS);
}
