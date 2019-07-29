
#pragma once

#include "LamiaGfxUtil.h"
#include "Pipeline.h"

struct MUniformData
{
  VkBuffer buf;
  VkDeviceMemory mem;
  VkDescriptorBufferInfo info;
};

struct MVertexData
{
  VkBuffer buf;
  VkDeviceMemory mem;
  VkDescriptorBufferInfo info;
};

class Model
{
  public:
    Model();
    ~Model();

    VkResult CreateVertexBuffer(DeviceInfo &di, const void *vertexData, uint32_t dataSize, uint32_t dataStride, bool textured);

    VkVertexInputBindingDescription& GetVBinds(void);
    VkVertexInputAttributeDescription& GetVAtrribs(unsigned index);
    VkBuffer GetVBuffer(void);

    void Update(float dt);

    glm::mat4 GetMatrix(void);

    // maybe later move this to physics object?
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 scale;

  private: //unsure if these will stay private
    VkVertexInputBindingDescription viBinds;
    VkVertexInputAttributeDescription viAttribs[2];

    MVertexData vData;

    glm::mat4 model;
};


