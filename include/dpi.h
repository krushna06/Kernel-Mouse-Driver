#ifndef DPI_H
#define DPI_H

#define DPI_MIN 400
#define DPI_MAX 3200
#define DPI_STEP 200

extern int currentDPI;
void UpdateDpiLabel(HWND hwndLabel);

#endif
