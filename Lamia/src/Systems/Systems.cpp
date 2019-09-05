
#include "Systems.h"

using namespace LamiaSystems;

static Systems* g_LamiaSystem;

bool LamiaSystems::LamiaSystemsInit(void)
{
  // initialize the system itself
  g_LamiaSystem = new Systems();

  // initialize our sub systems file, audio, graphics, etc
  LamiaFileInit();


  return false;
}

bool LamiaSystems::LamiaSystemsShutdown(void)
{
  return false;
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
