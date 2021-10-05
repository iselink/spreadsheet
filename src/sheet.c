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

void initiateSheet(int rows, int cols) {
	g_row_count = rows;
	g_cols_count = cols;
	g_cells = (Cell **)malloc(sizeof(Cell *) * cols * rows);
	//TODO: error memory allocation handle
	memset(g_cells, 0, sizeof(Cell *) * cols * rows);
}

void disposeSheet() {
	if (g_cells == NULL) {
		return;
	}

	g_cursor_x = -1;
	g_cursor_y = -1;

	for (int i = 0; i < g_cols_count * g_row_count; i++) {
		if (g_cells[i] == NULL)
			continue;
		//TODO: dealocate cells
		if (g_cells[i]->text != NULL) {
			free((void *) g_cells[i]->text);
		}
		free(g_cells[i]);
	}

	free(g_cells);
	g_cells = NULL;
}

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
void setCellValue(const char *text) {
	if (!isCursorPositionValid())
		return;

	int index = g_cursor_x + (g_cursor_y * g_cols_count);
	//TODO: boundary check
	if (g_cells[index] == NULL) {
		g_cells[index] = (Cell *)malloc(sizeof(Cell));
		int textLen = strlen(text);
		g_cells[index]->text = (const char *)malloc(sizeof(const char) * textLen);
		//TODO: memory allocation check
		g_cells[index]->textLenght = textLen;
	}

	strcpy((char *) g_cells[index]->text, text);
}
const char *getCellValue() {
	if (!isCursorPositionValid()) {
		return NULL;
	}

	int index = g_cursor_x + (g_cursor_y * g_cols_count);
	return g_cells[index] != NULL ? g_cells[index]->text : NULL;
}

int isCursorPositionValid() {
	return g_cursor_x >= 0 && g_cursor_y >= 0 && g_cursor_x <= g_cols_count && g_cursor_y <= g_row_count;
}

void sheetTranslateColToLetter(int col, int text_size, char *text) {
	if (col < 0 || text_size <= 0 || text == NULL) {
		return;	 //TODO: error check for parameters and so on...
	}

	//it is so fucking late and my brain is dead inside
	//TODO: do this better before it hurts me lol
	text[0] = 'A';
	for (int counter = 0; counter < col; counter++) {
		for (int i = 0; i < text_size; i++) {
			if (text[i] >= 'A')
				text[i]++;
			if (text[i] > 'Z') {
				text[i] = 'A';
				if (text[i + 1] == 0) {
					text[i + 1] = 'A';
				} else {
					text[i + 1]++;
				}
			}
		}
	}
}

int getCursorPositionX() {
	return g_cursor_x;
}

int getCursorPositionY() {
	return g_cursor_y;
}

int isSheetAvailable() {
	return g_cells != NULL;
}

int getViewX() {
	return g_view_x;
}

int getViewY() {
	return g_view_y;
}

void setViewPosition(int x, int y) {
	g_view_x = x;
	g_view_y = y;
}

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
			char letterCol[11];
			memset(letterCol, '\0', sizeof(char) * 11);
			sheetTranslateColToLetter(x, 11, letterCol);
			printf("[%4i][%4i]\t(%i%s): %s\n", x, y, x, letterCol, cell != NULL ? cell->text : "(null)");
		}
	}
}