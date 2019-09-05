
#pragma once
#include "LamiaFile.h"
#include "LamiaGraphics.h"
#include "LamiaInput.h"

namespace LamiaSystems
{

  class Systems
  {
  public:
    Systems();
    ~Systems();

    LamiaFile* const FileSystem(void);
  private:
    // file system
    LamiaFile* lf = NULL;
    // audio system

    // raw input system
    LamiaInput* li = NULL;
    // graphics system
    LamiaGraphics* vi = NULL;
    // physics system
  };

  bool LamiaSystemsInit(void);
  bool LamiaSystemsShutdown(void);

  Systems* const LamiaSystem(void);

}
