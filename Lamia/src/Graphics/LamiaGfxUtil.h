
/*
 * Vulkan Samples
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "..\Window\LamiaWindow.h"



#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <vector>
#include <fstream>


#define GLM_FORCE_RADIANS
#include "..\..\include\glm\glm.hpp"
#include "..\..\include\glm\gtc\matrix_transform.hpp"
#include "..\..\include\glm\gtc\quaternion.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#define U_ASSERT_ONLY

#include "..\..\include\vulkan\vulkan.h"
#include "..\..\include\vulkan\vk_platform.h"

#include "..\..\Include\glslang\Include\Common.h"
#include "..\..\Include\glslang\Include\revision.h"
#include "..\..\Include\glslang\MachineIndependent\localintermediate.h"
#include "..\..\Include\glslang\MachineIndependent\SymbolTable.h"
#include "..\..\Include\SPIRV\GlslangToSpv.h"

#define MAX_APP_NAME_LEN 80
#define FRAME_TIME 0.0069444f // targeting 144 fps across all systems
#define NUM_SAMPLES VK_SAMPLE_COUNT_1_BIT
#define NUM_DESCRIPTOR_SETS 1
/* Number of viewports and number of scissors have to be the same */
/* at pipeline creation and in any call to set them dynamically   */
/* They also have to be the same as each other                    */
#define NUM_VIEWPORTS 1
#define NUM_SCISSORS NUM_VIEWPORTS

/* Amount of time, in nanoseconds, to wait for a command buffer to complete */
#define FENCE_TIMEOUT 100000000

#define DEFAULT_HEIGHT 720
#define DEFAULT_WIDTH 1280

static const char *vShdTxt =
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

static const char *fShdTxt =
"#version 400\n"
"#extension GL_ARB_separate_shader_objects : enable\n"
"#extension GL_ARB_shading_language_420pack : enable\n"
"layout (location = 0) in vec4 color;\n"
"layout (location = 0) out vec4 outColor;\n"
"void main() {\n"
"   outColor = color;\n"
"}\n";

// vertex data for a cube
#include "..\..\data\cube_data.h"



/*
* structure to track all objects related to a texture.
*/
struct texture_object {
  VkSampler sampler;

  VkImage image;
  VkImageLayout imageLayout;

  VkDeviceMemory mem;
  VkImageView view;
  int32_t tex_width, tex_height;
};

/*
* Keep each of our swap chain buffers' image, command buffer and view in one
* spot
*/
typedef struct _swap_chain_buffers {
  VkImage image;
  VkImageView view;
} swap_chain_buffer;

/*
* A layer can expose extensions, keep track of those
* extensions here.
*/
typedef struct {
  VkLayerProperties properties;
  std::vector<VkExtensionProperties> extensions;
} layer_properties;

struct DeviceInfo
{
  HINSTANCE connection;        // hInstance - Windows Instance
  wchar_t name[MAX_APP_NAME_LEN]; // Name to put on the window/icon
  HWND window;                 // hWnd - window handle

  VkSurfaceKHR surface;
  bool prepared;
  bool use_staging_buffer;
  bool save_images;

  std::vector<const char *> instance_layer_names;
  std::vector<const char *> instance_extension_names;
  std::vector<layer_properties> instance_layer_properties;
  std::vector<VkExtensionProperties> instance_extension_properties;
  VkInstance inst;

  std::vector<const char *> device_extension_names;
  std::vector<VkExtensionProperties> device_extension_properties;
  std::vector<VkPhysicalDevice> gpus;
  VkDevice device;
  VkQueue graphics_queue;
  VkQueue present_queue;
  uint32_t graphics_queue_family_index;
  uint32_t present_queue_family_index;
  VkPhysicalDeviceProperties gpu_props;
  std::vector<VkQueueFamilyProperties> queue_props;
  VkPhysicalDeviceMemoryProperties memory_properties;

  VkFramebuffer *framebuffers;
  int width, height;
  VkFormat format;

  uint32_t swapchainImageCount;
  VkSwapchainKHR swap_chain;
  std::vector<swap_chain_buffer> buffers;
  VkSemaphore imageAcquiredSemaphore;

  VkCommandPool cmd_pool;

  struct {
    VkFormat format;

    VkImage image;
    VkDeviceMemory mem;
    VkImageView view;
  } depth;

  std::vector<struct texture_object> textures;

  struct {
    VkBuffer buf;
    VkDeviceMemory mem;
    VkDescriptorBufferInfo buffer_info;
  } uniform_data;

  struct {
    VkDescriptorImageInfo image_info;
  } texture_data;
  VkDeviceMemory stagingMemory;
  VkImage stagingImage;

  VkCommandBuffer cmd; // Buffer for initialization commands

  VkRenderPass render_pass;

  PFN_vkCreateDebugReportCallbackEXT dbgCreateDebugReportCallback;
  PFN_vkDestroyDebugReportCallbackEXT dbgDestroyDebugReportCallback;
  PFN_vkDebugReportMessageEXT dbgBreakCallback;
  std::vector<VkDebugReportCallbackEXT> debug_report_callbacks;

  uint32_t current_buffer;
  uint32_t queue_family_count;
};

void LamiaMain(DeviceInfo &info);

bool memory_type_from_properties(DeviceInfo & info, uint32_t typeBits, VkFlags requirements_mask, uint32_t * typeIndex);
glm::mat4 GLMModelSRT(glm::vec3 scale, glm::vec3 const & Rotate, glm::vec3 const &Translate);

// startup functions
VkResult VK_Start_Sequence(DeviceInfo &info);

VkResult VK_Global_Ext_Props(layer_properties &layer_props); // done

VkResult VK_Global_Layer_Props(DeviceInfo &info); // done
VkResult VK_Instance_Ext_Names(DeviceInfo &info); // done
VkResult VK_Device_Ext_Names(DeviceInfo &info); // done
VkResult VK_Create_Instance(DeviceInfo &info); // done
VkResult VK_Enumerate_Device(DeviceInfo &info, uint32_t gpu_count = 1); // done

VkResult VK_Create_Window(DeviceInfo &info); // done

VkResult VK_Swapchain_Ext(DeviceInfo &info); // done
VkResult VK_Create_Device(DeviceInfo &info); // done

VkResult VK_Cmd_Pool(DeviceInfo &info);
VkResult VK_Cmd_Buffer(DeviceInfo &info);
VkResult VK_Exec_Cmd_Buffer(DeviceInfo &info);
VkResult VK_Device_Queue(DeviceInfo &info);
VkResult VK_Swapchain(DeviceInfo &info, VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT);
VkResult VK_Depth_Buffer(DeviceInfo &info); // done

VkResult VK_Renderpass(DeviceInfo &info, bool include_depth, bool clear = true, VkImageLayout finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

VkResult VK_Framebuffers(DeviceInfo &info, bool IncludeDepth);


VkResult VK_Shutdown_Sequence(DeviceInfo& info);

/*
vkDestroySemaphore(info.device, imageAcquiredSemaphore, NULL);
vkDestroyFence(info.device, drawFence, NULL);
destroy_pipeline(info);
destroy_pipeline_cache(info);
destroy_descriptor_pool(info);
destroy_vertex_buffer(info);
destroy_framebuffers(info);
destroy_shaders(info);
destroy_renderpass(info);
destroy_descriptor_and_pipeline_layouts(info);
destroy_uniform_buffer(info);
destroy_depth_buffer(info);
destroy_swap_chain(info);
destroy_command_buffer(info);
destroy_command_pool(info);
destroy_device(info);
destroy_window(info);
destroy_instance(info);
*/


