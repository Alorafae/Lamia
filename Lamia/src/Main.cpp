
#include "..\src\Graphics\LamiaGfxUtil.h"
#include "..\src\Systems\Systems.h"

// make sure to link vulkan-1.lib to the project

int main(void)
{
  // need to make a "system" that holds the
  // physfs, vulkan, messaging, etc
  // that components will have access to 

  LamiaSystems::LamiaSystemsInit();

  PHYSFS_init(NULL);

  DeviceInfo VK_Engine = {};

  // file system, messaging system, physics system
  // vulkan system... more to come probably.

  VK_Start_Sequence(VK_Engine);

  UpdateWindow(VK_Engine.window);

  MSG msg = {};
  while(GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  
  VK_Shutdown_Sequence(VK_Engine);

  return 0;
}
