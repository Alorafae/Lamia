
#include "Pipeline.h"

LamiaPipeline::LamiaPipeline()
{
}

LamiaPipeline::~LamiaPipeline()
{
}

VkResult LamiaPipeline::CreateDescriptorPipelineLayout(DeviceInfo & di, bool textured, VkDescriptorSetLayoutCreateFlags descSetLayoutCreateFlags)
{

  // shader layout bindings i think
  VkDescriptorSetLayoutBinding layout_bindings[2];
  layout_bindings[0].binding = 0;
  layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  layout_bindings[0].descriptorCount = 1;
  layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  layout_bindings[0].pImmutableSamplers = NULL;

  // binding a texture to the shader? aka sampler
  if (textured)
  {
    layout_bindings[1].binding = 1;
    layout_bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    layout_bindings[1].descriptorCount = 1;
    layout_bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    layout_bindings[1].pImmutableSamplers = NULL;
  }

  /* Next take layout bindings and use them to create a descriptor set layout
  */
  VkDescriptorSetLayoutCreateInfo descriptor_layout = {};
  descriptor_layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  descriptor_layout.pNext = NULL;
  descriptor_layout.flags = descSetLayoutCreateFlags;
  descriptor_layout.bindingCount = textured ? 2 : 1;
  descriptor_layout.pBindings = layout_bindings;

  VkResult U_ASSERT_ONLY res;

  descLayout.resize(NUM_DESCRIPTOR_SETS);
  res = vkCreateDescriptorSetLayout(di.device, &descriptor_layout, NULL, descLayout.data());
  assert(res == VK_SUCCESS);

  /* Now use the descriptor layout to create a pipeline layout */
  VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
  pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pPipelineLayoutCreateInfo.pNext = NULL;
  pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
  pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;
  pPipelineLayoutCreateInfo.setLayoutCount = NUM_DESCRIPTOR_SETS;
  pPipelineLayoutCreateInfo.pSetLayouts = descLayout.data();

  res = vkCreatePipelineLayout(di.device, &pPipelineLayoutCreateInfo, NULL, &pipeLayout);
  assert(res == VK_SUCCESS);

  return res;
}

