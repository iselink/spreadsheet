#ifndef INCLUDED_UI
#define INCLUDED_UI

//colors!
#define UI_COLOR_DEFAULT 0
#define UI_COLOR_HEADER 1
#define UI_COLOR_SHEET 2
#define UI_COLOR_SHEET_FOCUS 3

//ui focus elements
#define UI_FOCUS_NONE 0
#define UI_FOCUS_SHEET 1
#define UI_FOCUS_CELL_POSITION 2
#define UI_FOCUS_CELL_VALUE 3

#define UI_DATA_FIELD_SIZE 256

void uiInitiate();
void uiDispose();

void uiHandleInput();
int uiShouldClose();
void uiRefreshScreen();

#endif	// !INCLUDED_UI