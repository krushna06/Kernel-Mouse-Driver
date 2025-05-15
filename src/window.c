#include <windows.h>
#include <tchar.h>
#include "../include/resource.h"
#include "../include/dpi.h"
#include "../include/window.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hDpiLabel, hBtnPlus, hBtnMinus, hBtnApply;

    switch (msg) {
    case WM_CREATE:
        hDpiLabel = CreateWindow(TEXT("STATIC"), TEXT(""),
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            300, 40, 160, 40,
            hwnd, (HMENU)IDC_DPI_LABEL, NULL, NULL);
        SetWindowText(hDpiLabel, TEXT("Current DPI: 800"));
        SetWindowPos(hDpiLabel, NULL, 300, 40, 160, 40, SWP_NOZORDER);

        hBtnPlus = CreateWindow(TEXT("BUTTON"), TEXT("DPI +"),
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            300, 90, 75, 30,
            hwnd, (HMENU)IDC_DPI_PLUS, NULL, NULL);

        hBtnMinus = CreateWindow(TEXT("BUTTON"), TEXT("DPI -"),
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            385, 90, 75, 30,
            hwnd, (HMENU)IDC_DPI_MINUS, NULL, NULL);

        hBtnApply = CreateWindow(TEXT("BUTTON"), TEXT("Apply"),
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            340, 135, 75, 30,
            hwnd, (HMENU)IDC_APPLY, NULL, NULL);

        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_DPI_PLUS:
            if (currentDPI + DPI_STEP <= DPI_MAX) {
                currentDPI += DPI_STEP;
                UpdateDpiLabel(hDpiLabel);
            }
            break;
        case IDC_DPI_MINUS:
            if (currentDPI - DPI_STEP >= DPI_MIN) {
                currentDPI -= DPI_STEP;
                UpdateDpiLabel(hDpiLabel);
            }
            break;
        case IDC_APPLY: {
            int mouseSpeed = 1 + (currentDPI - DPI_MIN) * 19 / (DPI_MAX - DPI_MIN);
            BOOL success = SystemParametersInfo(SPI_SETMOUSESPEED, 0, (PVOID)(UINT_PTR)mouseSpeed,
                SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
            if (success) {
                TCHAR msg[100];
                wsprintf(msg, TEXT("Mouse speed set to %d (from DPI %d)"), mouseSpeed, currentDPI);
                MessageBox(hwnd, msg, TEXT("DPI Applied"), MB_OK | MB_ICONINFORMATION);
            }
            else {
                MessageBox(hwnd, TEXT("Failed to set mouse speed"), TEXT("Error"), MB_OK | MB_ICONERROR);
            }
            break;
        }
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);
        HBRUSH hBrush = CreateSolidBrush(RGB(32, 32, 32));
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);

        RECT mouseRect = { 20, 20, 260, 160 };
        HBRUSH grayBrush = CreateSolidBrush(RGB(50, 50, 50));
        FillRect(hdc, &mouseRect, grayBrush);
        DeleteObject(grayBrush);

        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        RoundRect(hdc, mouseRect.left + 20, mouseRect.top + 10, mouseRect.right - 20, mouseRect.bottom - 10, 60, 100);

        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);

        SetTextColor(hdc, RGB(180, 180, 180));
        SetBkMode(hdc, TRANSPARENT);
        DrawText(hdc, TEXT("My Mouse Image to be placed\nPlaceholder"), -1, &mouseRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
