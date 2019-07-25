
#pragma once

#include "LamiaGfxUtil.h"

class LamiaPipeline
{
  public:
    LamiaPipeline();
    ~LamiaPipeline();

    VkResult CreateDescriptorPipelineLayout(DeviceInfo &di, bool textured, VkDescriptorSetLayoutCreateFlags descSetLayoutCreateFlags = 0);
    VkResult CreateDescriptorPool(DeviceInfo &di, bool textured);
    VkResult CreateDescriptorSet(DeviceInfo &di, bool textured);

    VkResult CreatePipelineCache(DeviceInfo &di);

    VkResult CreatePipeline(DeviceInfo &di, VkBool32 depth, VkBool32 vertexInput, bool textured);

  private:
    VkPipelineLayout pipeLayout;
    VkPipelineCache pipeCache;

    std::vector<VkDescriptorSetLayout> descLayout;
    VkDescriptorPool descPool;
    std::vector<VkDescriptorSet> descSet;

    VkPipeline pipe;
};
