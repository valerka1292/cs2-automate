#pragma once
#include <windows.h>

void SmoothMouseMove(int x, int y, float smoothness);

void FastMouseMove(int x, int y);

void MoveMouseTo(int x, int y);

void LeftMouseDown();
void LeftMouseUp();
void LeftClick();

void RightMouseDown();
void RightMouseUp();
void RightClick();

void SendInputMouseMove(int dx, int dy);
void SendInputLeftMouseDown();
void SendInputLeftMouseUp();
void SendInputLeftClick();
void SendInputFastMouseMove(int dx, int dy);
