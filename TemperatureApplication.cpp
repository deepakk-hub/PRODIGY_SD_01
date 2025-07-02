#include <windows.h>
#include <string>
#include <sstream>

#define ID_INPUT 1
#define ID_COMBO 2
#define ID_BUTTON 3
#define ID_OUTPUT 4

const wchar_t g_szClassName[] = L"TempConverterClass";

// Conversion functions
std::wstring ConvertTemperature(const std::wstring& inputText, int selectedIndex) {
    try {
        double input = std::stod(inputText);
        double result = 0.0;
        std::wstring label;

        switch (selectedIndex) {
        case 0: // C to F
            result = (input * 9 / 5) + 32;
            label = L"°F";
            break;
        case 1: // F to C
            result = (input - 32) * 5 / 9;
            label = L"°C";
            break;
        case 2: // C to K
            result = input + 273.15;
            label = L"K";
            break;
        case 3: // K to C
            result = input - 273.15;
            label = L"°C";
            break;
        case 4: // F to K
            result = ((input - 32) * 5 / 9) + 273.15;
            label = L"K";
            break;
        case 5: // K to F
            result = ((input - 273.15) * 9 / 5) + 32;
            label = L"°F";
            break;
        }

        std::wostringstream ss;
        ss.precision(2);
        ss << std::fixed << result << L" " << label;
        return ss.str();
    }
    catch (...) {
        return L"Invalid input";
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hInput, hCombo, hButton, hOutput;

    switch (msg) {
    case WM_CREATE:
        hInput = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
            20, 20, 200, 25, hwnd, (HMENU)ID_INPUT, NULL, NULL);

        hCombo = CreateWindowW(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
            20, 60, 200, 100, hwnd, (HMENU)ID_COMBO, NULL, NULL);

        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Celsius to Fahrenheit");
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Fahrenheit to Celsius");
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Celsius to Kelvin");
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Kelvin to Celsius");
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Fahrenheit to Kelvin");
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Kelvin to Fahrenheit");
        SendMessageW(hCombo, CB_SETCURSEL, 0, 0);

        hButton = CreateWindowW(L"BUTTON", L"Convert", WS_CHILD | WS_VISIBLE,
            20, 100, 100, 30, hwnd, (HMENU)ID_BUTTON, NULL, NULL);

        hOutput = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE,
            20, 140, 250, 25, hwnd, (HMENU)ID_OUTPUT, NULL, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUTTON) {
            wchar_t inputText[100];
            GetWindowTextW(hInput, inputText, 100);
            int selectedIndex = SendMessageW(hCombo, CB_GETCURSEL, 0, 0);
            std::wstring result = ConvertTemperature(inputText, selectedIndex);
            SetWindowTextW(hOutput, result.c_str());
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = g_szClassName;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(g_szClassName, L"Temperature Converter",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        320, 250, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
