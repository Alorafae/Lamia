
#include "Model.h"
#include "..\Systems\Systems.h"

Model::Model()
{
  LamiaSystems::LamiaSystem();
}

Model::~Model()
{
}

VkResult Model::CreateVertexBuffer(DeviceInfo & di, const void * vertexData, uint32_t dataSize, uint32_t dataStride, bool textured)
{
  VkResult U_ASSERT_ONLY res;
  bool U_ASSERT_ONLY pass;

  VkBufferCreateInfo buf_info = {};
  buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buf_info.pNext = NULL;
  buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  buf_info.size = dataSize;
  buf_info.queueFamilyIndexCount = 0;
  buf_info.pQueueFamilyIndices = NULL;
  buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  buf_info.flags = 0;
  res = vkCreateBuffer(di.device, &buf_info, NULL, &vData.buf);
  assert(res == VK_SUCCESS);

  VkMemoryRequirements mem_reqs;
  vkGetBufferMemoryRequirements(di.device, vData.buf, &mem_reqs);

  VkMemoryAllocateInfo alloc_info = {};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.pNext = NULL;
  alloc_info.memoryTypeIndex = 0;

  alloc_info.allocationSize = mem_reqs.size;
  pass = memory_type_from_properties(di, mem_reqs.memoryTypeBits,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    &alloc_info.memoryTypeIndex);
  assert(pass && "No mappable, coherent memory");

  res = vkAllocateMemory(di.device, &alloc_info, NULL, &(vData.mem));
  assert(res == VK_SUCCESS);
  vData.info.range = mem_reqs.size;
  vData.info.offset = 0;

  uint8_t *pData;
  res = vkMapMemory(di.device, vData.mem, 0, mem_reqs.size, 0, (void **)&pData);
  assert(res == VK_SUCCESS);

  memcpy(pData, vertexData, dataSize);

  vkUnmapMemory(di.device, vData.mem);

  res = vkBindBufferMemory(di.device, vData.buf, vData.mem, 0);
  assert(res == VK_SUCCESS);

  viBinds.binding = 0;
  viBinds.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  viBinds.stride = dataStride;

  viAttribs[0].binding = 0;
  viAttribs[0].location = 0;
  viAttribs[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
  viAttribs[0].offset = 0;
  viAttribs[1].binding = 0;
  viAttribs[1].location = 1;
  viAttribs[1].format = textured ? VK_FORMAT_R32G32_SFLOAT : VK_FORMAT_R32G32B32A32_SFLOAT;
  viAttribs[1].offset = 16;

  return res;
}

VkVertexInputBindingDescription& Model::GetVBinds(void)
{
  return viBinds;
}

VkVertexInputAttributeDescription& Model::GetVAtrribs(unsigned index)
{
  return viAttribs[index];
}

VkBuffer Model::GetVBuffer(void)
{
  return vData.buf;
}

void Model::Update(float dt)
{
  model = GLMModelSRT(scale, rot, pos);
}

void Model::Render(DeviceInfo & di, Camera & cam)
{
  const VkDeviceSize offsets[1] = { 0 };
  vkCmdBindVertexBuffers(di.cmd, 0, 1, &vData.buf, offsets);

  //cam.SetViewport(di);
  //cam.SetScissor(di);

  vkCmdDraw(di.cmd, vCount, 1, 0, 0);
}

void Model::UpdateUniform(DeviceInfo & di, float dt, glm::mat4 mvp)
{
  VkResult U_ASSERT_ONLY res;

  VkMemoryRequirements mr;
  vkGetBufferMemoryRequirements(di.device, UBOBuffer, &mr);
  // map memory may need to be different or not needed
  uint8_t *pData;
  res = vkMapMemory(di.device, VKD_Mem, 0, mr.size, 0, (void **)&pData);
  assert(res == VK_SUCCESS);

  memcpy(pData, &mvp, sizeof(mvp));

  vkUnmapMemory(di.device, VKD_Mem);

  res = vkBindBufferMemory(di.device, UBOBuffer, VKD_Mem, 0);
  assert(res == VK_SUCCESS);
}

void Model::BindUBO(DeviceInfo & di, glm::mat4 & mvp)
{
  VkBufferCreateInfo bi = {};
  bi.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bi.pNext = NULL;
  bi.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  bi.size = sizeof(mvp); // will fix magic number here later
  // need to make the UBO binding more dynamic 
  // maybe with type info / meta data
  bi.queueFamilyIndexCount = 0;
  bi.pQueueFamilyIndices = NULL;
  bi.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  bi.flags = 0;
  VkResult U_ASSERT_ONLY res = vkCreateBuffer(di.device, &bi, NULL, &UBOBuffer);
  assert(res == VK_SUCCESS);

  VkMemoryRequirements mr;
  vkGetBufferMemoryRequirements(di.device, UBOBuffer, &mr);

  VkMemoryAllocateInfo ai = {};
  ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  ai.pNext = NULL;
  ai.memoryTypeIndex = 0;

  ai.allocationSize = mr.size;
  bool U_ASSERT_ONLY pass = memory_type_from_properties(di, mr.memoryTypeBits,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    &ai.memoryTypeIndex);
  assert(pass && "No mappable, coherent memory");

  res = vkAllocateMemory(di.device, &ai, NULL, &(VKD_Mem));
  assert(res == VK_SUCCESS);

  // map memory may need to be different or not needed
  uint8_t *pData;
  res = vkMapMemory(di.device, VKD_Mem, 0, mr.size, 0, (void **)&pData);
  assert(res == VK_SUCCESS);

  memcpy(pData, &mvp, sizeof(mvp));

  vkUnmapMemory(di.device, VKD_Mem);

  res = vkBindBufferMemory(di.device, UBOBuffer, VKD_Mem, 0);
  assert(res == VK_SUCCESS);

  UBufferInfo.buffer = UBOBuffer;
  UBufferInfo.offset = 0;
  UBufferInfo.range = sizeof(mvp);
}

VkDescriptorBufferInfo Model::GetUBOInfo(void)
{
  return UBufferInfo;
}

VkResult Model::CreateDescriptorSet(DeviceInfo & di, VkDescriptorPool &descPool, VkDescriptorSetLayout *descLayoutData, VkDescriptorBufferInfo bufInfo, VkDescriptorImageInfo imgInfo, bool textured)
{
  /* DEPENDS on init_descriptor_pool() */

  VkResult U_ASSERT_ONLY res;

  VkDescriptorSetAllocateInfo alloc_info[1];
  alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  alloc_info[0].pNext = NULL;
  alloc_info[0].descriptorPool = descPool;
  alloc_info[0].descriptorSetCount = NUM_DESCRIPTOR_SETS;
  alloc_info[0].pSetLayouts = descLayoutData;

  descSet.resize(NUM_DESCRIPTOR_SETS);
  res = vkAllocateDescriptorSets(di.device, alloc_info, descSet.data());
  assert(res == VK_SUCCESS);

  // number of writes would equal numbers of uniforms + number of samplers
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

VkDescriptorSet * Model::GetDescriptorSetData(void)
{
  return descSet.data();
}

glm::mat4 Model::GetMatrix(void)
{
  return model;
}
