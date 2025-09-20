#pragma once
#include <array>
#include <vector>
#include <string>
#include <atomic>
#include <thread>

using fixed_point = int32_t;
const int FIXED_POINT_SCALE = 256;

inline fixed_point float_to_fixed(float val) { return static_cast<fixed_point>(val * FIXED_POINT_SCALE); }
inline float fixed_to_float(fixed_point val) { return static_cast<float>(val) / FIXED_POINT_SCALE; }

inline fixed_point fixed_mul(fixed_point a, fixed_point b) { return (a * b) >> 8; }
inline fixed_point fixed_div(fixed_point a, fixed_point b) { return (a << 8) / b; }

struct RecoilStep {
    int deltaX;
    int deltaY;
    int sleepMs;
};

enum class WeaponType {
    NONE,
    AK47,
    M4A1,
    M4A4,
    FAMAS,
    GALIL,
    UMP45,
    AUG,
    SG553
};

std::string GetWeaponName(WeaponType weapon);

class WeaponRecoilController {
public:
    WeaponRecoilController(float sensitivity, float zoomSensitivity = 1.0f);
    ~WeaponRecoilController() = default;

    void SetWeapon(WeaponType weapon);

    void StartRecoilPattern();

    void StopRecoilPattern();

    bool IsActive() const { return isActive.load(); }

    void SetSensitivity(float sens);

    void SetZoomSensitivity(float zoomSens);

    bool SupportsZoom() const;

    WeaponType GetCurrentWeapon() const { return currentWeapon; }


private:
    std::vector<RecoilStep> recoilPattern;
    WeaponType currentWeapon;

    fixed_point modifier;
    fixed_point zoomModifier;
    int initialSleep;

    fixed_point precalculatedNormalModifier;
    fixed_point precalculatedZoomModifier;
    bool precalculatedValuesValid;

    std::atomic<bool> isActive;
    std::atomic<bool> stopRequested;

    static const std::vector<RecoilStep> AK47_PATTERN;
    static const std::vector<RecoilStep> M4A1_PATTERN;
    static const std::vector<RecoilStep> M4A4_PATTERN;
    static const std::vector<RecoilStep> FAMAS_PATTERN;
    static const std::vector<RecoilStep> GALIL_PATTERN;
    static const std::vector<RecoilStep> UMP45_PATTERN;
    static const std::vector<RecoilStep> AUG_PATTERN;
    static const std::vector<RecoilStep> SG553_PATTERN;

    void InitializeRecoilPattern(WeaponType weapon);

    void PrecalculateModifiers();
};
