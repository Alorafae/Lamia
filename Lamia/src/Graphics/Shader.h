
#pragma once

#include "LamiaGfxUtil.h"

#define SHADER_STRAG_VERT 0
#define SHADER_STAGE_FRAG 1

//temp shader text until i write file IO
static const char *vShdTxt2 =
"#version 400\n"
"#extension GL_ARB_separate_shader_objects : enable\n"
"#extension GL_ARB_shading_language_420pack : enable\n"
"layout (std140, binding = 0) uniform bufferVals {\n"
"    mat4 mvp;\n"
"} myBufferVals;\n"
"layout (location = 0) in vec4 pos;\n"
"layout (location = 1) in vec4 inColor;\n"
"layout (location = 0) out vec4 outColor;\n"
"void main() {\n"
"   outColor = inColor;\n"
"   gl_Position = myBufferVals.mvp * pos;\n"
"}\n";

static const char *fShdTxt2 =
"#version 400\n"
"#extension GL_ARB_separate_shader_objects : enable\n"
"#extension GL_ARB_shading_language_420pack : enable\n"
"layout (location = 0) in vec4 color;\n"
"layout (location = 0) out vec4 outColor;\n"
"void main() {\n"
"   outColor = color;\n"
"}\n";

// shader utilities
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

    VkPipelineShaderStageCreateInfo* GetShaderStages(void);

  private:
    const char* vertName;
    const char* fragName;
    bool fileload = false;
    VkPipelineShaderStageCreateInfo ShdTech[2];
};
