
#pragma once
#include <Windows.h> // need this included in less files which is part of sorting out the whole window situation
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

class LamiaMouse
{
  public:
    LamiaMouse();
    ~LamiaMouse();


    USHORT mButtonFlags;
    ULONG mLastX;
    ULONG mLastY;
};

class LamiaInputStateMachine
{
  public:
    // map skip list for active keys?

  //private:
    // might change this to std::bitset
    // also needs to not be hard coded obviously
    LamiaKeyboard keys[256];

    // need to add mouse stuff
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
    LamiaInputStateMachine lism;
};

bool LamiaInputInit(void);
LamiaInput* LamiaInputGetSystem(void);
