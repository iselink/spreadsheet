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

//////////////////////////////////////////////////

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

	//size of the header part
	int headerSize = 3;
	g_winHeader = newwin(headerSize, COLS, 0, 0);
	g_winHeaderCellEntry = subwin(g_winHeader, 1, 16, 1, 1);
	g_winSheet = newwin(LINES - headerSize, COLS, headerSize, 0);
}

void uiDispose() {
	delwin(g_winHeaderCellEntry);
	delwin(g_winHeader);
	delwin(g_winSheet);
	endwin();
}

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
	int viewX = getViewX();
	int viewY = getViewY();

	int cellXFullOffset = viewX / 16;
	int offsetX = viewX % 16;

	for (int i = 0; i < 8; i += 2) {
		int cx = 0;
		for (int c = 0; c < COLS; c++) {
			mvwaddch(g_winSheet, i, c, cx > 15 ? ACS_PLUS : ACS_HLINE);
			cx++;
			if (cx > 16) {
				cx = 0;
			}
		}
	}

	refresh();
	wrefresh(g_winHeader);
	wrefresh(g_winHeaderCellEntry);
	wrefresh(g_winSheet);

}

int uiShouldClose() {
	return g_close;
}