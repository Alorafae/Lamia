
#include "Systems.h"

static Systems* g_LamiaSystem;

bool LamiaSystemsInit(void)
{
  // initialize the system itself
  g_LamiaSystem = new Systems();

  // initialize our sub systems file, audio, graphics, etc
  LamiaFileInit();


  return false;
}

bool LamiaSystemsShutdown(void)
{
  return false;
}

Systems* const LamiaSystem(void)
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
