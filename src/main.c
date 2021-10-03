#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "memory.h"
#include "sheet.h"

int main(int argc, char **argv) {
	//initiateSheet(4096, 4096);
	initiateSheet(1, 40);
	setCursor(0, 0);
	setCellValue("A1");
	sheetDebugDump();
	disposeSheet();
}