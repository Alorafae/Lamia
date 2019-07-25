
#pragma once

#include "LamiaGfxUtil.h"

#define SHADER_STRAG_VERT 0
#define SHADER_STAGE_FRAG 1

// shader utilities
bool memory_type_from_properties(DeviceInfo & info, uint32_t typeBits, VkFlags requirements_mask, uint32_t * typeIndex);
bool GLSLtoSPV(const VkShaderStageFlagBits shader_type, const char *pshader, std::vector<unsigned int> &spirv);
void VK_Shader_Init_Resources(TBuiltInResource &Resources);
EShLanguage FindLanguage(const VkShaderStageFlagBits shader_type);

const char* ShaderFromFile(const char* file);

class Shader
{
  public:
    Shader();
    Shader(const char* vertFileName, const char* fragFileName, bool loadfromfile = false);
    ~Shader();

    VkResult CreateShaderStages(DeviceInfo &di);

  private:
    const char* vertName;
    const char* fragName;
    VkPipelineShaderStageCreateInfo ShdTech[2];
};
