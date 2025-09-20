#include <string>
#include <windows.h>
#include <thread>
#include <chrono>
#include "keyboard_input.h"
#include "weapon_recoil.h"
#include "counterstrafe.h"

bool g_paused = false;
WeaponType g_currentWeapon = WeaponType::AK47;
bool g_scriptActive = true;
CounterStrafeController g_counterStrafeController;
WeaponRecoilController g_weaponController(3.1f, 1.0f);

struct KeyState {
    bool wasPressed;
    std::chrono::steady_clock::time_point lastPressTime;
    static constexpr auto DEBOUNCE_DELAY = std::chrono::milliseconds(50);

    KeyState() : wasPressed(false), lastPressTime(std::chrono::steady_clock::now() - DEBOUNCE_DELAY) {}

    bool IsDebounced(bool currentState) {
        auto now = std::chrono::steady_clock::now();
        if (currentState && !wasPressed && (now - lastPressTime) >= DEBOUNCE_DELAY) {
            wasPressed = true;
            lastPressTime = now;
            return true;
        }
        if (!currentState) {
            wasPressed = false;
        }
        return false;
    }
};

float ParseSensitivity(int argc, char* argv[]) {
    float defaultSens = 3.10f;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--sens" && i + 1 < argc) {
            try {
                float sens = std::stof(argv[i + 1]);
                if (sens >= 0.1f && sens <= 10.0f) {
                    return sens;
                }
            } catch (const std::exception&) {
            }
        }
    }

    return defaultSens;
}

WeaponType CycleWeapon(WeaponType current) {
    switch (current) {
        case WeaponType::NONE: return WeaponType::AK47;
        case WeaponType::AK47: return WeaponType::M4A1;
        case WeaponType::M4A1: return WeaponType::M4A4;
        case WeaponType::M4A4: return WeaponType::FAMAS;
        case WeaponType::FAMAS: return WeaponType::GALIL;
        case WeaponType::GALIL: return WeaponType::UMP45;
        case WeaponType::UMP45: return WeaponType::AUG;
        case WeaponType::AUG: return WeaponType::SG553;
        case WeaponType::SG553: return WeaponType::AK47;
        default: return WeaponType::AK47;
    }
}

LRESULT CALLBACK OverlayWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rect;
            GetClientRect(hwnd, &rect);
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);

            HFONT hFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkMode(hdc, TRANSPARENT);

            const char* scriptStatus = g_paused ? "Script not active" : "Script active";
            std::string weaponText = "Weapon: " + GetWeaponName(g_currentWeapon);
            std::string counterStrafeStatus = "CounterStrafe: " + std::string(g_counterStrafeController.IsActive() ? "ON" : "OFF");

            TextOutA(hdc, 10, 8, scriptStatus, lstrlenA(scriptStatus));
            TextOutA(hdc, 10, 28, weaponText.c_str(), static_cast<int>(weaponText.length()));
            TextOutA(hdc, 10, 48, counterStrafeStatus.c_str(), static_cast<int>(counterStrafeStatus.length()));

            SelectObject(hdc, hOldFont);
            DeleteObject(hFont);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void RunGUIOverlay() {
    const char* WINDOW_CLASS_NAME = "OverlayWindowClass";
    const int WINDOW_WIDTH = 200;
    const int WINDOW_HEIGHT = 100;

    WNDCLASSEXA wc = {};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = OverlayWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if (!RegisterClassExA(&wc)) {
        return;
    }

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    HWND hwnd = CreateWindowExA(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        WINDOW_CLASS_NAME,
        "CS2 Overlay",
        WS_POPUP,
        screenWidth - WINDOW_WIDTH - 20,
        screenHeight - WINDOW_HEIGHT - 100,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    if (!hwnd) {
        return;
    }

    SetLayeredWindowAttributes(hwnd, 0, 180, LWA_ALPHA);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (g_scriptActive) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        InvalidateRect(hwnd, NULL, TRUE);
        Sleep(100);
    }

    DestroyWindow(hwnd);
}


int main(int argc, char* argv[]) {
    float sensitivity = ParseSensitivity(argc, argv);
    float zoomSensitivity = 1.0f;

    g_weaponController.SetSensitivity(sensitivity);
    g_weaponController.SetZoomSensitivity(zoomSensitivity);

    g_paused = false;
    g_currentWeapon = WeaponType::NONE;
    g_scriptActive = true;

    std::thread guiThread(RunGUIOverlay);
    guiThread.detach();

    KeyState pauseKeyState;
    KeyState cycleKeyState;
    KeyState offKeyState;
    KeyState counterStrafeKeyState;

    while (g_scriptActive) {
        if (IsKeyPressed(KEY_EXIT)) {
            g_scriptActive = false;
            if (g_weaponController.IsActive()) {
                g_weaponController.StopRecoilPattern();
            }
            break;
        }

        if (pauseKeyState.IsDebounced(IsKeyPressedFast(KEY_PAUSE))) {
            g_paused = !g_paused;
            if (g_paused && g_weaponController.IsActive()) {
                g_weaponController.StopRecoilPattern();
            }
        }

        if (!g_paused) {
            if (cycleKeyState.IsDebounced(IsKeyPressedFast(KEY_CYCLE_WEAPON))) {
                g_currentWeapon = CycleWeapon(g_currentWeapon);
                g_weaponController.SetWeapon(g_currentWeapon);
            }

            if (offKeyState.IsDebounced(IsKeyPressedFast(KEY_OFF))) {
                g_currentWeapon = WeaponType::NONE;
                if (g_weaponController.IsActive()) {
                    g_weaponController.StopRecoilPattern();
                }
            }

            if (counterStrafeKeyState.IsDebounced(IsKeyPressedFast(KEY_TOGGLE_COUNTERSTRAFE))) {
                g_counterStrafeController.SetActive(!g_counterStrafeController.IsActive());
            }


            static bool wasdPressed[4] = {false, false, false, false};
            bool currentWASD[4] = {
                IsKeyPressedFast(0x57),
                IsKeyPressedFast(0x41),
                IsKeyPressedFast(0x53),
                IsKeyPressedFast(0x44)
            };

            for (int i = 0; i < 4; ++i) {
                char key = "WASD"[i];
                if (currentWASD[i] && !wasdPressed[i]) {
                    g_counterStrafeController.OnKeyPress(key);
                } else if (!currentWASD[i] && wasdPressed[i]) {
                    g_counterStrafeController.OnKeyRelease(key);
                }
                wasdPressed[i] = currentWASD[i];
            }

            bool isShooting = IsKeyPressedFast(KEY_SHOOT) && g_currentWeapon != WeaponType::NONE;
            bool isActive = g_weaponController.IsActive();

            if (isShooting && !isActive) {
                g_weaponController.StartRecoilPattern();
            } else if (!isShooting && isActive) {
                g_weaponController.StopRecoilPattern();
            }
        }
    }

    return 0;
}