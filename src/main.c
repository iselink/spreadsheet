#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "memory.h"
#include "sheet.h"
#include "ui.h"

void createWelcomeSheet() {
	//initiate welcome sheet
	initiateSheet(64, 64);
	setCursor(1, 1);
	char *welcomeBanner = "Welcome to the Iselink's spreadsheet software!";
	int len = strlen(welcomeBanner);

	//TODO: replace fixed cell size (like everywhere)
	//TODO: good idea is create text-fill function for longer texts (something like this ↓)
	for (int copChars = 0, index = 0; copChars < len; copChars += 16) {
		setCellValue(welcomeBanner + (index++ * 16));
		setCursor(1, getCursorPositionX() + 1);
	}

	setCursor(2, 0);
}

int main(int argc, char **argv) {
	char *filename = NULL;

	setlocale(LC_ALL, NULL);

	//parse arguments, if passed
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			if (argv[i][0] != '-') {
				filename = argv[i];
			}
		}
	}

	if (filename == NULL) {
		//no file to open, create standard sheet
		createWelcomeSheet();
	} else {
		//open file
		if (loadFile(filename)) {
			fprintf(stderr, "While opening file %s error occured and program can't contiue. :(\n", filename);
			exit(1);
		}
	}

	uiInitiate();
	uiRefreshScreen();
	while (!uiShouldClose()) {
		uiRefreshScreen();
		uiHandleInput();
	}

	uiDispose();

	disposeSheet();
}