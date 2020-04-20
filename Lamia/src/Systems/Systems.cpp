
#include "Systems.h"

using namespace LamiaSystems;

static Systems* g_LamiaSystem;

LAMIA_RESULT LamiaSystems::LamiaSystemsInit(void)
{
  // initialize the system itself
  g_LamiaSystem = new Systems;

  // initialize our sub systems file, audio, graphics, etc
  bool ret = LamiaFileInit();
  if (ret == false)
    return LAMIA_E_AUDIO_SYS;

  // this is setting our file system pointer lf in g_LamiaSystem
  g_LamiaSystem->SetFileSystemPtr(LamiaFileGetSystem());

  //-- end audio

  ret = LamiaInputInit();
  if (ret == false)
    return LAMIA_E_INPUT_SYS;

  return LAMIA_E_SUCCESS;
}

LAMIA_RESULT LamiaSystems::LamiaSystemsShutdown(void)
{
  delete g_LamiaSystem;

  return LAMIA_E_SUCCESS;
}

Systems* const LamiaSystems::LamiaSystem(void)
{
  return g_LamiaSystem;
}

Systems::Systems()
{
}

Systems::~Systems()
{
}

LamiaFile* const Systems::FileSystem(void)
{
  return LFileSys; // should no longer be null anymore
}

LamiaInput * const LamiaSystems::Systems::InputSystem(void)
{
  return LInputSys;
}

void LamiaSystems::Systems::SetFileSystemPtr(LamiaFile * fileSys)
{
  LFileSys = fileSys;
}

void LamiaSystems::Systems::SetInputSystemPtr(LamiaInput * inputSys)
{
  LInputSys = inputSys;
}
