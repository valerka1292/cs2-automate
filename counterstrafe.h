#pragma once
#include <atomic>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <windows.h>

class CounterStrafeController {
public:
    CounterStrafeController();
    ~CounterStrafeController() = default;

    void SetActive(bool active);
    bool IsActive() const { return isActive.load(); }

    void OnKeyPress(char key);
    void OnKeyRelease(char key);

private:
    static constexpr DWORD MIN_HOLD_TIME_MS = 200;
    static constexpr DWORD MIN_HOLD_DURATION_MS = 100;
    static constexpr DWORD MAX_HOLD_DURATION_MS = 125;
    static constexpr DWORD MIN_PRE_DELAY_MS = 16;
    static constexpr DWORD MAX_PRE_DELAY_MS = 30;

    std::atomic<bool> isActive;
    std::atomic<bool> counterStrafeRunning;
    std::unordered_map<char, DWORD> keyPressTimes;

    void PerformCounterStrafe(char originalKey, char oppositeKey);

    bool IsKeyPressedByCode(int vk) const;

    DWORD GenerateRandomDelayMs();
    DWORD GenerateRandomHoldDurationMs();
};
