
#include "Model.h"

Model::Model()
{
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
