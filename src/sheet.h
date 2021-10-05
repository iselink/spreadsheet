#ifndef INCLUDE_SHEET
#define INCLUDE_SHEET


typedef struct Cell_ {
	const char *text;
	int textLenght;
} Cell;

int initiateSheet(int rows, int cols);
void disposeSheet();

void setCursor(int row, int col);
int setCellValue(const char *text);
const char *getCellValue();
const char *getCellValueAt(int x, int y);
int isCursorPositionValid();
void sheetTranslateColToLetter(int col, int text_size, char *text);
int getCursorPositionX();
int getCursorPositionY();
int isSheetAvailable();

int getViewX();
int getViewY();
void setViewPosition(int x, int y);

//debug dump of sheet to the stdout
//so I can see what is program doing at all
void sheetDebugDump();

#endif // !INCLUDE_SHEET