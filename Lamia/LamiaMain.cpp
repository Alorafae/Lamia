

#include "LamiaMain.h"

bool LamiaMain(DeviceInfo &info, float dt)
{
  HWND hWnd = info.window;
  MSG lpMsg;
  UINT filterMin = WM_KEYFIRST;
  UINT filterMax = WM_KEYLAST;

  // input
  LamiaSystems::LamiaSystem()->InputSystem()->Update(dt, lpMsg);

  // physics

  // audio

  // render


  return true;
}
