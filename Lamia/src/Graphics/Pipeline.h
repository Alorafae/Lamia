
#pragma once

#include "LamiaGfxUtil.h"
#include "Camera.h"

struct VertexBufferInfo
{
  VkVertexInputBindingDescription viBinds;
  VkVertexInputAttributeDescription viAttribs[2];
};

class LamiaPipeline
{
  public:
    LamiaPipeline();
    ~LamiaPipeline();

    VkResult CreateDescriptorPipelineLayout(DeviceInfo &di, bool textured, VkDescriptorSetLayoutCreateFlags descSetLayoutCreateFlags = 0);
    VkResult CreateDescriptorPool(DeviceInfo &di, bool textured);
    VkResult CreateDescriptorSet(DeviceInfo &di, VkDescriptorBufferInfo bufInfo, VkDescriptorImageInfo imgInfo, bool textured);

    VkResult CreatePipelineCache(DeviceInfo &di);

    VkResult CreatePipeline(DeviceInfo &di, VkBool32 depth, VkBool32 vertexInput, VertexBufferInfo &VBI, VkPipelineShaderStageCreateInfo *ShdTechStages, bool textured);

    void RenderTest(DeviceInfo &di, VkBuffer vBuff, Camera &cam);

  private:
    VkPipelineLayout pipeLayout;
    VkPipelineCache pipeCache;

    std::vector<VkDescriptorSetLayout> descLayout;
    VkDescriptorPool descPool;
    std::vector<VkDescriptorSet> descSet;

    VkPipeline pipe;
};
