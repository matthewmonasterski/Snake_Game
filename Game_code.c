#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <signal.h>

//Student: Riley
void updateDirection() {
    //You can paste your logic here 
}
//Student: Riley
void moveSnake() {
    //paste you movement logic here
}
//Student: Matthew
void pauseScreen() {
    //screen for when user chooses to pause the game
}
//Student: Matthew
void render() /*redraws the entire game state each frame */{
    
    clear();
    //draw snake and border logic here

    refresh();

}


int main() {
    init_ui();

    int ch;
    int paused = 0;
    int running = 1;

    while (running) {
        ch = getch();

        switch (ch) {
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
        }

        if (paused) {
            pauseScreen();
        } else {
            moveSnake();   
            render();       
        }
    }

    endwin();
    return 0;

}