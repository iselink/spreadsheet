/**
 * All TUI releated functions.
**/
#include "ui.h"

#include <ncursesw/ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "sheet.h"

WINDOW *g_window = NULL;

//all header windows
WINDOW *g_winHeader = NULL;
WINDOW *g_winHeaderCellEntry = NULL;

//all sheet releated windows
WINDOW *g_winSheet = NULL;

//close flag; indicate user's request to close program
int g_close = 0;

//size of the header part
int g_headerSize = 3;

//////////////////////////////////////////////////

/**
 * initiate NCURSESW library.
**/
//TODO: init error handling
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

	g_winHeader = newwin(g_headerSize, COLS, 0, 0);
	g_winHeaderCellEntry = subwin(g_winHeader, 1, 16, 1, 1);
	g_winSheet = newwin(LINES - g_headerSize, COLS, g_headerSize, 0);
}

/**
 * Release all resources taken by curses library.
**/
void uiDispose() {
	delwin(g_winHeaderCellEntry);
	delwin(g_winHeader);
	delwin(g_winSheet);
	g_winHeaderCellEntry = NULL;
	g_winHeader = NULL;
	g_winSheet = NULL;
	endwin();
}

/**
 * Handle key presses.
**/
void uiHandleInput() {
	switch (getch()) {
		case KEY_F(10):
		case 'Q':
		case 'q': {
			g_close = 1;
			break;
		}
		//cursor movement
		//TODO: add also viewport movement (with scr lock)
		case KEY_LEFT: {
			setCursor(getCursorPositionY(), getCursorPositionX() - 1);
			break;
		}
		case KEY_RIGHT: {
			setCursor(getCursorPositionY(), getCursorPositionX() + 1);
			break;
		}
		case KEY_UP: {
			setCursor(getCursorPositionY() - 1, getCursorPositionX());
			break;
		}
		case KEY_DOWN: {
			setCursor(getCursorPositionY() + 1, getCursorPositionX());
			break;
		}
	}
}

void uiRefreshScreen() {
	//Header window
	wbkgd(g_winHeader, COLOR_PAIR(UI_COLOR_HEADER));
	wclear(g_winHeader);

	wbkgd(g_winHeaderCellEntry, '_' | COLOR_PAIR(UI_COLOR_SHEET));
	wclear(g_winHeaderCellEntry);
	if (isCursorPositionValid()) {
		char tmp[7];
		memset(tmp, 0, sizeof(char) * 7);
		sheetTranslateColToLetter(getCursorPositionX(), 7, tmp);
		mvwprintw(g_winHeaderCellEntry, 0, 0, "%s%i (%ix%i)", tmp, getCursorPositionY(), getCursorPositionX(), getCursorPositionY());
	} else {
		mvwprintw(g_winHeaderCellEntry, 0, 0, "-/-");
	}

	//Sheet window
	wbkgd(g_winSheet, COLOR_PAIR(UI_COLOR_SHEET));
	wclear(g_winSheet);

	//draw sheet
	for (int x = getViewX(); x < 10; x++) {
		for (int y = getViewY(); y < 10; y++) {
			int realCol = x * 16;
			int realRow = y;
			const char *value = getCellValueAt(x, y);
			if (value != NULL) {
				attr_t attr = getCursorPositionX() == x && getCursorPositionY() == y ? A_REVERSE : 0;
				wattron(g_winSheet, attr);
				mvwaddnstr(g_winSheet, realRow, realCol, value, 16);
				wattroff(g_winSheet, attr);
			}
		}
	}

	wmove(g_winSheet, getCursorPositionY(), getCursorPositionX() * 16);

	refresh();
	wrefresh(g_winHeader);
	wrefresh(g_winHeaderCellEntry);
	wrefresh(g_winSheet);
}

int uiShouldClose() {
	return g_close;
}