
#pragma once

#include <Windows.h>
#pragma comment(linker, "/subsystem:console")
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif



LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void LamiaInput_KEYDOWN(WPARAM wParam);
