
#pragma once

#include "LamiaGfxUtil.h"

class Camera
{
  public:
    Camera();
    ~Camera();

  private:
    glm::mat4 view;
    glm::mat4 proj;
};
