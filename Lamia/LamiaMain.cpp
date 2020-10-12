

#include "LamiaMain.h"

bool LamiaMain(DeviceInfo &info, float dt)
{
  HWND hWnd = info.window;
  MSG lpMsg;
  UINT filterMin = WM_KEYFIRST;
  UINT filterMax = WM_KEYLAST;

  // if this returns 0, no messages to process
  //bool newMsgs = PeekMessageA(&lpMsg, hWnd, 0, 0, PM_REMOVE);


  //if (newMsgs)
    LamiaSystems::LamiaSystem()->InputSystem()->Update(dt, lpMsg);


  return true;
}
