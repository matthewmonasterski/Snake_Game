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
int perimeter; //perimeter of border

/*
* Student: Riley
8 Allows user to set difficulty and returns the time based off selection
*/
int setDifficulty() {
    int ch; // selection
    nodelay(stdscr, FALSE); // Wait for user input
    clear();
    mvprintw(7, 10, "Select Difficulty:"); //header
    mvprintw(8, 10, "1. Easy"); //easy
    mvprintw(9, 10, "2. Medium"); //medium
    mvprintw(10, 10, "3. Hard (0.1ms)"); //hard
    refresh(); //refresh

    ch = getch(); //hold user input
    nodelay(stdscr, TRUE); // Return to non-blocking for game
    clear(); //clear options

    if(ch == '1') return 200000; // 200ms
    if(ch == '2') return 150000; // 150ms
    if(ch == '3') return 100000; // 100ms
    return 150000; //default
}

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
    } 
    perimeter = 2 * ((BOX_RIGHT - BOX_LEFT) + (BOX_BOTTOM - BOX_TOP));  //Edited by: Matthew. Adding the required length of snake to win the game
    if (length == perimeter / 2) {
    gameWon();
    *running = 0;
    return;
}
    // check wall collision
    if (x[0] <= BOX_LEFT || x[0] >= BOX_RIGHT ||
        y[0] <= BOX_TOP  || y[0] >= BOX_BOTTOM) {
        gameOver();
        *running = 0;
    }
    // Checks if the snake hits itself
    for (int i = 1; i < length; i++) {
        if (x[0] == x[i] && y[0] == y[i]) { //if x0 equals x1-10 same for y
            gameOver(); // end game
            *running = 0;
            return; // Stop checking
        }
    }
   
}
/*
* Student: Riley
* Displays current length
*/
void displayLength(){
    mvprintw(10, 50, "Score: %d / %d", length, perimeter/2);
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
* Student: Riley, edited by Matthew
* This function moves the snake and handles the logic for arrow keys, building the snake
and displaying the apple and handling collision with the apple. It also implements delay time
*/
void moveSnake(int ch, int *running, int delay) {
   switch (ch) {
    case KEY_UP:
        if (dy != 1) { dx = 0; dy = -1; }  
        break;
    case KEY_DOWN:
        if (dy != -1) { dx = 0; dy = 1; }  //Edited so snake cannot reverse into itself
        break;
    case KEY_LEFT:
        if (dx != 1) { dx = -1; dy = 0; }  
        break;
    case KEY_RIGHT:
        if (dx != -1) { dx = 1; dy = 0; }  
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
    usleep(delay); // Chosen time between moving
    
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
    int current_speed = setDifficulty(); //call setDifficulty and store the result

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
                if (!paused) moveSnake(ch, &running, current_speed); //call moveSnake with direction, running, and current speed
                break;
        }

        if (paused) pauseScreen();
    }
    endwin();
    return 0;
}
