
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
  private: //unsure if these will stay private
    VkVertexInputBindingDescription vi_binding;
    VkVertexInputAttributeDescription vi_attribs[2];
};


