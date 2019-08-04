
#include "Camera.h"

Camera::Camera()
{
  float fov = glm::radians(90.0f);

  if (DEFAULT_WIDTH > DEFAULT_HEIGHT)
  {
    fov *= static_cast<float>(DEFAULT_HEIGHT) / static_cast<float>(DEFAULT_WIDTH);
  }

  cUBO.proj = glm::perspective(fov, static_cast<float>(DEFAULT_WIDTH) / static_cast<float>(DEFAULT_HEIGHT), DEFAULT_NEAR_PLANE, DEFAULT_FAR_PLANE);
  cUBO.view = glm::lookAt(glm::vec3(-5, -3, -10),  // Camera is at (-5,3,-10), in World Space
    glm::vec3(0, 0, 0),     // and looks at the origin
    glm::vec3(0, -1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
  );

  glm::mat4 model = glm::mat4(1.0f);
  // Vulkan clip space has inverted Y and half Z.
  cUBO.clip = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f);

  //di.MVP = this->clip * di.Projection * di.View * di.Model;
  TestMVP = cUBO.clip * cUBO.proj * cUBO.view * model;
}

Camera::Camera(DeviceInfo &di)
{
  float fov = glm::radians(90.0f);

  if (di.width > di.height)
  {
    fov *= static_cast<float>(di.height) / static_cast<float>(di.width);
  }

  cUBO.proj = glm::perspective(fov, static_cast<float>(di.width) / static_cast<float>(di.height), DEFAULT_NEAR_PLANE, DEFAULT_FAR_PLANE);
  cUBO.view = glm::lookAt(glm::vec3(-5, 3, -10),  // Camera is at (-5,3,-10), in World Space
    glm::vec3(0, 0, 0),     // and looks at the origin
    glm::vec3(0, -1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
  );

  //di.Model = glm::mat4(1.0f);
  // Vulkan clip space has inverted Y and half Z.
  cUBO.clip = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f);

  //di.MVP = this->clip * di.Projection * di.View * di.Model;
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetMVP(void)
{
  return TestMVP;
}

void Camera::SetViewport(DeviceInfo & di)
{
  viewport.height = (float)di.height;
  viewport.width = (float)di.width;
  viewport.minDepth = (float)0.0f;
  viewport.maxDepth = (float)1.0f;
  viewport.x = 0;
  viewport.y = 0;
  vkCmdSetViewport(di.cmd, 0, NUM_VIEWPORTS, &viewport);
}

void Camera::SetScissor(DeviceInfo & di)
{
  scissor.extent.width = di.width;
  scissor.extent.height = di.height;
  scissor.offset.x = 0;
  scissor.offset.y = 0;
  vkCmdSetScissor(di.cmd, 0, NUM_SCISSORS, &scissor);
}
