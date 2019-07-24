
#include "Camera.h"

Camera::Camera()
{
  float fov = glm::radians(90.0f);

  if (di.width > di.height)
  {
    fov *= static_cast<float>(di.height) / static_cast<float>(di.width);
  }

  this->proj = glm::perspective(fov, static_cast<float>(di.width) / static_cast<float>(di.height), 0.1f, 100.0f);
  this->view = glm::lookAt(glm::vec3(-5, 3, -10),  // Camera is at (-5,3,-10), in World Space
    glm::vec3(0, 0, 0),     // and looks at the origin
    glm::vec3(0, -1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
  );

  //di.Model = glm::mat4(1.0f);
  // Vulkan clip space has inverted Y and half Z.
  this->clip = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f);

  di.MVP = this->clip * di.Projection * di.View;
}

Camera::Camera(DeviceInfo &di)
{
  float fov = glm::radians(90.0f);

  if (di.width > di.height)
  {
    fov *= static_cast<float>(di.height) / static_cast<float>(di.width);
  }

  this->proj = glm::perspective(fov, static_cast<float>(di.width) / static_cast<float>(di.height), 0.1f, 100.0f);
  this->view = glm::lookAt(glm::vec3(-5, 3, -10),  // Camera is at (-5,3,-10), in World Space
    glm::vec3(0, 0, 0),     // and looks at the origin
    glm::vec3(0, -1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
  );

  //di.Model = glm::mat4(1.0f);
  // Vulkan clip space has inverted Y and half Z.
  this->clip = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f);

  //di.MVP = this->clip * di.Projection * di.View;
}

Camera::~Camera()
{
}
