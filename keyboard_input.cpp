#include "keyboard_input.h"
#include <memory>

bool IsKeyPressed(int virtualKey) {
    return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}

bool IsKeyPressedFast(int virtualKey) {
    return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}

KeyboardManager& KeyboardManager::GetInstance() {
    static KeyboardManager instance;
    return instance;
}

KeyboardManager::~KeyboardManager() {
    if (rawInputDevice) {
    }
}

bool KeyboardManager::InitializeRawInput(HWND hwnd) {
    RAWINPUTDEVICE rid[1];
    rid[0].usUsagePage = 0x01;
    rid[0].usUsage = 0x06;
    rid[0].dwFlags = RIDEV_INPUTSINK;
    rid[0].hwndTarget = hwnd;

    if (!RegisterRawInputDevices(rid, 1, sizeof(rid[0]))) {
        return false;
    }

    ClearKeyStates();
    return true;
}

bool KeyboardManager::IsKeyPressedOptimized(int virtualKey) {
    if (virtualKey < 0 || virtualKey >= KEY_CACHE_SIZE) {
        return false;
    }
    return keyStates[virtualKey].load(std::memory_order_relaxed);
}

void KeyboardManager::ProcessRawInputMessage(LPARAM lParam) {
    UINT dwSize = 0;
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

    if (dwSize == 0) return;

    std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(dwSize);
    if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawData.get(), &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {
        return;
    }

    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());

    if (raw->header.dwType == RIM_TYPEKEYBOARD) {
        const RAWKEYBOARD& kb = raw->data.keyboard;

        USHORT vKey = kb.VKey;
        if (vKey < KEY_CACHE_SIZE) {
            bool isPressed = (kb.Flags & RI_KEY_BREAK) == 0;
            keyStates[vKey].store(isPressed, std::memory_order_relaxed);
        }
    }
}

void KeyboardManager::ClearKeyStates() {
    for (int i = 0; i < KEY_CACHE_SIZE; ++i) {
        keyStates[i].store(false, std::memory_order_relaxed);
    }
}