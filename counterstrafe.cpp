#include "counterstrafe.h"
#include "keyboard_input.h"
#include "mouse_control.h"
#include <windows.h>
#include <random>
#include <thread>
#include <unordered_map>

CounterStrafeController::CounterStrafeController()
    : isActive(true), counterStrafeRunning(false) {
}

void CounterStrafeController::SetActive(bool active) {
    isActive.store(active, std::memory_order_relaxed);
}

void CounterStrafeController::OnKeyPress(char key) {
    char lowerKey = tolower(key);
    if (lowerKey == 'w' || lowerKey == 'a' || lowerKey == 's' || lowerKey == 'd') {
        keyPressTimes[lowerKey] = GetTickCount();
    }
}

void CounterStrafeController::OnKeyRelease(char key) {
    char lowerKey = tolower(key);
    if (lowerKey != 'w' && lowerKey != 'a' && lowerKey != 's' && lowerKey != 'd') {
        return;
    }

    auto it = keyPressTimes.find(lowerKey);
    if (it == keyPressTimes.end()) {
        return;
    }

    DWORD startTime = it->second;
    keyPressTimes.erase(it);

    DWORD durationMs = GetTickCount() - startTime;

    if (!isActive.load() || durationMs <= MIN_HOLD_TIME_MS) {
        return;
    }

    char oppositeKey;
    switch (lowerKey) {
        case 'w': oppositeKey = 's'; break;
        case 'a': oppositeKey = 'd'; break;
        case 's': oppositeKey = 'w'; break;
        case 'd': oppositeKey = 'a'; break;
        default: return;
    }

    int oppositeVk = toupper(oppositeKey);
    if (IsKeyPressedByCode(oppositeVk)) {
        return;
    }

    std::thread([this, lowerKey, oppositeKey]() {
        PerformCounterStrafe(lowerKey, oppositeKey);
    }).detach();
}

void CounterStrafeController::PerformCounterStrafe(char originalKey, char oppositeKey) {
    int originalVk = toupper(originalKey);
    int oppositeVk = toupper(oppositeKey);

    if (IsKeyPressedByCode(oppositeVk) || IsKeyPressedByCode(originalVk)) {
        return;
    }

    DWORD delayBeforePress = GenerateRandomDelayMs();
    DWORD startDelay = GetTickCount();
    DWORD endDelay = startDelay + delayBeforePress;

    while (GetTickCount() < endDelay) {
        Sleep(0);
        if (!isActive.load() || IsKeyPressedByCode(oppositeVk) || IsKeyPressedByCode(originalVk)) {
            return;
        }
    }

    if (!isActive.load()) {
        return;
    }

    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = oppositeVk;
    input.ki.dwFlags = 0;
    input.ki.wScan = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.time = 0;
    SendInput(1, &input, sizeof(INPUT));

    DWORD holdDuration = GenerateRandomHoldDurationMs();
    DWORD startHold = GetTickCount();
    DWORD endHold = startHold + holdDuration;

    while (GetTickCount() < endHold) {
        Sleep(0);
        if (!isActive.load() || IsKeyPressedByCode(originalVk)) {
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            input.ki.wScan = 0;
            input.ki.dwExtraInfo = 0;
            input.ki.time = 0;
            SendInput(1, &input, sizeof(INPUT));
            return;
        }
    }

    input.ki.dwFlags = KEYEVENTF_KEYUP;
    input.ki.wScan = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.time = 0;
    SendInput(1, &input, sizeof(INPUT));
}

bool CounterStrafeController::IsKeyPressedByCode(int vk) const {
    return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

DWORD CounterStrafeController::GenerateRandomDelayMs() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<DWORD> dis(MIN_PRE_DELAY_MS, MAX_PRE_DELAY_MS);
    return dis(gen);
}

DWORD CounterStrafeController::GenerateRandomHoldDurationMs() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<DWORD> dis(MIN_HOLD_DURATION_MS, MAX_HOLD_DURATION_MS);
    return dis(gen);
}