VkResult LamiaPipeline::CreateDescriptorPool(DeviceInfo & di, bool textured)
{
  /* uniform buffer & pipeline layout dependent*/

  VkResult U_ASSERT_ONLY res;
  VkDescriptorPoolSize type_count[2];
  type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  type_count[0].descriptorCount = 1;
  if (textured) {
    type_count[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    type_count[1].descriptorCount = 1;
  }

  VkDescriptorPoolCreateInfo descriptor_pool = {};
  descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  descriptor_pool.pNext = NULL;
  descriptor_pool.maxSets = 2; // total number of descriptor sets that can be created for this pipeline
  descriptor_pool.poolSizeCount = textured ? 2 : 1;
  descriptor_pool.pPoolSizes = type_count;

  res = vkCreateDescriptorPool(di.device, &descriptor_pool, NULL, &descPool);
  assert(res == VK_SUCCESS);

  return res;
}

VkResult LamiaPipeline::CreatePipelineCache(DeviceInfo & di)
{
  VkResult U_ASSERT_ONLY res;

  VkPipelineCacheCreateInfo pipelineCache;
  pipelineCache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
  pipelineCache.pNext = NULL;
  pipelineCache.initialDataSize = 0;
  pipelineCache.pInitialData = NULL;
  pipelineCache.flags = 0;
  res = vkCreatePipelineCache(di.device, &pipelineCache, NULL, &pipeCache);
  assert(res == VK_SUCCESS);

  return res;
}

VkResult LamiaPipeline::CreatePipeline(DeviceInfo & di, VkBool32 depth, VkBool32 vertexInput, VertexBufferInfo &VBI,
  VkPipelineShaderStageCreateInfo *ShdTechStages, bool textured, VkPrimitiveTopology topology)
{
  VkResult U_ASSERT_ONLY res;

  VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
  VkPipelineDynamicStateCreateInfo dynamicState = {};
  memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);
  dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicState.pNext = NULL;
  dynamicState.pDynamicStates = dynamicStateEnables;
  dynamicState.dynamicStateCount = 0;

  VkPipelineVertexInputStateCreateInfo vi;
  memset(&vi, 0, sizeof(vi));
  vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

  if (vertexInput) {
    vi.pNext = NULL;
    vi.flags = 0;
    vi.vertexBindingDescriptionCount = 1;
    vi.pVertexBindingDescriptions = &VBI.viBinds;
    vi.vertexAttributeDescriptionCount = 2;
    vi.pVertexAttributeDescriptions = VBI.viAttribs;
  }
  // this determines triangles, lines, etc
  VkPipelineInputAssemblyStateCreateInfo ia;
  ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  ia.pNext = NULL;
  ia.flags = 0;
  ia.primitiveRestartEnable = VK_FALSE;
  // this is tri list, line list, etc
  ia.topology = topology;

  VkPipelineRasterizationStateCreateInfo rs;
  rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rs.pNext = NULL;
  rs.flags = 0;
  rs.polygonMode = VK_POLYGON_MODE_FILL;
  rs.cullMode = VK_CULL_MODE_BACK_BIT;
  rs.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rs.depthClampEnable = VK_FALSE;
  rs.rasterizerDiscardEnable = VK_FALSE;
  rs.depthBiasEnable = VK_FALSE;
  rs.depthBiasConstantFactor = 0;
  rs.depthBiasClamp = 0;
  rs.depthBiasSlopeFactor = 0;
  rs.lineWidth = 1.0f;

  VkPipelineColorBlendStateCreateInfo cb;
  cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  cb.flags = 0;
  cb.pNext = NULL;
  VkPipelineColorBlendAttachmentState att_state[1];
  att_state[0].colorWriteMask = 0xf;
  att_state[0].blendEnable = VK_FALSE;
  att_state[0].alphaBlendOp = VK_BLEND_OP_ADD;
  att_state[0].colorBlendOp = VK_BLEND_OP_ADD;
  att_state[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  att_state[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  att_state[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  att_state[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  cb.attachmentCount = 1;
  cb.pAttachments = att_state;
  cb.logicOpEnable = VK_FALSE;
  cb.logicOp = VK_LOGIC_OP_NO_OP;
  cb.blendConstants[0] = 1.0f;
  cb.blendConstants[1] = 1.0f;
  cb.blendConstants[2] = 1.0f;
  cb.blendConstants[3] = 1.0f;

  VkPipelineViewportStateCreateInfo vp = {};
  vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  vp.pNext = NULL;
  vp.flags = 0;

  vp.viewportCount = NUM_VIEWPORTS;
  dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_VIEWPORT;
  vp.scissorCount = NUM_SCISSORS;
  dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_SCISSOR;
  vp.pScissors = NULL;
  vp.pViewports = NULL;

  VkPipelineDepthStencilStateCreateInfo ds;
  ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  ds.pNext = NULL;
  ds.flags = 0;
  ds.depthTestEnable = depth;
  ds.depthWriteEnable = depth;
  ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
  ds.depthBoundsTestEnable = VK_FALSE;
  ds.stencilTestEnable = VK_FALSE;
  ds.back.failOp = VK_STENCIL_OP_KEEP;
  ds.back.passOp = VK_STENCIL_OP_KEEP;
  ds.back.compareOp = VK_COMPARE_OP_ALWAYS;
  ds.back.compareMask = 0;
  ds.back.reference = 0;
  ds.back.depthFailOp = VK_STENCIL_OP_KEEP;
  ds.back.writeMask = 0;
  ds.minDepthBounds = 0;
  ds.maxDepthBounds = 0;
  ds.stencilTestEnable = VK_FALSE;
  ds.front = ds.back;

  VkPipelineMultisampleStateCreateInfo ms;
  ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  ms.pNext = NULL;
  ms.flags = 0;
  ms.pSampleMask = NULL;
  ms.rasterizationSamples = NUM_SAMPLES;
  ms.sampleShadingEnable = VK_FALSE;
  ms.alphaToCoverageEnable = VK_FALSE;
  ms.alphaToOneEnable = VK_FALSE;
  ms.minSampleShading = 0.0;

  VkGraphicsPipelineCreateInfo pipeline;
  pipeline.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline.pNext = NULL;
  pipeline.layout = pipeLayout;
  pipeline.basePipelineHandle = VK_NULL_HANDLE;
  pipeline.basePipelineIndex = 0;
  pipeline.flags = 0;
  pipeline.pVertexInputState = &vi;
  pipeline.pInputAssemblyState = &ia;
  pipeline.pRasterizationState = &rs;
  pipeline.pColorBlendState = &cb;
  pipeline.pTessellationState = NULL;
  pipeline.pMultisampleState = &ms;
  pipeline.pDynamicState = &dynamicState;
  pipeline.pViewportState = &vp;
  pipeline.pDepthStencilState = &ds;
  // watch pStages closely
  pipeline.pStages = ShdTechStages;
  pipeline.stageCount = 2;
  pipeline.renderPass = di.render_pass;
  pipeline.subpass = 0;

  res = vkCreateGraphicsPipelines(di.device, pipeCache, 1, &pipeline, NULL, &pipe);
  assert(res == VK_SUCCESS);

  return res;
}

VkDescriptorPool LamiaPipeline::GetDescPool(void)
{
  return descPool;
}

VkDescriptorSetLayout* LamiaPipeline::GetDescLayoutData(void)
{
  return descLayout.data();
}

void LamiaPipeline::BindDescriptorSet(DeviceInfo & di, VkDescriptorSet * dSetData)
{
  vkCmdBindDescriptorSets(di.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLayout, 0, NUM_DESCRIPTOR_SETS, dSetData, 0, NULL);
}

void LamiaPipeline::FrameStart(DeviceInfo & di, FrameInfo & fi)
{
  VkResult U_ASSERT_ONLY res;

  VkClearValue clear_values[2];
  clear_values[0].color.float32[0] = 0.0f;
  clear_values[0].color.float32[1] = 0.0f;
  clear_values[0].color.float32[2] = 0.0f;
  clear_values[0].color.float32[3] = 0.0f;
  clear_values[1].depthStencil.depth = 1.0f;
  clear_values[1].depthStencil.stencil = 0;

  VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;
  imageAcquiredSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  imageAcquiredSemaphoreCreateInfo.pNext = NULL;
  imageAcquiredSemaphoreCreateInfo.flags = 0;


  res = vkCreateSemaphore(di.device, &imageAcquiredSemaphoreCreateInfo, NULL, &fi.imageAcquiredSemaphore);
  assert(res == VK_SUCCESS);

  // Get the index of the next available swapchain image:
  res = vkAcquireNextImageKHR(di.device, di.swap_chain, UINT64_MAX, fi.imageAcquiredSemaphore, VK_NULL_HANDLE, &di.current_buffer);

  // TODO: Deal with the VK_SUBOPTIMAL_KHR and VK_ERROR_OUT_OF_DATE_KHR
  // return codes
  assert(res == VK_SUCCESS);

  VkRenderPassBeginInfo rp_begin;
  rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  rp_begin.pNext = NULL;
  rp_begin.renderPass = di.render_pass;
  rp_begin.framebuffer = di.framebuffers[di.current_buffer];
  rp_begin.renderArea.offset.x = 0;
  rp_begin.renderArea.offset.y = 0;
  rp_begin.renderArea.extent.width = di.width;
  rp_begin.renderArea.extent.height = di.height;
  rp_begin.clearValueCount = 2;
  rp_begin.pClearValues = clear_values;

  //-----
  VK_Exec_Cmd_Buffer(di);

  vkCmdBeginRenderPass(di.cmd, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(di.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipe);

  // change per object
  //vkCmdBindDescriptorSets(di.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLayout, 0, NUM_DESCRIPTOR_SETS, descSet.data(), 0, NULL);
}

void LamiaPipeline::FrameEnd(DeviceInfo & di, FrameInfo & fi)
{
  VkResult U_ASSERT_ONLY res;

  vkCmdEndRenderPass(di.cmd);
  res = vkEndCommandBuffer(di.cmd);
  //---

  const VkCommandBuffer cmd_bufs[] = { di.cmd };

  VkFenceCreateInfo fenceInfo;
  VkFence drawFence;
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.pNext = NULL;
  fenceInfo.flags = 0;
  vkCreateFence(di.device, &fenceInfo, NULL, &drawFence);

  VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  VkSubmitInfo submit_info[1] = {};
  submit_info[0].pNext = NULL;
  submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info[0].waitSemaphoreCount = 1;
  submit_info[0].pWaitSemaphores = &fi.imageAcquiredSemaphore;
  submit_info[0].pWaitDstStageMask = &pipe_stage_flags;
  submit_info[0].commandBufferCount = 1;
  submit_info[0].pCommandBuffers = cmd_bufs;
  submit_info[0].signalSemaphoreCount = 0;
  submit_info[0].pSignalSemaphores = NULL;

  /* Queue the command buffer for execution */
  res = vkQueueSubmit(di.graphics_queue, 1, submit_info, drawFence);
  assert(res == VK_SUCCESS);

  /* Now present the image in the window */

  VkPresentInfoKHR present;
  present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  present.pNext = NULL;
  present.swapchainCount = 1;
  present.pSwapchains = &di.swap_chain;
  present.pImageIndices = &di.current_buffer;
  present.pWaitSemaphores = NULL;
  present.waitSemaphoreCount = 0;
  present.pResults = NULL;

  /* Make sure command buffer is finished before presenting */
  do {
    res = vkWaitForFences(di.device, 1, &drawFence, VK_TRUE, FENCE_TIMEOUT);
  } while (res == VK_TIMEOUT);

  assert(res == VK_SUCCESS);

  res = vkQueuePresentKHR(di.present_queue, &present);

  assert(res == VK_SUCCESS);
}

VkPipeline LamiaPipeline::GetPipeline(void)
{
  return pipe;
}
