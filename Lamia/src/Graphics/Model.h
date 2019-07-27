
#pragma once

#include "LamiaGfxUtil.h"

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

  private: //unsure if these will stay private
    VkVertexInputBindingDescription viBinds;
    VkVertexInputAttributeDescription viAttribs[2];

    MVertexData vData;
};


