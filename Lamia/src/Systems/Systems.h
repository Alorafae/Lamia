
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

    void SetFileSystemPtr(LamiaFile* fileSys);
  private:
    // need to make sure all of these do get set when implemented to prevent undefined behavior
    // even though it might work everytime with them null during indirect calls
    // now used correctly
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
