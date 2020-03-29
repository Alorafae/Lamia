
#include "Systems.h"

using namespace LamiaSystems;

static Systems* g_LamiaSystem;

LAMIA_RESULT LamiaSystems::LamiaSystemsInit(void)
{
  // initialize the system itself
  g_LamiaSystem = new Systems();

  // initialize our sub systems file, audio, graphics, etc
  bool ret = LamiaFileInit();
  if (ret == false)
    return LAMIA_E_AUDIO_SYS;



  return LAMIA_E_SUCCESS;
}

LAMIA_RESULT LamiaSystems::LamiaSystemsShutdown(void)
{
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
  return lf;
}
