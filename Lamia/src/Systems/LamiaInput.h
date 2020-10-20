
#pragma once
#include <Windows.h> // need this included in less files which is part of sorting out the whole window situation
#include <queue>

// class to create and keep track of the real-time state of the keyboard
// maybe just use an array that can hold each VK
// i dont think ill ever want to use this but ima just leave it here for now
class LamiaKeyboard
{
  public:

  //private:
    UINT key = 0;
    UINT flags = 0;
    UINT msg = 0;
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

    std::queue<LamiaKeyboard> kbInput;
};

bool LamiaInputInit(void);
LamiaInput* LamiaInputGetSystem(void);
