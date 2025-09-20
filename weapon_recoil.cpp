#include "weapon_recoil.h"
#include "mouse_control.h"
#include "keyboard_input.h"
#include <thread>
#include <string>
#include <algorithm>
#include <cmath>

const std::vector<RecoilStep> WeaponRecoilController::AK47_PATTERN = {
    {-4, 7, 99}, {4, 19, 99}, {-3, 29, 99}, {-1, 31, 99},
    {13, 31, 99}, {8, 28, 99}, {13, 21, 99}, {-17, 12, 99},
    {-42, -3, 99}, {-21, 2, 99}, {12, 11, 99}, {-15, 7, 99},
    {-26, -8, 99}, {-3, 4, 99}, {40, 1, 99}, {19, 7, 99},
    {14, 10, 99}, {27, 0, 99}, {33, -10, 99}, {-21, -2, 99},
    {7, 3, 99}, {-7, 9, 99}, {-8, 4, 99}, {19, -3, 99},
    {5, 6, 99}, {-20, -1, 99}, {-33, -4, 99}, {-45, -21, 99},
    {-14, 1, 80}
};

const std::vector<RecoilStep> WeaponRecoilController::M4A1_PATTERN = {
    {1, 6, 88}, {0, 4, 88}, {-4, 14, 88}, {4, 18, 88},
    {-6, 21, 88}, {-4, 24, 88}, {14, 14, 88}, {8, 12, 88},
    {18, 5, 88}, {-4, 10, 88}, {-14, 5, 88}, {-25, -3, 88},
    {-19, 0, 88}, {-22, -3, 88}, {1, 3, 88}, {8, 3, 88},
    {-9, 1, 88}, {-13, -2, 88}, {3, 2, 88}, {1, 1, 88}
};

const std::vector<RecoilStep> WeaponRecoilController::M4A4_PATTERN = {
    {2, 7, 88}, {0, 9, 87}, {-6, 16, 87}, {7, 21, 87},
    {-9, 23, 87}, {-5, 27, 87}, {16, 15, 88}, {11, 13, 88},
    {22, 5, 88}, {-4, 11, 88}, {-18, 6, 88}, {-30, -4, 88},
    {-24, 0, 88}, {-25, -6, 88}, {0, 4, 87}, {8, 4, 87},
    {-11, 1, 87}, {-13, -2, 87}, {2, 2, 88}, {33, -1, 88},
    {10, 6, 88}, {27, 3, 88}, {10, 2, 88}, {11, 0, 88},
    {-12, 0, 87}, {6, 5, 87}, {4, 5, 87}, {3, 1, 87},
    {4, -1, 0}
};

const std::vector<RecoilStep> WeaponRecoilController::FAMAS_PATTERN = {
    {-4, 5, 88}, {1, 4, 88}, {-6, 10, 88}, {-1, 17, 88},
    {0, 20, 88}, {14, 18, 88}, {16, 12, 88}, {-6, 12, 88},
    {-20, 8, 88}, {-16, 5, 88}, {-13, 2, 88}, {4, 5, 88},
    {23, 4, 88}, {12, 6, 88}, {20, -3, 88}, {5, 0, 88},
    {15, 0, 88}, {3, 5, 80}, {-4, 3, 88}, {-25, -1, 80},
    {-3, 2, 84}, {11, 0, 80}, {15, -7, 88}, {15, -10, 88}
};

const std::vector<RecoilStep> WeaponRecoilController::GALIL_PATTERN = {
    {4, 4, 90}, {-2, 5, 90}, {6, 10, 90}, {12, 15, 90},
    {-1, 21, 90}, {2, 24, 90}, {6, 16, 90}, {11, 10, 90},
    {-4, 14, 90}, {-22, 8, 90}, {-30, -3, 90}, {-29, -13, 90},
    {-9, 8, 90}, {-12, 2, 90}, {-7, 1, 90}, {0, 1, 50},
    {4, 7, 90}, {25, 7, 90}, {14, 4, 90}, {25, -3, 90},
    {31, -9, 90}, {6, 3, 90}, {-12, 3, 90}, {13, -1, 90},
    {10, -1, 90}, {16, -4, 90}, {-9, 5, 90}, {-32, -5, 90},
    {-24, -3, 90}, {-15, 5, 90}, {6, 8, 90}, {-14, -3, 90},
    {-24, -14, 90}, {-13, -1, 90}
};

