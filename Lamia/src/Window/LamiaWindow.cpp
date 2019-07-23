
#include "LamiaWindow.h"

void LamiaInput_KEYDOWN(WPARAM wParam)
{
  switch (wParam)
  {
    case VK_ESCAPE:
      PostQuitMessage(0);
      break;
  }
}
