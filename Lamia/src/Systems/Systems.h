
#pragma once
#include "LamiaFile.h"

class Systems
{
  public:
    Systems();
    ~Systems();


    LamiaFile* FileSystem(void);
  private:
    // file system
    LamiaFile *lf;
    // audio system

    // input system

    // graphics system
};

const Systems* LamiaSystem(void);