const std::vector<RecoilStep> WeaponRecoilController::UMP45_PATTERN = {
    {-1, 6, 90}, {-4, 8, 90}, {-2, 18, 90}, {-4, 23, 90},
    {-9, 23, 90}, {-3, 26, 90}, {11, 17, 90}, {-4, 12, 90},
    {9, 13, 90}, {18, 8, 90}, {15, 5, 90}, {-1, 3, 90},
    {5, 6, 90}, {0, 6, 90}, {9, -3, 90}, {5, -1, 90},
    {-12, 4, 90}, {-19, 1, 85}, {-1, -2, 90}, {15, -5, 90},
    {17, -2, 85}, {-6, 3, 90}, {-20, -2, 90}, {-3, -1, 90}
};

const std::vector<RecoilStep> WeaponRecoilController::AUG_PATTERN = {
    {5, 6, 89}, {0, 13, 89}, {-5, 22, 89}, {-7, 26, 88},
    {5, 29, 88}, {9, 30, 80}, {14, 21, 80}, {6, 15, 89},
    {14, 13, 88}, {-16, 11, 89}, {-5, 6, 89}, {13, 0, 88},
    {1, 6, 89}, {-22, 5, 88}, {-38, -11, 89}, {-31, -13, 89},
    {-3, 6, 88}, {-5, 5, 89}, {-9, 0, 88}, {24, 1, 89},
    {32, 3, 88}, {15, 6, 89}, {-5, 1, 88}
};

const std::vector<RecoilStep> WeaponRecoilController::SG553_PATTERN = {
    {-4, 9, 89}, {-13, 15, 89}, {-9, 25, 89}, {-6, 29, 88},
    {-8, 31, 88}, {-7, 36, 80}, {-20, 14, 80}, {14, 17, 89},
    {-8, 12, 88}, {-15, 8, 89}, {-5, 5, 89}, {6, 5, 88},
    {-8, 6, 89}, {2, 11, 88}, {-14, -6, 89}, {-20, -17, 89},
    {-18, -9, 88}, {-8, -2, 89}, {41, 3, 88}, {56, -5, 89},
    {43, -1, 88}, {18, 9, 89}, {14, 9, 88}, {6, 7, 89},
    {21, -3, 95}, {29, -4, 89}, {-6, 8, 89}, {-15, 5, 89},
    {-38, -5, 89}
};

std::string GetWeaponName(WeaponType weapon) {
    switch (weapon) {
        case WeaponType::AK47: return "AK-47";
        case WeaponType::M4A1: return "M4A1-S";
        case WeaponType::M4A4: return "M4A4";
        case WeaponType::FAMAS: return "FAMAS";
        case WeaponType::GALIL: return "Galil AR";
        case WeaponType::UMP45: return "UMP-45";
        case WeaponType::AUG: return "AUG";
        case WeaponType::SG553: return "SG 553";
        case WeaponType::NONE: return "None";
        default: return "Unknown";
    }
}

WeaponRecoilController::WeaponRecoilController(float sensitivity, float zoomSensitivity)
    : currentWeapon(WeaponType::NONE), initialSleep(50),
      precalculatedNormalModifier(float_to_fixed(1.0f)), precalculatedZoomModifier(float_to_fixed(1.0f)), precalculatedValuesValid(false),
      isActive(false), stopRequested(false) {
    SetSensitivity(sensitivity);
    SetZoomSensitivity(zoomSensitivity);
}

void WeaponRecoilController::SetWeapon(WeaponType weapon) {
    currentWeapon = weapon;
    InitializeRecoilPattern(weapon);
    PrecalculateModifiers();
}

void WeaponRecoilController::SetSensitivity(float sens) {
    modifier = float_to_fixed(2.52f / sens);
    PrecalculateModifiers();
}

