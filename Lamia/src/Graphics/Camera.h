
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
};

class Camera
{
  public:
    Camera();
    Camera(DeviceInfo &di);
    ~Camera();

    void Update(float dt);
    void BindUBO(DeviceInfo &info, glm::mat4 &model);

    void SetViewport(DeviceInfo &di);
    void SetScissor(DeviceInfo &di);

    glm::vec3 cPos = glm::vec3(0.0f);
    
  private:
    CameraUBO cUBO;

    VkViewport viewport;
    VkRect2D scissor;

    // uniform data
    VkBuffer UBOBuffer;
    VkDeviceMemory VKD_Mem;
    VkDescriptorBufferInfo UBufferInfo;
};
