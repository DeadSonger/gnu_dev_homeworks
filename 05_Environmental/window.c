#include <ncurses.h>
#include "own_regexp.h"
#include <locale.h>

#define DX 3
#define ONLELINE 3
#define MAXSTR 80

int main() {

	setlocale(LC_ALL, "");

	WINDOW *winA, *winB, *winO;
	unsigned char inA[MAXSTR], inB[MAXSTR];
	int half;

	initscr();
	cbreak();
	printw("Input:");
	refresh();

	half = (COLS-2*DX)/2;
	winA = newwin(ONLELINE, half, DX, DX);
	winB = newwin(ONLELINE, half, DX, DX+half);
	winO = newwin(LINES-ONLELINE-DX*2, (COLS-2*DX), DX+ONLELINE, DX);
	keypad(winA, TRUE);
	keypad(winB, TRUE);
	scrollok (winO, TRUE);
	wmove(winO, 1, 0);

	box(winA, 0, 0); wrefresh(winA);
	box(winB, 0, 0); wrefresh(winB);
	box(winO, 0, 0); wrefresh(winO);

	do {
		werase(winA); box(winA, 0, 0);
		mvwgetnstr(winA, 1, 1, (char*)inA, MAXSTR);
		werase(winB); box(winB, 0, 0);
		mvwgetnstr(winB, 1, 1, (char*)inB, MAXSTR);
		wprintw(winO, "  pattern: %s\t\t\tsubject: %s\n", inA, inB);
		matchRegexp(inA, inB, winO);
		wprintw(winO, "  \n");
		box(winO, 0, 0);
		wrefresh(winO);
	} while(*inA);

	endwin();

}

