
#include "LamiaInput.h"
#include <Windows.h> // need this included in less files which is part of sorting out the whole window situation

static LamiaInput* g_LamiaInput;


LamiaInput::LamiaInput()
{
}

LamiaInput::~LamiaInput()
{
}

void LamiaInput::ReadInputUnbuffered()
{
}

void LamiaInput::ReadInputBuffered()
{
}

bool LamiaInputInit(void)
{
  RAWINPUTDEVICE Rid[2];

  Rid[0].usUsagePage = 0x01;
  Rid[0].usUsage = 0x02;
  Rid[0].dwFlags = RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
  Rid[0].hwndTarget = 0;

  Rid[1].usUsagePage = 0x01;
  Rid[1].usUsage = 0x06;
  Rid[1].dwFlags = RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
  Rid[1].hwndTarget = 0;

  if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE)
  {
    //registration failed. Call GetLastError for the cause of the error
    return false;
  }


  return true;
}

LamiaInput * LamiaInputGetSystem(void)
{
  return nullptr;
}
