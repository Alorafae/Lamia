
#pragma once
#include "LamiaFile.h"
#include "LamiaGraphics.h"
#include "LamiaInput.h"

class Systems
{
  public:
    Systems();
    ~Systems();


    LamiaFile* FileSystem(void);
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

const Systems* LamiaSystem(void);

