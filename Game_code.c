#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

int y[100], x[100]; //start here
int length = 3; //snake initial length
int dx = 1, dy = 0; //direction (dx 1 is right, -1 left, etc.)
int paused = 0;
int appleX, appleY; // apple position


char getHeadChar(int dx, int dy) { //get head symbol 
    if (dx == 1) return '>'; //right
    if (dx == -1) return '<'; //left
    if (dy == 1) return 'v'; //down
    if (dy == -1) return '^'; //up
    return 'O';
}

//Student: Riley
void moveSnake(int ch) {
    switch (ch) { //switch statement to see what use clicks
        case KEY_UP: //go right
            dx = 0; dy = -1; 
            break;
        case KEY_DOWN:
            dx = 0; dy = 1; //go down
            break;
        case KEY_LEFT: //go left
            dx = -1; dy = 0;
            break;
        case KEY_RIGHT: //go right
            dx = 1; dy = 0;
            break;
    }
    for (int i = length - 1; i > 0; i--) { //move body parts
        x[i] = x[i - 1]; //move in x direction
        y[i] = y[i - 1]; //move in y direction
    }
    x[0] += dx; //move head
    y[0] += dy; //move head

    clear(); //clear display

    for (int i = 0; i < length; i++) { //loop through all body parts to print
        if (i == 0) { // if at head postion 
            mvaddch(y[i], x[i], getHeadChar(dx, dy)); //print head to display with head from output
        } else { //for body
            if(dx != 0){ //if x is 1 or -1 use - for body
                mvaddch(y[i], x[i], '-');
            }
            else{ // else user | for vertical movement
                mvaddch(y[i], x[i], '|');
            }       
        }
    }
    mvaddch(appleY, appleX, 'A'); // draw apple

    refresh(); //refresh
    usleep(200000); // (0.2s) between moving
    
    /*if (paused) {
            pauseScreen();
    } else {
        moveSnake();   
        render();       
    }*/
}

void apple(){
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    appleX = rand() % cols;
    appleY = rand() % rows;
}
//Student: Matthew
void pauseScreen() {
    //screen for when user chooses to pause the game
}
//Student: Matthew
//void render() /*redraws the entire game state each frame */{
    
   // clear();
    //draw border logic here

   // refresh();

//}


int main() {
    //init_ui();
    initscr(); // initialize curses 
    keypad(stdscr, TRUE); //user keypad
    curs_set(0); // removes input box

    int ch;
    int running = 1;

    for(int i = 0; i < length; i++){ //initial position 
        x[i] = 10-i; //moving right
        y[i] = 10; // 10
    }
    nodelay(stdscr, TRUE);  //no delay on user input

    srand(time(NULL)); //rand for position of apple
    apple(); // place apple

    while (running) {
        ch = getch();
        moveSnake(ch);
        /*switch (ch) {
            case 'q':
                running = 0;
                break;
            case 'p':
                paused = !paused;
                break;
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                updateDirection(ch); 
                break;
        }*/
    }
    endwin();
    return 0;
}
