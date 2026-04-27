#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*Box border
Student: Matthew
*/
#define BOX_TOP 2
#define BOX_LEFT 2
#define BOX_BOTTOM 22
#define BOX_RIGHT 42

int y[100], x[100]; //start here
int length = 3; //snake initial length
int dx = 1, dy = 0; //direction (dx 1 is right, -1 left, etc.)
int paused = 0;
int appleX, appleY; // apple position

/* Student: Matthew
 * Initializes curses settings for the game
 */
void init_ui() {
    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);
    noecho();
}
/* Student: Matthew
 * Draws the visible border of the snake pit
 */
void drawBorder() {
    for (int i = BOX_LEFT; i <= BOX_RIGHT; i++) {
        mvaddch(BOX_TOP, i, '#');     // top wall
        mvaddch(BOX_BOTTOM, i, '#'); // bottom wall
    }
    for (int i = BOX_TOP; i <= BOX_BOTTOM; i++) {
        mvaddch(i, BOX_LEFT, '#');   // left wall
        mvaddch(i, BOX_RIGHT, '#');  // right wall
    }
}
/* Student: Matthew
 * Displays game over screen
 */
void gameOver() {
    clear();
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    mvprintw(rows / 2,     cols / 2 - 10, "GAME OVER");
    mvprintw(rows / 2 + 1, cols / 2 - 10, "Press 'q' to quit");
    refresh();
    napms(3000); // show screen for 3 seconds then exit
}
/*
* Student: Riley
* Used Matthew's code for when game is won
*/
void gameWon(){
    clear();
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    mvprintw(rows / 2,     cols / 2 - 10, "YOU WIN");
    mvprintw(rows / 2 + 1, cols / 2 - 10, "Press 'q' to quit");
    refresh();
    napms(3000); // show screen for 3 seconds then exit
}

/*
* Student: Riley
* This gets the number of rows and cols of the terminal and sets apple coordinates
* to be random. 
*/
void apple(){
    int rows, cols; //row and column of terminal
    getmaxyx(stdscr, rows, cols); // gets values for rows and cols
    appleX = (rand() % (BOX_RIGHT - BOX_LEFT - 1) + BOX_LEFT + 1); //get random x coordinate and stay in box
    appleY = (rand() % (BOX_BOTTOM - BOX_TOP - 1)) + BOX_TOP + 1; //get random y coordinate and stay in box
}

/*
* Student: Riley
* This checks to see if the snakes head touches the coordinates of the apple
* and increases the length and sets a new location for the apple.
*/
void checkCollision(int *running){
    if (x[0] == appleX && y[0] == appleY) { //check to see if coordinates of head equal apple coordinates
        length++; //increment snake length
        apple();  //new apple location
        // check wall collision
    }
    if (x[0] <= BOX_LEFT || x[0] >= BOX_RIGHT ||
        y[0] <= BOX_TOP  || y[0] >= BOX_BOTTOM) {
        gameOver();
        *running = 0;
    }
    if(length == 10){ // if length is 10, display game won screen
        gameWon();
        *running = 0;
    }
}
/*
* Student: Riley
* Displays current length
*/
void displayLength(){
    mvprintw(10, 50, "Score: %d / 10", length);
}
/*
* Student: Riley
* This gets the head shape depending on the location that the snake is going in.
*/
char getHeadChar(int dx, int dy) { //get head symbol 
    if (dx == 1) return '>'; //right
    if (dx == -1) return '<'; //left
    if (dy == 1) return 'v'; //down
    if (dy == -1) return '^'; //up
    return 'O';
}

/*
* Student: Riley
* This function moves the snake and handles the logic for arrow keys, building the snake
and displaying the apple and handling collision with the apple.
*/
void moveSnake(int ch, int *running) {
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

    checkCollision(running); //check if head is on apple

    clear(); //clear display
    drawBorder(); 
    displayLength();

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
    
}

//Student: Matthew
void pauseScreen() {
    //screen for when user chooses to pause the game
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    mvprintw(rows / 2, cols / 2 - 10, "PAUSED - press 'p' to resume");
    refresh();
}


int main() {
    init_ui();

    int ch;
    int running = 1;

    for(int i = 0; i < length; i++){ //initial position 
        x[i] = 10-i; //moving right
        y[i] = 10; // 10
    }

    srand(time(NULL)); //rand for position of apple
    apple(); // place apple

    while (running) {
    ch = getch();

    switch (ch) {
        case 'q':
            running = 0;
            break;
        case 'p':
            paused = !paused;
            break;
        default:
            if (!paused) moveSnake(ch, &running);
            break;
    }

    if (paused) pauseScreen();
}
    endwin();
    return 0;
}
