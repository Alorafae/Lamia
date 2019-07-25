
#pragma once

#include "LamiaGfxUtil.h"

class LamiaPipeline
{
  public:
    LamiaPipeline();
    ~LamiaPipeline();

  private:
    VkPipelineLayout pipeLayout;
    VkPipelineCache pipeCache;

    std::vector<VkDescriptorSetLayout> descLayout;
    VkDescriptorPool descPool;
    std::vector<VkDescriptorSet> descSet;

    VkPipeline pipe;
};
