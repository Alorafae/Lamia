
#pragma once

#include "LamiaGfxUtil.h"
#include "Camera.h"

struct VertexBufferInfo
{
  VkVertexInputBindingDescription viBinds;
  VkVertexInputAttributeDescription viAttribs[2];
};

struct FrameInfo
{
  VkSemaphore imageAcquiredSemaphore;
  VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;
  VkRenderPassBeginInfo rpBeginInfo;

  VkFenceCreateInfo fenceInfo;
  VkFence drawFence;
};

class LamiaPipeline
{
  public:
    LamiaPipeline();
    ~LamiaPipeline();

    VkResult CreateDescriptorPipelineLayout(DeviceInfo &di, bool textured, VkDescriptorSetLayoutCreateFlags descSetLayoutCreateFlags = 0);
    VkResult CreateDescriptorPool(DeviceInfo &di, bool textured);

    VkResult CreatePipelineCache(DeviceInfo &di);

    VkResult CreatePipeline(DeviceInfo &di, VkBool32 depth, VkBool32 vertexInput, VertexBufferInfo &VBI, VkPipelineShaderStageCreateInfo *ShdTechStages, bool textured);


    VkDescriptorPool GetDescPool(void);
    VkDescriptorSetLayout* GetDescLayoutData(void);
    void BindDescriptorSet(DeviceInfo &di, VkDescriptorSet* dSetData);

    void FrameStart(DeviceInfo &di, FrameInfo &fi);


    void FrameEnd(DeviceInfo &di, FrameInfo &fi);

  private:
    VkPipelineLayout pipeLayout;
    VkPipelineCache pipeCache;

    std::vector<VkDescriptorSetLayout> descLayout;
    VkDescriptorPool descPool;

    VkPipeline pipe;
};
