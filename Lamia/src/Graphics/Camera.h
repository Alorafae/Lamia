
#pragma once

#include "LamiaGfxUtil.h"

#define DEFAULT_NEAR_PLANE 0.1f
#define DEFAULT_FAR_PLANE 1000.f

// uniform buffer object
struct CameraUBO
{
  glm::mat4 clip;
  glm::mat4 proj;
  glm::mat4 view;
  glm::mat4 model;
  glm::mat4 mvp;
};

class Camera
{
  public:
    Camera();
    Camera(DeviceInfo &di);
    ~Camera();

    glm::mat4 GetMVP(void);

    void SetViewport(DeviceInfo &di);
    void SetScissor(DeviceInfo &di);

    glm::mat4 SetView(void);

    glm::vec3 cPos = glm::vec3(0.0f);
    glm::vec3 cRot = glm::vec3(0.0f);

    glm::mat4 TestMVP;
    
  private:
    CameraUBO cUBO;
    glm::mat4 cView = glm::mat4(1.0f);

    VkViewport viewport;
    VkRect2D scissor;
};
