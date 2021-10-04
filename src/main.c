#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "sheet.h"
#include "ui.h"

int main(int argc, char **argv) {
	setlocale(LC_ALL, NULL);

	initiateSheet(64, 64);
	setCursor(0, 0);

	uiInitiate();
	uiRefreshScreen();
	while (!uiShouldClose()) {
		uiHandleInput();
		uiRefreshScreen();
	}

	uiDispose();

	disposeSheet();
}