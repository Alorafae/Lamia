
#pragma once
#include <Windows.h> // need this included in less files which is part of sorting out the whole window situation
#include <vector>
// class to create and keep track of the real-time state of the keyboard
// maybe just use an array that can hold each VK
// i dont think ill ever want to use this but ima just leave it here for now
class LamiaKeyboard
{
  public:
    LamiaKeyboard();
    LamiaKeyboard(UINT key, UINT flags, UINT msg);
    ~LamiaKeyboard();

  //private:
    UINT pKey = 0;
    UINT pFlags = 0;
    UINT pMsg = 0;
};

class LamiaInputStateMachine
{
  public:
    // map skip list for active keys?

  //private:
    LamiaKeyboard keys[256];

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

    //bool inputChanged = false;

    std::vector<LamiaKeyboard> kbInputQ;

    LamiaInputStateMachine lism;
};

bool LamiaInputInit(void);
LamiaInput* LamiaInputGetSystem(void);