void WeaponRecoilController::SetZoomSensitivity(float zoomSens) {
    zoomModifier = float_to_fixed(1.2f / zoomSens);
    PrecalculateModifiers();
}

void WeaponRecoilController::PrecalculateModifiers() {
    precalculatedNormalModifier = modifier;
    precalculatedZoomModifier = fixed_mul(modifier, zoomModifier);
    precalculatedValuesValid = true;
}

bool WeaponRecoilController::SupportsZoom() const {
    return currentWeapon == WeaponType::AUG || currentWeapon == WeaponType::SG553;
}

void WeaponRecoilController::InitializeRecoilPattern(WeaponType weapon) {
    switch (weapon) {
        case WeaponType::AK47:
            recoilPattern.assign(AK47_PATTERN.begin(), AK47_PATTERN.end());
            initialSleep = 50;
            break;
        case WeaponType::M4A1:
            recoilPattern.assign(M4A1_PATTERN.begin(), M4A1_PATTERN.end());
            initialSleep = 15;
            break;
        case WeaponType::M4A4:
            recoilPattern.assign(M4A4_PATTERN.begin(), M4A4_PATTERN.end());
            initialSleep = 15;
            break;
        case WeaponType::FAMAS:
            recoilPattern.assign(FAMAS_PATTERN.begin(), FAMAS_PATTERN.end());
            initialSleep = 30;
            break;
        case WeaponType::GALIL:
            recoilPattern.assign(GALIL_PATTERN.begin(), GALIL_PATTERN.end());
            initialSleep = 10;
            break;
        case WeaponType::UMP45:
            recoilPattern.assign(UMP45_PATTERN.begin(), UMP45_PATTERN.end());
            initialSleep = 15;
            break;
        case WeaponType::AUG:
            recoilPattern.assign(AUG_PATTERN.begin(), AUG_PATTERN.end());
            initialSleep = 30;
            break;
        case WeaponType::SG553:
            recoilPattern.assign(SG553_PATTERN.begin(), SG553_PATTERN.end());
            initialSleep = 30;
            break;
        default:
            recoilPattern.clear();
            break;
    }
}

void WeaponRecoilController::StartRecoilPattern() {
    if (isActive.load() || currentWeapon == WeaponType::NONE) return;

    stopRequested = false;
    isActive = true;

    std::thread([this]() {
        SendInputLeftMouseDown();

        if (initialSleep > 0) Sleep(initialSleep);

        const size_t patternSize = recoilPattern.size();
        bool isZoomSupported = SupportsZoom();

        for (size_t i = 0; i < patternSize; ++i) {
            const auto& step = recoilPattern[i];

            if (stopRequested.load(std::memory_order_relaxed) ||
                !isActive.load(std::memory_order_relaxed) ||
                !IsKeyPressed(KEY_SHOOT)) {
                SendInputLeftMouseUp();
                break;
            }

            fixed_point currentModifier = (isZoomSupported && IsKeyPressed(KEY_ZOOM)) ?
                                        precalculatedZoomModifier : precalculatedNormalModifier;

            fixed_point deltaX_fixed = float_to_fixed(static_cast<float>(step.deltaX));
            fixed_point deltaY_fixed = float_to_fixed(static_cast<float>(step.deltaY));

            int adjustedX = fixed_mul(deltaX_fixed, currentModifier) >> 8;
            int adjustedY = fixed_mul(deltaY_fixed, currentModifier) >> 8;

            if (currentWeapon == WeaponType::SG553) {
                SendInputFastMouseMove(adjustedX, adjustedY);
            } else {
                SendInputMouseMove(adjustedX, adjustedY);
            }

            if (step.sleepMs > 0) Sleep(step.sleepMs);
        }

        if (isActive.load(std::memory_order_relaxed) && !stopRequested.load(std::memory_order_relaxed)) {
            SendInputLeftMouseUp();
        }

        isActive.store(false, std::memory_order_relaxed);
        stopRequested.store(false, std::memory_order_relaxed);
    }).detach();
}

void WeaponRecoilController::StopRecoilPattern() {
    stopRequested = true;
    isActive = false;
    SendInputLeftMouseUp();
}
