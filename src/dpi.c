#include <windows.h>
#include "../include/dpi.h"

int currentDPI = 800;

void UpdateDpiLabel(HWND hwndLabel) {
    TCHAR text[50];
    wsprintf(text, TEXT("Current DPI: %d"), currentDPI);
    SetWindowText(hwndLabel, text);
}
