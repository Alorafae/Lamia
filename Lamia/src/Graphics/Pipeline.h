
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

    // pipeline layout, desc pool, and actual pipeline creation will vary
    // based on rendering type, uniforms, etc.
    virtual VkResult CreateDescriptorPipelineLayout(DeviceInfo &di, bool textured, VkDescriptorSetLayoutCreateFlags descSetLayoutCreateFlags = 0);
    virtual VkResult CreateDescriptorPool(DeviceInfo &di, bool textured);

    VkResult CreatePipelineCache(DeviceInfo &di);

    virtual VkResult CreatePipeline(DeviceInfo &di, VkBool32 depth, VkBool32 vertexInput, VertexBufferInfo &VBI,
      VkPipelineShaderStageCreateInfo *ShdTechStages, bool textured, VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);


    VkDescriptorPool GetDescPool(void);
    VkDescriptorSetLayout* GetDescLayoutData(void);
    void BindDescriptorSet(DeviceInfo &di, VkDescriptorSet* dSetData);

    // these most likely won't change much between pipelines
    // afaik right now (yet)
    void FrameStart(DeviceInfo &di, FrameInfo &fi);
    void FrameEnd(DeviceInfo &di, FrameInfo &fi);

  private:
    VkPipelineLayout pipeLayout;
    VkPipelineCache pipeCache;

    std::vector<VkDescriptorSetLayout> descLayout;
    VkDescriptorPool descPool;

    VkPipeline pipe;
};
