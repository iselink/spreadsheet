#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sheet.h"

int calcColCount(char *);

FILE *g_file_src = NULL;

///////////////////////////////////////////////////////

/**
 * Load and parse CSV file into memory.
**/
int loadFile(const char *filename) {
	g_file_src = fopen(filename, "r");
	if (g_file_src == NULL) {
		return 1;
	}

	//count how many cols this file have
	//TODO: this is more like life hack rather proper implementation ↓
	//TODO: \" ← support
	char line[256];
	memset(line, '\0', sizeof(char) * 256);
	int c;
	int index = 0;
	while ((c = fgetc(g_file_src)) != EOF) {
		if (c == '\n') {
			line[index] = '\0';
			break;
		}
		line[index++] = (char)c;
	}

	int colCount = calcColCount(line);

	//1 line for sur
	rewind(g_file_src);
	//calculate, how many rows this file have
	int rowCount = 1;
	while ((c = fgetc(g_file_src)) != EOF) {
		if (c == '\n') {
			rowCount++;
		}
	}

	if (initiateSheet(rowCount, colCount)) {
		fclose(g_file_src);
		g_file_src = NULL;
		return 1;
	} else {
		//cursor is required to set somewhere
		//otherwise program will crash (this is random fix lol)
		setCursor(0, 0);
	}

	//last YT rewind
	rewind(g_file_src);

	//read file until end
	int cellRow = 0;
	int cellCol = 0;
	int buffIndex = 0;
	memset(line, 0, sizeof(char) * 256);
	while ((c = fgetc(g_file_src)) != EOF) {
		switch (c) {
			case ',': {
				setCursor(cellRow, cellCol);
				setCellValue(line);
				memset(line, 0, sizeof(char) * 256);
				buffIndex = 0;
				cellCol++;
				break;
			}
			case '\n': {
				setCursor(cellRow, cellCol);
				setCellValue(line);
				memset(line, 0, sizeof(char) * 256);
				buffIndex = 0;
				cellCol = 0;
				cellRow++;
				break;
			}

			default: {
				line[buffIndex++] = c;
				break;
			}
		}
	}

	fclose(g_file_src);
	g_file_src = NULL;
	return 0;
}

int calcColCount(char *line) {
	int counter = 1;  //one comma is missing (at the end of the line).
	for (char *c = line; *c != '\0'; c++) {
		if (*c == ',')
			counter++;
	}
	return counter;
}