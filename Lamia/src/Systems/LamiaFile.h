
#pragma once
#include "..\..\include\physfs\physfs.h"

class LamiaFile
{
  public:
    LamiaFile();
    ~LamiaFile();

    const char* GetFileData(const char* filename);
  private:

};

bool LamiaFileInit(void);
