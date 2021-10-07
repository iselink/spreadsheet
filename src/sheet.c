/**
 * Functions releated to the sheet manipulation. 
**/
#include "sheet.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int g_row_count = -1;
int g_cols_count = -1;

int g_cursor_x = -1;
int g_cursor_y = -1;

int g_view_x = 0;
int g_view_y = 0;

Cell **g_cells = NULL;

/**
 * Allocate memory for new sheet.
 * If error occured while memory allocation, then 1 is returned.
**/
int initiateSheet(int rows, int cols) {
	g_row_count = rows;
	g_cols_count = cols;
	g_cells = (Cell **)malloc(sizeof(Cell *) * cols * rows);
	if (g_cells == NULL) {
		return 1;
	}
	memset(g_cells, 0, sizeof(Cell *) * cols * rows);
	return 0;
}

/**
 * Release all of the memory allocated by the sheet.
**/
void disposeSheet() {
	if (g_cells == NULL) {
		return;
	}

	g_cursor_x = -1;
	g_cursor_y = -1;
	g_cols_count = -1;
	g_row_count = -1;
	g_view_x = -1;
	g_view_y = -1;

	for (int i = 0; i < g_cols_count * g_row_count; i++) {
		if (g_cells[i] == NULL)
			continue;

		if (g_cells[i]->text != NULL) {
			free((void *)g_cells[i]->text);
		}
		free(g_cells[i]);
	}

	free(g_cells);
	g_cells = NULL;
}

/**
 * Set cursor to the given cell.
 * If position is out of bound, then cursor is set to the nearest cell.
**/
void setCursor(int row, int col) {
	if (row < 0) {
		g_cursor_y = 0;
	} else if (row > g_row_count) {
		g_cursor_y = g_row_count;
	} else {
		g_cursor_y = row;
	}

	if (col < 0) {
		g_cursor_x = 0;
	} else if (col > g_cols_count) {
		g_cursor_x = g_cols_count;
	} else {
		g_cursor_x = col;
	}
}

/**
 * Set cell's value by parameter.
 * If NULL is passed, then cell is cleared.
 * Cell is select via cursor.
 */
//TODO: reset cell's value if text is null.
int setCellValue(const char *text) {
	if (!isCursorPositionValid())
		return 1;

	int index = g_cursor_x + (g_cursor_y * g_cols_count);
	//TODO: boundary check

	//if cell isn't allocated, then allocate it
	if (g_cells[index] == NULL) {
		g_cells[index] = (Cell *)malloc(sizeof(Cell));
		if (g_cells[index] == NULL) {
			return 1;
		}
	}

	if (text != NULL) {
		//set cell's value
		int textLen = strlen(text);
		g_cells[index]->text = (const char *)malloc(sizeof(const char) * textLen);
		if (g_cells[index]->text == NULL) {
			return 1;
		}

		g_cells[index]->textLenght = textLen;
		strcpy((char *)g_cells[index]->text, text);

	} else {
		//reset cell's value
		g_cells[index]->textLenght = 0;
		free((void *)g_cells[index]->text);
		g_cells[index]->text = NULL;
	}

	return 0;
}

/**
 * Return cell's value if given cell have some.
 * Otherwise return null.
 */
const char *getCellValueAt(int x, int y) {
	if (g_cells == NULL)
		return NULL;

	int index = x + (y * g_cols_count);
	if (index > g_cols_count * g_row_count) {
		return NULL;
	}
	return g_cells[index] != NULL ? g_cells[index]->text : NULL;
}

const char *getCellValue() {
	return getCellValueAt(g_cursor_x, g_cursor_y);
}

/**
 * Check if cursor position is in sheet boundary.
 */
int isCursorPositionValid() {
	return g_cursor_x >= 0 && g_cursor_y >= 0 && g_cursor_x <= g_cols_count && g_cursor_y <= g_row_count;
}

/**
 * Translate collumn number into letter as is common it other spreadsheet SW.
 */
void sheetTranslateColToLetter(int col, int text_size, char *text) {
	if (col < 0 || text_size < 1 || text == NULL) {
		return;	 //TODO: just do better error handling
	}
	memset(text, 0, sizeof(char) * text_size);

	col++;

	int dividend = col;
	int i;
	int modulo;
	while (dividend > 0) {
		modulo = (dividend - 1) % 26;
		i = 65 + modulo;
		//move all text a one place right
		for (int i = text_size - 1; i >= 0; i--) {
			text[i + 1] = text[i];
		}
		text[0] = (char)i;
		dividend = (int)((dividend - modulo) / 26);
	}
}

/**
 * Return X position of the cursor.
 */
int getCursorPositionX() {
	return g_cursor_x;
}

/**
 * Return Y position of the cursor.
 */
int getCursorPositionY() {
	return g_cursor_y;
}

/**
 * Check if sheet is available (i.e. allocated) so it is safe to use sheet-releated functions.
 */
int isSheetAvailable() {
	return g_cells != NULL;
}

/**
 * return X coord of the viewport.
 */
int getViewX() {
	return g_view_x;
}

/**
 * return Y coord of the viewport.
 */
int getViewY() {
	return g_view_y;
}

/**
 * Set position of the viewport to the X,Y position.
**/
void setViewPosition(int x, int y) {
	g_view_x = x;
	g_view_y = y;
}

/**
 * dump sheet and some other info to the stdout.
**/
void sheetDebugDump() {
	printf("Spreadsheet debug dump\n");
	if (g_cells == NULL) {
		printf("No sheet allocated.\n");
		return;
	}

	printf("Sheet meta data:\n\tColumns: %i\n\tRows: %i\n\tMemory allocated: %li Kbytes.\n",
		   g_cols_count, g_row_count, g_cols_count * g_row_count * sizeof(Cell) / 1024);

	for (int x = 0; x < g_cols_count; x++) {
		for (int y = 0; y < g_row_count; y++) {
			int index = x + (y * g_cols_count);
			Cell *cell = g_cells[index];
			if (cell != NULL && cell->text != NULL) {
				char letterCol[11];
				memset(letterCol, '\0', sizeof(char) * 11);
				sheetTranslateColToLetter(x, 11, letterCol);
				printf("[%4i][%4i]\t(%i%s): %s\n", x, y, x, letterCol, cell != NULL ? cell->text : "(null)");
			}
		}
	}
}