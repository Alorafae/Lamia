
#pragma once
#include "LamiaFile.h"
#include "LamiaGraphics.h"
#include "LamiaInput.h"

enum LAMIA_RESULT
{
  LAMIA_E_SUCCESS = 10000,
  LAMIA_E_FILE_SYS,
  LAMIA_E_FILE_NOT_FOUND,
  LAMIA_E_FILE_DBL_FREE,
  LAMIA_E_AUDIO_SYS
};


namespace LamiaSystems
{

  class Systems
  {
  public:
    Systems();
    ~Systems();

    LamiaFile* const FileSystem(void);
  private:
    // these are currently unused but also used
    // file system
    LamiaFile* lf = NULL;
    // audio system

    // raw input system
    LamiaInput* li = NULL;
    // graphics system
    LamiaGraphics* vi = NULL;
    // physics system
  };

  LAMIA_RESULT LamiaSystemsInit(void);
  LAMIA_RESULT LamiaSystemsShutdown(void);

  Systems* const LamiaSystem(void);

}
