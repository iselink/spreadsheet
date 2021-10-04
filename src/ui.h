#ifndef INCLUDED_UI
#define INCLUDED_UI

#define UI_COLOR_DEFAULT 0
#define UI_COLOR_HEADER 1
#define UI_COLOR_SHEET 2

void uiInitiate();
void uiDispose();

void uiHandleInput();
int uiShouldClose();
void uiRefreshScreen();


#endif	// !INCLUDED_UI