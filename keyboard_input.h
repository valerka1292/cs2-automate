#pragma once
#include <windows.h>
#include <atomic>

bool IsKeyPressed(int virtualKey);

bool IsKeyPressedFast(int virtualKey);

const int KEY_SHOOT = VK_LBUTTON;
const int KEY_ZOOM = VK_LMENU;
const int KEY_PAUSE = VK_F8;
const int KEY_CYCLE_WEAPON = VK_F9;

const int KEY_EXIT = VK_F12;
const int KEY_OFF = VK_F11;
const int KEY_TOGGLE_COUNTERSTRAFE = VK_F6;

class KeyboardManager {
public:
    static KeyboardManager& GetInstance();

    bool InitializeRawInput(HWND hwnd);

    bool IsKeyPressedOptimized(int virtualKey);

    void ProcessRawInputMessage(LPARAM lParam);

    void ClearKeyStates();

private:
    KeyboardManager() = default;
    ~KeyboardManager();

    KeyboardManager(const KeyboardManager&) = delete;
    KeyboardManager& operator=(const KeyboardManager&) = delete;

    static constexpr int KEY_CACHE_SIZE = 256;
    std::atomic<bool> keyStates[KEY_CACHE_SIZE];

    HANDLE rawInputDevice = nullptr;
};
