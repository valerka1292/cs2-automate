#include "mouse_control.h"

void SendInputMouseMove(int dx, int dy) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    SendInput(1, &input, sizeof(INPUT));
}

void SendInputLeftMouseDown() {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));
}

void SendInputLeftMouseUp() {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

void SendInputLeftClick() {
    SendInputLeftMouseDown();
    Sleep(1); // Минимальная задержка
    SendInputLeftMouseUp();
}

void SendInputFastMouseMove(int dx, int dy) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    SendInput(1, &input, sizeof(INPUT));
}

void SmoothMouseMove(int targetX, int targetY, float smoothness) {
    const int steps = 3;
    const float stepSizeX = static_cast<float>(targetX) / steps;
    const float stepSizeY = static_cast<float>(targetY) / steps;
    const int sleepTime = static_cast<int>(1 * smoothness);

    static INPUT inputs[3];

    float currentX = static_cast<float>(targetX);
    float currentY = static_cast<float>(targetY);

    for (int i = 0; i < steps; ++i) {
        currentX -= stepSizeX;
        currentY -= stepSizeY;

        inputs[i].type = INPUT_MOUSE;
        inputs[i].mi.dwFlags = MOUSEEVENTF_MOVE;
        inputs[i].mi.dx = static_cast<LONG>(currentX);
        inputs[i].mi.dy = static_cast<LONG>(currentY);
        inputs[i].mi.dwExtraInfo = 0;
        inputs[i].mi.mouseData = 0;
        inputs[i].mi.time = 0;
    }

    SendInput(steps, inputs, sizeof(INPUT));

    if (sleepTime > 0) Sleep(sleepTime);
}

void MoveMouseTo(int x, int y) {
    static INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    input.mi.dx = static_cast<LONG>(x * (65535.0 / GetSystemMetrics(SM_CXSCREEN)));
    input.mi.dy = static_cast<LONG>(y * (65535.0 / GetSystemMetrics(SM_CYSCREEN)));
    input.mi.dwExtraInfo = 0;
    input.mi.mouseData = 0;
    input.mi.time = 0;

    SendInput(1, &input, sizeof(INPUT));
}

void LeftMouseDown() {
    static INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    input.mi.dx = 0;
    input.mi.dy = 0;
    input.mi.dwExtraInfo = 0;
    input.mi.mouseData = 0;
    input.mi.time = 0;

    SendInput(1, &input, sizeof(INPUT));
}

void LeftMouseUp() {
    static INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    input.mi.dx = 0;
    input.mi.dy = 0;
    input.mi.dwExtraInfo = 0;
    input.mi.mouseData = 0;
    input.mi.time = 0;

    SendInput(1, &input, sizeof(INPUT));
}

void LeftClick() {
    LeftMouseDown();
    LeftMouseUp();
}

void RightMouseDown() {
    static INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    input.mi.dx = 0;
    input.mi.dy = 0;
    input.mi.dwExtraInfo = 0;
    input.mi.mouseData = 0;
    input.mi.time = 0;

    SendInput(1, &input, sizeof(INPUT));
}

void RightMouseUp() {
    static INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    input.mi.dx = 0;
    input.mi.dy = 0;
    input.mi.dwExtraInfo = 0;
    input.mi.mouseData = 0;
    input.mi.time = 0;

    SendInput(1, &input, sizeof(INPUT));
}

void RightClick() {
    RightMouseDown();
    RightMouseUp();
}

void FastMouseMove(int x, int y) {
    static INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = x;
    input.mi.dy = y;
    input.mi.dwExtraInfo = 0;
    input.mi.mouseData = 0;
    input.mi.time = 0;

    SendInput(1, &input, sizeof(INPUT));
}

