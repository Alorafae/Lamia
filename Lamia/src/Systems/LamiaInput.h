
#pragma once
#include <Windows.h> // need this included in less files which is part of sorting out the whole window situation

// class to create and keep track of the real-time state of the keyboard
// maybe just use an array that can hold each VK
class LamiaKeyboard
{
  public:

  private:

};

class LamiaInput
{
  public:
    LamiaInput();
    ~LamiaInput();

    void Update(float dt, MSG &msg);

    void ReadInputUnbuffered(LPARAM lParam);
    void ReadInputBuffered();
  private:
    RAWINPUT* keyStateRAW = NULL;
    RAWINPUT* mouseStateRAW = NULL;
};

bool LamiaInputInit(void);
LamiaInput* LamiaInputGetSystem(void);
