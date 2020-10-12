
#include "LamiaInput.h"

#include <stdio.h>
#include <strsafe.h>

#include "..\Graphics\LamiaGfxUtil.h"

static LamiaInput* g_LamiaInput;


LamiaInput::LamiaInput()
{
}

LamiaInput::~LamiaInput()
{
}

void LamiaInput::Update(float dt)
{
  // access to HWND needed I think
  //PeekMessageA();
  HWND hWnd = NULL;
  LPMSG lpMsg =  NULL;
  UINT filterMin = WM_KEYFIRST;
  UINT filterMax = WM_KEYLAST;

  struct DeviceInfo *info = reinterpret_cast<struct DeviceInfo *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

  // if this returns 0, no messages to process
  PeekMessageA(lpMsg, hWnd, filterMin, filterMax, PM_QS_INPUT);
}

void LamiaInput::ReadInputUnbuffered(LPARAM lParam)
{
  
  UINT dwSize;
  HRESULT hResult;
  LPTSTR szTempOutput = new TCHAR[256];

  GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
  LPBYTE lpb = new BYTE[dwSize];
  if (lpb == NULL)
  {
    return;
  }

  if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize,
    sizeof(RAWINPUTHEADER)) != dwSize)
    OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

  RAWINPUT* raw = (RAWINPUT*)lpb;

  if (raw->header.dwType == RIM_TYPEKEYBOARD)
  {
    hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT(" Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n"),
      raw->data.keyboard.MakeCode,
      raw->data.keyboard.Flags,
      raw->data.keyboard.Reserved,
      raw->data.keyboard.ExtraInformation,
      raw->data.keyboard.Message,
      raw->data.keyboard.VKey);
    if (FAILED(hResult))
    {
      // TODO: write error handler
    }
    OutputDebugString(szTempOutput);
  }
  else if (raw->header.dwType == RIM_TYPEMOUSE)
  {
    hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
      raw->data.mouse.usFlags,
      raw->data.mouse.ulButtons,
      raw->data.mouse.usButtonFlags,
      raw->data.mouse.usButtonData,
      raw->data.mouse.ulRawButtons,
      raw->data.mouse.lLastX,
      raw->data.mouse.lLastY,
      raw->data.mouse.ulExtraInformation);

    if (FAILED(hResult))
    {
      // TODO: write error handler
    }
    OutputDebugString(szTempOutput);
  }

  // works :)
  if (raw->data.keyboard.VKey == VK_ESCAPE)
    if (raw->data.keyboard.Flags == RI_KEY_MAKE)
      PostQuitMessage(0);

  ProcessInputMessage(raw->data.keyboard.VKey);

  delete[] lpb;
  return;
}

void LamiaInput::ReadInputBuffered()
{
  printf("Read Input Buffered Called\n");
}

bool LamiaInputInit(void)
{
  g_LamiaInput = new LamiaInput();


  RAWINPUTDEVICE Rid[2]; // this will probably move to the class

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
    // need to find header file function is in
    return false;
  }


  return true;
}

LamiaInput * LamiaInputGetSystem(void)
{
  return g_LamiaInput;
}
