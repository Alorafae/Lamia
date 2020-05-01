
#pragma once
#include "..\Graphics\LamiaGfxUtil.h"

class LamiaGraphics
{
  public:
    LamiaGraphics();
    ~LamiaGraphics();



    void Update(float dt);


  private:
    VulkanInfo* vi;
};

bool LamiaGraphicsInit(void);
