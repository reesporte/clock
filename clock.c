// clock is an analog clock in your terminal
// written by reese (reesporte.github.io)
#include <math.h>
#include <ncurses.h>
#include <time.h>

#define PI 3.1415926535
#define OFFSET 20
#define RADIUS 18
#define MIDX OFFSET
#define MIDY OFFSET / 2
#define GREEN 1

// msleep is sleep but in ms
void msleep(long msec) {
    struct timespec ts;
    int res = 1;

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    while (res) {
        res = nanosleep(&ts, &ts);
    }
}

typedef struct {
    int x;
    int y;
} coord;

// getCoord gets a coordinate on the clock based on the number of degrees `i`
void getCoord(double i, coord* c) {
    // taking the ceiling so my clock runs a little fast :)
    c->x = MIDX + (int)ceil(RADIUS * cos((i * (PI / 180)) - (PI / 2)));
    c->y = MIDY + (int)ceil((RADIUS / 2) * sin((i * (PI / 180)) - (PI / 2)));
}

// showClock shows a clock
void showClock(int hour, int min, int sec) {
    char buf[3];
    coord c;

    // the clock circle
    for (int i = 0; i < 12; i++) {
        getCoord(i * 30, &c);
        if (i == 0) {
            sprintf(buf, "%d", 12);
        } else {
            sprintf(buf, "%d", i);
        }
        mvprintw(c.y, c.x, buf);
    }

    // use color for the other stuff
    attron(COLOR_PAIR(GREEN));

    // midpoint
    mvprintw(MIDY, MIDX, "*");

    // hour hand
    getCoord(((hour * 60) + min) / 2, &c);
    mvprintw(c.y, c.x, "h");

    // minute hand
    getCoord(min * 6, &c);
    mvprintw(c.y, c.x, "m");

    // second hand
    getCoord(sec * 6, &c);
    mvprintw(c.y, c.x, "s");

    // turn off color for next time
    attroff(COLOR_PAIR(GREEN));
}

int main() {
    time_t now;
    struct tm* t;

    initscr();
    curs_set(0);
    start_color();
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);

    while (1) {
        time(&now);
        t = localtime(&now);
        clear();
        showClock(t->tm_hour % 12, t->tm_min, t->tm_sec);
        refresh();
        msleep(10);
    }

    endwin();
    return 0;
}
