
#include "LamiaWindow.h"
#include "..\..\LamiaMain.h"
#include "..\Systems\Systems.h"
#include "..\Graphics\LamiaGfxUtil.h"

// MS-Windows event handling function:
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  // this actually needs to be like a system pointer
  // that will hold pointers to all our game systems
  // eg System* holds pointers to sound, graphics, etc
  struct DeviceInfo *info = reinterpret_cast<struct DeviceInfo *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

  switch (uMsg)
  {
  case WM_CLOSE:
    PostQuitMessage(0);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_PAINT:
    LamiaMain(*info, 0.0167777);
    LamiaRender(*info); // game main loop
    return 0;
  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
  case WM_INPUT:
    LamiaSystems::LamiaSystem()->InputSystem()->ReadInputUnbuffered(lParam);
    return 0;

  default:
    break;
  }
  return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}
