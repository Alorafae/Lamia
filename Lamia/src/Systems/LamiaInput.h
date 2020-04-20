
#pragma once
#include <Windows.h> // need this included in less files which is part of sorting out the whole window situation

class LamiaInput
{
  public:
    LamiaInput();
    ~LamiaInput();

    void ReadInputUnbuffered(LPARAM lParam);
    void ReadInputBuffered();
  private:

};

bool LamiaInputInit(void);
LamiaInput* LamiaInputGetSystem(void);
