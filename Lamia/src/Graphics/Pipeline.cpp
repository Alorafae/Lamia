
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
  descriptor_pool.maxSets = 1;
  descriptor_pool.poolSizeCount = textured ? 2 : 1;
  descriptor_pool.pPoolSizes = type_count;

  res = vkCreateDescriptorPool(di.device, &descriptor_pool, NULL, &descPool);
  assert(res == VK_SUCCESS);

  return res;
}

VkResult LamiaPipeline::CreateDescriptorSet(DeviceInfo & di, VkDescriptorBufferInfo bufInfo, VkDescriptorImageInfo imgInfo, bool textured)
{
  /* DEPENDS on init_descriptor_pool() */

  VkResult U_ASSERT_ONLY res;

  VkDescriptorSetAllocateInfo alloc_info[1];
  alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  alloc_info[0].pNext = NULL;
  alloc_info[0].descriptorPool = descPool;
  alloc_info[0].descriptorSetCount = NUM_DESCRIPTOR_SETS;
  alloc_info[0].pSetLayouts = descLayout.data();

  descSet.resize(NUM_DESCRIPTOR_SETS);
  res = vkAllocateDescriptorSets(di.device, alloc_info, descSet.data());
  assert(res == VK_SUCCESS);

  VkWriteDescriptorSet writes[2];

  // this is where the uniforms/samplers are bound to the pipeline
  writes[0] = {};
  writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  writes[0].pNext = NULL;
  writes[0].dstSet = descSet[0];
  writes[0].descriptorCount = 1;
  writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  // IMPORTANT THIS IS THE UNIFORM BUFFER DATA
  writes[0].pBufferInfo = &bufInfo;
  writes[0].dstArrayElement = 0;
  writes[0].dstBinding = 0;

  //texture sampler binding
  if (textured) {
    writes[1] = {};
    writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[1].dstSet = descSet[0];
    writes[1].dstBinding = 1;
    writes[1].descriptorCount = 1;
    writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writes[1].pImageInfo = &imgInfo;
    writes[1].dstArrayElement = 0;
  }

  vkUpdateDescriptorSets(di.device, textured ? 2 : 1, writes, 0, NULL);

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

VkResult LamiaPipeline::CreatePipeline(DeviceInfo & di, VkBool32 depth, VkBool32 vertexInput, VertexBufferInfo &VBI, VkPipelineShaderStageCreateInfo *ShdTechStages, bool textured)
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
  VkPipelineInputAssemblyStateCreateInfo ia;
  ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  ia.pNext = NULL;
  ia.flags = 0;
  ia.primitiveRestartEnable = VK_FALSE;
  ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

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
