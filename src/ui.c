#include "ui.h"

#include <ncursesw/ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "sheet.h"

WINDOW *g_window = NULL;
WINDOW *g_winHeader = NULL;
WINDOW *g_winSheet = NULL;
int g_close = 0;

/**
 * initiate NCURSESW library.
**/
void uiInitiate() {
	g_window = initscr();

	//TODO: more colors!
	if (has_colors() == TRUE) {
		start_color();
		init_pair(UI_COLOR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
		init_pair(UI_COLOR_HEADER, COLOR_WHITE, COLOR_BLUE);
		init_pair(UI_COLOR_SHEET, COLOR_GREEN, COLOR_BLACK);
	}

	noecho();
	keypad(stdscr, TRUE);
	raw();

	int headerSize = 3;
	g_winHeader = newwin(headerSize, COLS, 0, 0);
	g_winSheet = newwin(LINES - headerSize, COLS, headerSize, 0);
}

void uiDispose() {
	delwin(g_winHeader);
	delwin(g_winSheet);
	endwin();
}

void uiHandleInput() {
	switch (getch()) {
		case 'Q':
		case 'q': {
			g_close = 1;
			break;
		}
	}
}

void uiRefreshScreen() {
	wbkgd(g_winHeader, COLOR_PAIR(UI_COLOR_HEADER));
	wclear(g_winHeader);
	wborder(g_winHeader, '|', '|', '-', '-', '+', '+', '+', '+');
	waddch(g_winHeader, 'X');
	
	
	
	wbkgd(g_winSheet, COLOR_PAIR(UI_COLOR_SHEET));
	wclear(g_winSheet);
	waddch(g_winSheet, '#');


	refresh();
	wrefresh(g_winSheet);
	wrefresh(g_winHeader);
}

int uiShouldClose() {
	return g_close;
}