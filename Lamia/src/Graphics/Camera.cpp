
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

glm::mat4 Camera::SetView(void)
{
  // i dont know why i keep bothering to add this
  // it's just identity so it doesn't change anything
  // oh well i'll go through and remove it all later
  glm::mat4 model = glm::mat4(1.0f);
  
  /*
  glm::mat4 view = glm::mat4(1.0f);
  // per position change
  view = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

  view = glm::rotate(view, glm::radians(cRot.y), glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::rotate(view, glm::radians(cRot.x), glm::vec3(0.0f, 1.0f, 0.0f));
  view = glm::translate(view, glm::vec3(cPos));

  view[0][1] = view[0][1];

  TestMVP = cUBO.clip * cUBO.proj * view * model;
  */

  
  cUBO.view = glm::lookAt(glm::vec3(cPos.x, cPos.y, cPos.z),  // Camera is at (-5,3,-10), in World Space
    glm::vec3(0, 0, 0),     // and looks at the origin
    glm::vec3(0, -1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
  );
  TestMVP = cUBO.clip * cUBO.proj * cUBO.view * model;  
  

  return cUBO.view;
}
