
#include "Camera.h"

Camera::Camera()
{
  float fov = glm::radians(90.0f);

  if (DEFAULT_WIDTH > DEFAULT_HEIGHT)
  {
    fov *= static_cast<float>(DEFAULT_HEIGHT) / static_cast<float>(DEFAULT_WIDTH);
  }

  cUBO.proj = glm::perspective(fov, static_cast<float>(DEFAULT_WIDTH) / static_cast<float>(DEFAULT_HEIGHT), DEFAULT_NEAR_PLANE, DEFAULT_FAR_PLANE);
  cUBO.view = glm::lookAt(glm::vec3(-5, 3, -10),  // Camera is at (-5,3,-10), in World Space
    glm::vec3(0, 0, 0),     // and looks at the origin
    glm::vec3(0, -1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
  );

  //di.Model = glm::mat4(1.0f);
  // Vulkan clip space has inverted Y and half Z.
  cUBO.clip = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f);

  //di.MVP = this->clip * di.Projection * di.View * di.Model;
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

void Camera::BindUBO(DeviceInfo &info, glm::mat4 &model)
{
  // model will change for every object
  cUBO.model = model;

  VkBufferCreateInfo bi = {};
  bi.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bi.pNext = NULL;
  bi.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  bi.size = sizeof(cUBO);
  bi.queueFamilyIndexCount = 0;
  bi.pQueueFamilyIndices = NULL;
  bi.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  bi.flags = 0;
  VkResult U_ASSERT_ONLY res = vkCreateBuffer(info.device, &bi, NULL, &UBOBuffer);
  assert(res == VK_SUCCESS);

  VkMemoryRequirements mr;
  vkGetBufferMemoryRequirements(info.device, UBOBuffer, &mr);

  VkMemoryAllocateInfo ai = {};
  ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  ai.pNext = NULL;
  ai.memoryTypeIndex = 0;

  ai.allocationSize = mr.size;
  bool U_ASSERT_ONLY pass = memory_type_from_properties(info, mr.memoryTypeBits,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    &ai.memoryTypeIndex);
  assert(pass && "No mappable, coherent memory");

  res = vkAllocateMemory(info.device, &ai, NULL, &(VKD_Mem));
  assert(res == VK_SUCCESS);

  // map memory may need to be different or not needed
  uint8_t *pData;
  res = vkMapMemory(info.device, VKD_Mem, 0, mr.size, 0, (void **)&pData);
  assert(res == VK_SUCCESS);

  memcpy(pData, &cUBO, sizeof(cUBO));

  vkUnmapMemory(info.device, VKD_Mem);

  res = vkBindBufferMemory(info.device, UBOBuffer, VKD_Mem, 0);
  assert(res == VK_SUCCESS);

  UBufferInfo.buffer = UBOBuffer;
  UBufferInfo.offset = 0;
  UBufferInfo.range = sizeof(cUBO);
}
