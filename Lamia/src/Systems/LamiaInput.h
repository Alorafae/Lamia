
#pragma once


class LamiaInput
{
  public:
    LamiaInput();
    ~LamiaInput();

    void ReadInputUnbuffered();
    void ReadInputBuffered();
  private:

};

bool LamiaInputInit(void);
LamiaInput* LamiaInputGetSystem(void);
