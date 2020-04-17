
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
  return lf; // should no longer be null anymore
}

void LamiaSystems::Systems::SetFileSystemPtr(LamiaFile * fileSys)
{
  lf = fileSys;
}
