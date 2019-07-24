
#pragma once

#include "LamiaGfxUtil.h"

// uniform buffer object
struct CameraUBO
{
  glm::mat4 view;
  glm::mat4 proj;
  glm::mat4 model;
};

class Camera
{
  public:
    Camera();
    Camera(DeviceInfo &di);
    ~Camera();

    DeviceInfo di; // vulkan device info
  private:
    glm::mat4 view;
    glm::mat4 proj;
    glm::mat4 clip;
};
