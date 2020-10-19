
#include "LamiaInput.h"

#include <stdio.h>
#include <strsafe.h>

#include "..\Graphics\LamiaGfxUtil.h"

// i really should not have to typedef this when a windows macro uses it
typedef unsigned __int64 QWORD;

static LamiaInput* g_LamiaInput;


LamiaInput::LamiaInput()
{
  this->keyStateRAW = new RAWINPUT;
  this->mouseStateRAW = new RAWINPUT;
}

LamiaInput::~LamiaInput()
{
  delete this->keyStateRAW;
  delete this->mouseStateRAW;
}

void LamiaInput::Update(float dt, MSG &msg)
{
  // access to HWND needed I think
  //PeekMessageA();

  // if this returns 0, no messages to process
  //PeekMessageA(lpMsg, hWnd, filterMin, filterMax, PM_QS_INPUT);

  //ReadInputUnbuffered(msg.lParam);


  //get the keyboard state on a WM_INPUT event with ReadInputUnbuffered
  //record it for next frame in LamiaInput
  //reuse that keyboard state until the next WM_INPUT event message

  //if (!keyStateRAW->data.keyboard.Flags)
    //ProcessInputMessage(keyStateRAW->data.keyboard.VKey);


  // bugged with mouse on clicking window after it's lost focus
  ReadInputBuffered();
}

// pulled from MSDN so it might break later
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

    // saving the keyboard state
    //memcpy(keyStateRAW, raw, sizeof(RAWINPUT));
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

  //if (raw->data.keyboard.VKey == 0x57)
    //if (raw->data.keyboard.Flags == 0)
      //ProcessInputMessage(raw->data.keyboard.VKey);

  delete[] lpb;
  delete[] szTempOutput;
  return;
}

// pulled from MSDN so it might break later
void LamiaInput::ReadInputBuffered()
{
  printf("Read Input Buffered Called\n");

  UINT cbSize;
  Sleep(10);

  //VERIFY(GetRawInputBuffer(NULL, &cbSize, /*0,*/sizeof(RAWINPUTHEADER)) == 0);

  if (GetRawInputBuffer(NULL, &cbSize, sizeof(RAWINPUTHEADER)))
    return;
  //cbSize *= 16;            // this is a wild guess

  if (cbSize == 0)
    return;

  cbSize *= 16; // a different MSDN page says specifically to use 8
  // so idk wtf the 16 above from the rawinput msdn examples is about
  // maybe 16 is for 32 bit??? idk what else, cus 8 doesn't work

  //Log(_T("Allocating %d bytes"), cbSize);
  

  PRAWINPUT pRawInput = (PRAWINPUT)malloc(cbSize);
  if (pRawInput == NULL)
  {
    //Log(_T("Not enough memory"));
    return;
  }
  for (;;)
  {
    UINT cbSizeT = cbSize;
    UINT nInput = GetRawInputBuffer(pRawInput, &cbSizeT, sizeof(RAWINPUTHEADER));

    //Log(_T("nInput = %d"), nInput);
    if (nInput == 0)
    {
      break;
    }
    //ASSERT(nInput > 0);

    // nInput is hitting max uint for some reason
    PRAWINPUT* paRawInput = (PRAWINPUT*)malloc(sizeof(PRAWINPUT) * nInput);

    if (paRawInput == NULL)
    {
      //Log(_T("paRawInput NULL"));
      break;
    }

    PRAWINPUT pri = pRawInput;
    for (UINT i = 0; i < nInput; ++i)
    {
      //Log(_T(" input[%d] = @%p"), i, pri);
      paRawInput[i] = pri;

      // record vkey message and flags?

      pri = NEXTRAWINPUTBLOCK(pri);
    }
    // to clean the buffer
    DefRawInputProc(paRawInput, nInput, sizeof(RAWINPUTHEADER));

    free(paRawInput);
  }
  free(pRawInput);
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
