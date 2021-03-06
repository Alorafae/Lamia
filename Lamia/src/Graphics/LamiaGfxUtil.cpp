
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

#include "LamiaGfxUtil.h"
#include "Camera.h"
#include "Shader.h"
#include "Pipeline.h"
#include "Model.h"

#include "..\Systems\Systems.h"


VkResult VK_Create_Window(DeviceInfo &info)
{
  WNDCLASSEX win_class;
  assert(info.width > 0);
  assert(info.height > 0);

  info.connection = GetModuleHandle(NULL);
  swprintf_s(info.name, L"Lamia Engine 0.1"); //wide char cus we're using unicode settings in VS

  // Initialize the window class structure:
  win_class.cbSize = sizeof(WNDCLASSEX);
  win_class.style = CS_HREDRAW | CS_VREDRAW;
  win_class.lpfnWndProc = WndProc;
  win_class.cbClsExtra = 0;
  win_class.cbWndExtra = 0;
  win_class.hInstance = info.connection;  // hInstance
  win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
  win_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  win_class.lpszMenuName = NULL;
  win_class.lpszClassName = (LPCWSTR)info.name;
  win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
  // Register window class:
  if (!RegisterClassEx(&win_class))
  {
    // It didn't work, so try to give a useful error:
    printf("Unexpected error trying to start the application!\n");
    fflush(stdout);
    exit(1);
  }

  // we use these 2 values to center the window on the users screen
  int centerX = (GetSystemMetrics(SM_CXSCREEN) - info.width) / 2;
  int centerY = (GetSystemMetrics(SM_CYSCREEN) - info.height) / 2;

  // Create window with the registered class:
  RECT wr = { 0, 0, info.width, info.height };
  AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
  info.window = CreateWindowEx(0,
    (LPCWSTR)info.name,             // class name
    (LPCWSTR)info.name,             // app name
    WS_OVERLAPPEDWINDOW |  // window style
    WS_VISIBLE | WS_SYSMENU,
    centerX, centerY,            // x/y coords
    wr.right - wr.left,  // width
    wr.bottom - wr.top,  // height
    NULL,                // handle to parent
    NULL,                // handle to menu
    info.connection,     // hInstance
    NULL);               // no extra parameters
  if (!info.window)
  {
    // It didn't work, so try to give a useful error:
    printf("Cannot create a window in which to draw!\n");
    fflush(stdout);
    exit(1);
  }
  SetWindowLongPtr(info.window, GWLP_USERDATA, (LONG_PTR)&info);
  
  return VK_SUCCESS;
}


// testing global vars to make sure pipeline, shaders, model, camera work as intended

static Camera g_Camera;
static Model g_Cube;
static Shader g_ShdTech = Shader(vShdTxt2, fShdTxt2);
static LamiaPipeline g_Pipeline;

static Model g_OtherCube;

// attempting to make a grid from line list
// with a new pipeline that is set for lines
static Model g_Grid;
static LamiaPipeline g_LinePipeline;

static std::vector<glm::vec4> g_GridLinedata;

void CustomPipeInit(DeviceInfo &di)
{
  // intended way of using the systems
  // not sure if i want to add it to a namespace
  const char* filedata = LamiaSystems::LamiaSystem()->FileSystem()->GetFileData("test.txt");

  LAMIA_RESULT lr = LamiaSystems::LamiaSystem()->FileSystem()->ReleaseFileData("tes.txt");

  lr = LamiaSystems::LamiaSystem()->FileSystem()->ReleaseAllData();

  // fixed so this actually works and isn't null
  LamiaFile* lf = LamiaSystems::LamiaSystem()->FileSystem();

  // init our camera
  g_Camera.cPos = glm::vec3(-5, -3, -10);
  g_Camera.SetView();

  g_Camera.SetScissor(di);
  g_Camera.SetViewport(di);
  glm::mat4 model = glm::mat4(1.0f);

  glm::mat4 mvp = g_Camera.GetMVP();


  g_Cube.BindUBO(di, mvp);
  g_OtherCube.BindUBO(di, mvp);
  g_Grid.BindUBO(di, mvp);

  glm::vec4 gridstartbot = glm::vec4(-5.f, 0.f, -5.f, 1.f);
  glm::vec4 gridstarttop = glm::vec4(-5.f, 0.f, 5.f, 1.f);
  for (unsigned i = 0; i < 11; ++i)
  {
    g_GridLinedata.push_back(gridstartbot);
    g_GridLinedata.push_back(glm::vec4(.5f, .5f, .5f, 1.f));
    g_GridLinedata.push_back(gridstarttop);
    g_GridLinedata.push_back(glm::vec4(.5f, .5f, .5f, 1.f));
    gridstartbot.x += 1.f;
    gridstarttop.x += 1.f;    
  }

  glm::vec4 gridstartleft = glm::vec4(-5.f, 0.f, -5.f, 1.f);
  glm::vec4 gridstartright = glm::vec4(5.f, 0.f, -5.f, 1.f);
  for (unsigned i = 0; i < 11; ++i)
  {
    g_GridLinedata.push_back(gridstartleft);
    g_GridLinedata.push_back(glm::vec4(.5f, .5f, .5f, 1.f));
    g_GridLinedata.push_back(gridstartright);
    g_GridLinedata.push_back(glm::vec4(.5f, .5f, .5f, 1.f));
    gridstartleft.z += 1.f;
    gridstartright.z += 1.f;
  }

  // X orientation line
  g_GridLinedata.push_back(glm::vec4(-10.f, 0.f, 0.f, 1.f));
  g_GridLinedata.push_back(glm::vec4(1.f, 0.f, 0.f, 1.f));
  g_GridLinedata.push_back(glm::vec4(10.f, 0.f, 0.f, 1.f));
  g_GridLinedata.push_back(glm::vec4(1.f, 0.f, 0.f, 1.f));
  // Y orientation line
  g_GridLinedata.push_back(glm::vec4(0.f, -10.f, 0.f, 1.f));
  g_GridLinedata.push_back(glm::vec4(0.f, 1.f, 0.f, 1.f));
  g_GridLinedata.push_back(glm::vec4(0.f, 10.f, 0.f, 1.f));
  g_GridLinedata.push_back(glm::vec4(0.f, 1.f, 0.f, 1.f));
  // Z orientation line
  g_GridLinedata.push_back(glm::vec4(0.f, 0.f, -10.f, 1.f));
  g_GridLinedata.push_back(glm::vec4(0.f, 0.f, 1.f, 1.f));
  g_GridLinedata.push_back(glm::vec4(0.f, 0.f, 10.f, 1.f));
  g_GridLinedata.push_back(glm::vec4(0.f, 0.f, 1.f, 1.f));


  // init our shaders
  g_ShdTech.CreateShaderStages(di);


  // init our model
  g_Cube.CreateVertexBuffer(di, g_vb_solid_face_colors_Data, sizeof(g_vb_solid_face_colors_Data), sizeof(g_vb_solid_face_colors_Data[0]), false);
  g_Cube.pos = glm::vec3(0.0f);
  g_Cube.scale = glm::vec3(1.0f);
  g_Cube.rot = glm::vec3(0.0f);
  g_Cube.Update(FRAME_TIME);


  // second cube
  g_OtherCube.CreateVertexBuffer(di, g_vb_solid_face_colors_Data, sizeof(g_vb_solid_face_colors_Data), sizeof(g_vb_solid_face_colors_Data[0]), false);
  g_OtherCube.pos = glm::vec3(-5.0f, -1.f, -1.f);
  g_OtherCube.scale = glm::vec3(.5f);
  g_OtherCube.rot = glm::vec3(0.0f);
  g_OtherCube.Update(FRAME_TIME);

  // grid
  g_Grid.CreateVertexBuffer(di, g_GridLinedata.data(), uint32_t(g_GridLinedata.size()) * sizeof(glm::vec4), sizeof(glm::vec4) * 2, false);
  g_Grid.pos = glm::vec3(0.f, 0.f, 0.f);
  g_Grid.scale = glm::vec3(1.f);
  g_Grid.rot = glm::vec3(0.0f);
  g_Grid.Update(FRAME_TIME);

  VkDescriptorImageInfo imgInfo = VkDescriptorImageInfo();
  
  // init our pipeline
  g_Pipeline.CreateDescriptorPipelineLayout(di, false);
  g_Pipeline.CreateDescriptorPool(di, false);

  // init our grid pipeline
  g_LinePipeline.CreateDescriptorPipelineLayout(di, false);
  g_LinePipeline.CreateDescriptorPool(di, false);


  VkDescriptorPool dPool = g_Pipeline.GetDescPool();
  VkDescriptorSetLayout* dLayout = g_Pipeline.GetDescLayoutData();
  g_Cube.CreateDescriptorSet(di, dPool, dLayout, g_Cube.GetUBOInfo(), imgInfo, false);
  g_OtherCube.CreateDescriptorSet(di, dPool, dLayout, g_OtherCube.GetUBOInfo(), imgInfo, false);

  g_Pipeline.CreatePipelineCache(di);

  // grid pipeline
  VkDescriptorPool dPoolLine = g_LinePipeline.GetDescPool();
  VkDescriptorSetLayout* dLayoutLine = g_LinePipeline.GetDescLayoutData();
  g_Grid.CreateDescriptorSet(di, dPoolLine, dLayoutLine, g_Grid.GetUBOInfo(), imgInfo, false);
  g_LinePipeline.CreatePipelineCache(di);
  g_Grid.vCount = uint32_t(g_GridLinedata.size()) / 2;

  // dont like how this is set up & sent to pipeline creation
  // doesn't need to be per model
  // makes more sense to move to shader
  // since each shader would be concerned about vertex attributes
  // and models using same shader would use same v attribs
  VertexBufferInfo VBI;
  VBI.viBinds = g_Cube.GetVBinds();
  VBI.viAttribs[0] = g_Cube.GetVAtrribs(0);
  VBI.viAttribs[1] = g_Cube.GetVAtrribs(1);
  g_Pipeline.CreatePipeline(di, true, true, VBI, g_ShdTech.GetShaderStages(), false);


  VertexBufferInfo VBILine;
  VBILine.viBinds = g_Grid.GetVBinds();
  VBILine.viAttribs[0] = g_Grid.GetVAtrribs(0);
  VBILine.viAttribs[1] = g_Grid.GetVAtrribs(1);
  g_LinePipeline.CreatePipeline(di, true, true, VBILine, g_ShdTech.GetShaderStages(), false, VK_PRIMITIVE_TOPOLOGY_LINE_LIST);
}

// above function & globals to be removed after testing


// this will be the main render loop
void LamiaRender(DeviceInfo &info)
{
  g_Camera.SetView();
  glm::mat4 camMVP = g_Camera.GetMVP();
  // at this point all the startup has happened
  // vulkan, shaders and everything is initialized & compiled and ready to go

  //pipelines control the state(s) of rendering, including which shaders are used, etc

  //input
  g_Cube.rot += 25.f * FRAME_TIME;
  g_Cube.Update(FRAME_TIME);
  

  g_OtherCube.rot.y += 25.f * FRAME_TIME;
  g_OtherCube.Update(FRAME_TIME);

  g_Grid.Update(FRAME_TIME);
  //physics
  
  //sound
  
  //render
  FrameInfo fi;
  g_Pipeline.FrameStart(info, fi);

  
  g_Camera.SetViewport(info);
  g_Camera.SetScissor(info);


  g_Pipeline.BindDescriptorSet(info, g_Cube.GetDescriptorSetData());
  g_Cube.UpdateUniform(info, FRAME_TIME, camMVP * g_Cube.GetMatrix());
  g_Cube.Render(info, g_Camera);


  g_Pipeline.BindDescriptorSet(info, g_OtherCube.GetDescriptorSetData());
  g_OtherCube.UpdateUniform(info, FRAME_TIME, camMVP * g_OtherCube.GetMatrix());
  g_OtherCube.Render(info, g_Camera);


  vkCmdBindPipeline(info.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, g_LinePipeline.GetPipeline());
  g_LinePipeline.BindDescriptorSet(info, g_Grid.GetDescriptorSetData());
  g_Grid.UpdateUniform(info, FRAME_TIME, camMVP * g_Grid.GetMatrix());
  g_Grid.Render(info, g_Camera);

  // end of frame
  g_Pipeline.FrameEnd(info, fi);
}

// this all needs to be raw input later on, this is temporary
void ProcessInputMessage(WPARAM wParam)
{
  // this will be handled elsewhere obviously
  if (wParam == 0x57)
    g_Camera.cPos.z += 10.0f * FRAME_TIME;
  if (wParam == 0x44)
    g_Camera.cPos.x += 10.0f * FRAME_TIME;
  if (wParam == 0x41)
    g_Camera.cPos.x -= 10.0f * FRAME_TIME;
  if (wParam == VK_SPACE)
    g_Camera.cPos.y -= 10.0f * FRAME_TIME;
  if (wParam == 0x53)
    g_Camera.cPos.z -= 10.0f * FRAME_TIME;
  if (wParam == 0x43)
    g_Camera.cPos.y += 10.0f * FRAME_TIME;
    
  if (wParam == VK_ESCAPE)
    PostQuitMessage(0);
}


VkResult VK_Start_Sequence(DeviceInfo& info)
{
  info.width = DEFAULT_WIDTH;
  info.height = DEFAULT_HEIGHT;

  // init vulkan
  VK_Global_Layer_Props(info);
  VK_Instance_Ext_Names(info);
  VK_Device_Ext_Names(info);
  VK_Create_Instance(info);
  VK_Enumerate_Device(info);
  VK_Create_Window(info);
  VK_Swapchain_Ext(info);
  VK_Create_Device(info);

  // per framebuffer later on
  VK_Cmd_Pool(info);
  VK_Cmd_Buffer(info);

  // call before writing to cmd buffer
  //VK_Exec_Cmd_Buffer(info);

  // init vulkan
  VK_Device_Queue(info);
  VK_Swapchain(info);
  VK_Depth_Buffer(info);

  // init vulkan
  VK_Renderpass(info, true);

  // init vulkan
  VK_Framebuffers(info, true);


  // COOL ready to start rendering!!!

  CustomPipeInit(info);

  return VK_SUCCESS;
}

VkResult VK_Global_Ext_Props(layer_properties &layer_props)
{
  VkExtensionProperties *instance_extensions;
  uint32_t instance_extension_count;
  VkResult res;
  char *layer_name = NULL;

  layer_name = layer_props.properties.layerName;

  do {
    res = vkEnumerateInstanceExtensionProperties(layer_name, &instance_extension_count, NULL);
    if (res) return res;

    if (instance_extension_count == 0) {
      return VK_SUCCESS;
    }

    layer_props.extensions.resize(instance_extension_count);
    instance_extensions = layer_props.extensions.data();
    res = vkEnumerateInstanceExtensionProperties(layer_name, &instance_extension_count, instance_extensions);
  } while (res == VK_INCOMPLETE);

  return res;
}

VkResult VK_Global_Layer_Props(DeviceInfo &info)
{
  uint32_t instance_layer_count;
  VkLayerProperties *vk_props = NULL;
  VkResult res;

  /*
  * It's possible, though very rare, that the number of
  * instance layers could change. For example, installing something
  * could include new layers that the loader would pick up
  * between the initial query for the count and the
  * request for VkLayerProperties. The loader indicates that
  * by returning a VK_INCOMPLETE status and will update the
  * the count parameter.
  * The count parameter will be updated with the number of
  * entries loaded into the data pointer - in case the number
  * of layers went down or is smaller than the size given.
  */
  do {
    res = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
    if (res)
      return res;

    if (instance_layer_count == 0)
    {
      return VK_SUCCESS;
    }

    vk_props = (VkLayerProperties *)realloc(vk_props, instance_layer_count * sizeof(VkLayerProperties));

    res = vkEnumerateInstanceLayerProperties(&instance_layer_count, vk_props);
  } while (res == VK_INCOMPLETE);

  /*
  * Now gather the extension list for each instance layer.
  */
  for (uint32_t i = 0; i < instance_layer_count; i++)
  {
    layer_properties layer_props;
    layer_props.properties = vk_props[i];
    res = VK_Global_Ext_Props(layer_props);

    if (res)
      return res;

    info.instance_layer_properties.push_back(layer_props);
  }
  free(vk_props);

  return res;
}

VkResult VK_Instance_Ext_Names(DeviceInfo &info)
{
  info.instance_extension_names.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
  info.instance_extension_names.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

  return VK_SUCCESS;
}

VkResult VK_Device_Ext_Names(DeviceInfo &info)
{
  info.device_extension_names.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

  return VK_SUCCESS;
}

VkResult VK_Create_Instance(DeviceInfo &info)
{
  VkApplicationInfo app_info = {};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pNext = NULL;
  app_info.pApplicationName = "Alora's Vulkan";
  app_info.applicationVersion = 1;
  app_info.pEngineName = "Alora's Vulkan";
  app_info.engineVersion = 1;
  app_info.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo inst_info = {};
  inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  inst_info.pNext = NULL;
  inst_info.flags = 0;
  inst_info.pApplicationInfo = &app_info;
  inst_info.enabledLayerCount = (uint32_t)info.instance_layer_names.size();
  inst_info.ppEnabledLayerNames = info.instance_layer_names.size() ? info.instance_layer_names.data() : NULL; // change for readability
  inst_info.enabledExtensionCount = (uint32_t)info.instance_extension_names.size();
  inst_info.ppEnabledExtensionNames = info.instance_extension_names.data();

  VkResult res = vkCreateInstance(&inst_info, NULL, &info.inst);
  assert(res == VK_SUCCESS);

  return res;
}

VkResult VK_Enumerate_Device(DeviceInfo &info, uint32_t gpu_count)
{
  uint32_t const U_ASSERT_ONLY req_count = gpu_count;
  VkResult res = vkEnumeratePhysicalDevices(info.inst, &gpu_count, NULL);
  assert(gpu_count);
  info.gpus.resize(gpu_count);

  res = vkEnumeratePhysicalDevices(info.inst, &gpu_count, info.gpus.data());
  assert(!res && gpu_count >= req_count);

  vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, NULL);
  assert(info.queue_family_count >= 1);

  info.queue_props.resize(info.queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, info.queue_props.data());
  assert(info.queue_family_count >= 1);

  /* This is as good a place as any to do this */
  vkGetPhysicalDeviceMemoryProperties(info.gpus[0], &info.memory_properties);
  vkGetPhysicalDeviceProperties(info.gpus[0], &info.gpu_props);

  printf("Vulkan API Version: %u\n", info.gpu_props.apiVersion);
  printf("Vulkan Driver Version: %u\n", info.gpu_props.driverVersion);
  printf("GPU Vendor ID: %u\n", info.gpu_props.vendorID);
  printf("GPU Device ID: %u\n", info.gpu_props.deviceID);
  printf("GPU Device Name: %s\n", info.gpu_props.deviceName);

  printf("GPU Max Bound Descriptor Sets: %u\n", info.gpu_props.limits.maxBoundDescriptorSets);

  return res;
}

VkResult VK_Swapchain_Ext(DeviceInfo &info)
{
  /* DEPENDS on init_connection() and init_window() */

  VkResult U_ASSERT_ONLY res;

  // Construct the surface description:

  VkWin32SurfaceCreateInfoKHR createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  createInfo.pNext = NULL;
  createInfo.hinstance = info.connection;
  createInfo.hwnd = info.window;
  res = vkCreateWin32SurfaceKHR(info.inst, &createInfo, NULL, &info.surface);

  assert(res == VK_SUCCESS);

  // Iterate over each queue to learn whether it supports presenting:
  VkBool32 *pSupportsPresent = (VkBool32 *)malloc(info.queue_family_count * sizeof(VkBool32));
  for (uint32_t i = 0; i < info.queue_family_count; i++)
  {
    vkGetPhysicalDeviceSurfaceSupportKHR(info.gpus[0], i, info.surface, &pSupportsPresent[i]);
  }

  // Search for a graphics and a present queue in the array of queue
  // families, try to find one that supports both
  info.graphics_queue_family_index = UINT32_MAX;
  info.present_queue_family_index = UINT32_MAX;
  for (uint32_t i = 0; i < info.queue_family_count; ++i)
  {
    if ((info.queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
    {
      if (info.graphics_queue_family_index == UINT32_MAX) info.graphics_queue_family_index = i;

      if (pSupportsPresent[i] == VK_TRUE)
      {
        info.graphics_queue_family_index = i;
        info.present_queue_family_index = i;
        break;
      }
    }
  }

  if (info.present_queue_family_index == UINT32_MAX)
  {
    // If didn't find a queue that supports both graphics and present, then
    // find a separate present queue.
    for (uint32_t i = 0; i < info.queue_family_count; ++i)
      if (pSupportsPresent[i] == VK_TRUE)
      {
        info.present_queue_family_index = i;
        break;
      }
  }
  free(pSupportsPresent);

  // Generate error if could not find queues that support graphics
  // and present
  if (info.graphics_queue_family_index == UINT32_MAX || info.present_queue_family_index == UINT32_MAX)
  {
    printf("Could not find a queues for both graphics and present");
    exit(-1);
  }

  // Get the list of VkFormats that are supported:
  uint32_t formatCount;
  res = vkGetPhysicalDeviceSurfaceFormatsKHR(info.gpus[0], info.surface, &formatCount, NULL);
  assert(res == VK_SUCCESS);
  VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
  res = vkGetPhysicalDeviceSurfaceFormatsKHR(info.gpus[0], info.surface, &formatCount, surfFormats);
  assert(res == VK_SUCCESS);
  // If the format list includes just one entry of VK_FORMAT_UNDEFINED,
  // the surface has no preferred format.  Otherwise, at least one
  // supported format will be returned.
  if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED)
  {
    info.format = VK_FORMAT_B8G8R8A8_UNORM;
  }
  else
  {
    assert(formatCount >= 1);
    info.format = surfFormats[0].format;
  }
  free(surfFormats);

  return res;
}

VkResult VK_Create_Device(DeviceInfo &info)
{
  VkResult res;
  VkDeviceQueueCreateInfo queue_info = {};

  float queue_priorities[1] = { 0.0 };
  queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_info.pNext = NULL;
  queue_info.queueCount = 1;
  queue_info.pQueuePriorities = queue_priorities;
  queue_info.queueFamilyIndex = info.graphics_queue_family_index;

  VkDeviceCreateInfo device_info = {};
  device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_info.pNext = NULL;
  device_info.queueCreateInfoCount = 1;
  device_info.pQueueCreateInfos = &queue_info;
  device_info.enabledExtensionCount = (uint32_t)info.device_extension_names.size();
  device_info.ppEnabledExtensionNames = device_info.enabledExtensionCount ? info.device_extension_names.data() : NULL;
  device_info.pEnabledFeatures = NULL;

  res = vkCreateDevice(info.gpus[0], &device_info, NULL, &info.device);
  assert(res == VK_SUCCESS);

  return res;
}

VkResult VK_Cmd_Pool(DeviceInfo &info)
{
  /* DEPENDS on init_swapchain_extension() */
  VkResult U_ASSERT_ONLY res;

  VkCommandPoolCreateInfo cmd_pool_info = {};
  cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  cmd_pool_info.pNext = NULL;
  cmd_pool_info.queueFamilyIndex = info.graphics_queue_family_index;
  cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

  res = vkCreateCommandPool(info.device, &cmd_pool_info, NULL, &info.cmd_pool);
  assert(res == VK_SUCCESS);

  return res;
}

VkResult VK_Cmd_Buffer(DeviceInfo &info)
{
  /* DEPENDS on init_swapchain_extension() and init_command_pool() */
  VkResult U_ASSERT_ONLY res;

  VkCommandBufferAllocateInfo cmd = {};
  cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  cmd.pNext = NULL;
  cmd.commandPool = info.cmd_pool;
  cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  cmd.commandBufferCount = 1;

  res = vkAllocateCommandBuffers(info.device, &cmd, &info.cmd);
  assert(res == VK_SUCCESS);

  return res;
}

VkResult VK_Exec_Cmd_Buffer(DeviceInfo &info)
{
  /* DEPENDS on init_command_buffer() */
  VkResult U_ASSERT_ONLY res;

  VkCommandBufferBeginInfo cmd_buf_info = {};
  cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  cmd_buf_info.pNext = NULL;
  cmd_buf_info.flags = 0;
  cmd_buf_info.pInheritanceInfo = NULL;

  res = vkBeginCommandBuffer(info.cmd, &cmd_buf_info);
  assert(res == VK_SUCCESS);

  return res;
}

VkResult VK_Device_Queue(DeviceInfo &info)
{
  /* DEPENDS on init_swapchain_extension() */

  vkGetDeviceQueue(info.device, info.graphics_queue_family_index, 0, &info.graphics_queue);
  if (info.graphics_queue_family_index == info.present_queue_family_index)
  {
    info.present_queue = info.graphics_queue;
  }
  else
  {
    vkGetDeviceQueue(info.device, info.present_queue_family_index, 0, &info.present_queue);
  }

  return VK_SUCCESS;
}

VkResult VK_Swapchain(DeviceInfo &info, VkImageUsageFlags usageFlags)
{
  /* DEPENDS on info.cmd and info.queue initialized */

  VkResult U_ASSERT_ONLY res;
  VkSurfaceCapabilitiesKHR surfCapabilities;

  res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(info.gpus[0], info.surface, &surfCapabilities);
  assert(res == VK_SUCCESS);

  uint32_t presentModeCount;
  res = vkGetPhysicalDeviceSurfacePresentModesKHR(info.gpus[0], info.surface, &presentModeCount, NULL);
  assert(res == VK_SUCCESS);
  VkPresentModeKHR *presentModes = (VkPresentModeKHR *)malloc(presentModeCount * sizeof(VkPresentModeKHR));
  assert(presentModes);
  res = vkGetPhysicalDeviceSurfacePresentModesKHR(info.gpus[0], info.surface, &presentModeCount, presentModes);
  assert(res == VK_SUCCESS);

  VkExtent2D swapchainExtent;
  // width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
  if (surfCapabilities.currentExtent.width == 0xFFFFFFFF)
  {
    // If the surface size is undefined, the size is set to
    // the size of the images requested.
    swapchainExtent.width = info.width;
    swapchainExtent.height = info.height;
    if (swapchainExtent.width < surfCapabilities.minImageExtent.width)
    {
      swapchainExtent.width = surfCapabilities.minImageExtent.width;
    }
    else if (swapchainExtent.width > surfCapabilities.maxImageExtent.width)
    {
      swapchainExtent.width = surfCapabilities.maxImageExtent.width;
    }

    if (swapchainExtent.height < surfCapabilities.minImageExtent.height)
    {
      swapchainExtent.height = surfCapabilities.minImageExtent.height;
    }
    else if (swapchainExtent.height > surfCapabilities.maxImageExtent.height)
    {
      swapchainExtent.height = surfCapabilities.maxImageExtent.height;
    }
  }
  else
  {
    // If the surface size is defined, the swap chain size must match
    swapchainExtent = surfCapabilities.currentExtent;
  }

  // The FIFO present mode is guaranteed by the spec to be supported
  // Also note that current Android driver only supports FIFO
  VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

  // Determine the number of VkImage's to use in the swap chain.
  // We need to acquire only 1 presentable image at at time.
  // Asking for minImageCount images ensures that we can acquire
  // 1 presentable image as long as we present it before attempting
  // to acquire another.
  uint32_t desiredNumberOfSwapChainImages = surfCapabilities.minImageCount;

  VkSurfaceTransformFlagBitsKHR preTransform;
  if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
  {
    preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  }
  else
  {
    preTransform = surfCapabilities.currentTransform;
  }

  // Find a supported composite alpha mode - one of these is guaranteed to be set
  VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = {
    VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
    VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
    VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
  };

  for (uint32_t i = 0; i < sizeof(compositeAlphaFlags); i++)
  {
    if (surfCapabilities.supportedCompositeAlpha & compositeAlphaFlags[i])
    {
      compositeAlpha = compositeAlphaFlags[i];
      break;
    }
  }

  VkSwapchainCreateInfoKHR swapchain_ci = {};
  swapchain_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchain_ci.pNext = NULL;
  swapchain_ci.surface = info.surface;
  swapchain_ci.minImageCount = desiredNumberOfSwapChainImages;
  swapchain_ci.imageFormat = info.format;
  swapchain_ci.imageExtent.width = swapchainExtent.width;
  swapchain_ci.imageExtent.height = swapchainExtent.height;
  swapchain_ci.preTransform = preTransform;
  swapchain_ci.compositeAlpha = compositeAlpha;
  swapchain_ci.imageArrayLayers = 1;
  swapchain_ci.presentMode = swapchainPresentMode;
  swapchain_ci.oldSwapchain = VK_NULL_HANDLE;

  swapchain_ci.clipped = true;

  swapchain_ci.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
  swapchain_ci.imageUsage = usageFlags;
  swapchain_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swapchain_ci.queueFamilyIndexCount = 0;
  swapchain_ci.pQueueFamilyIndices = NULL;
  uint32_t queueFamilyIndices[2] = { (uint32_t)info.graphics_queue_family_index, (uint32_t)info.present_queue_family_index };
  if (info.graphics_queue_family_index != info.present_queue_family_index)
  {
    // If the graphics and present queues are from different queue families,
    // we either have to explicitly transfer ownership of images between the
    // queues, or we have to create the swapchain with imageSharingMode
    // as VK_SHARING_MODE_CONCURRENT
    swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swapchain_ci.queueFamilyIndexCount = 2;
    swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;
  }

  res = vkCreateSwapchainKHR(info.device, &swapchain_ci, NULL, &info.swap_chain);
  assert(res == VK_SUCCESS);

  res = vkGetSwapchainImagesKHR(info.device, info.swap_chain, &info.swapchainImageCount, NULL);
  assert(res == VK_SUCCESS);

  VkImage *swapchainImages = (VkImage *)malloc(info.swapchainImageCount * sizeof(VkImage));
  assert(swapchainImages);
  res = vkGetSwapchainImagesKHR(info.device, info.swap_chain, &info.swapchainImageCount, swapchainImages);
  assert(res == VK_SUCCESS);

  for (uint32_t i = 0; i < info.swapchainImageCount; i++)
  {
    swap_chain_buffer sc_buffer;

    VkImageViewCreateInfo color_image_view = {};
    color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    color_image_view.pNext = NULL;
    color_image_view.format = info.format;
    color_image_view.components.r = VK_COMPONENT_SWIZZLE_R;
    color_image_view.components.g = VK_COMPONENT_SWIZZLE_G;
    color_image_view.components.b = VK_COMPONENT_SWIZZLE_B;
    color_image_view.components.a = VK_COMPONENT_SWIZZLE_A;
    color_image_view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    color_image_view.subresourceRange.baseMipLevel = 0;
    color_image_view.subresourceRange.levelCount = 1;
    color_image_view.subresourceRange.baseArrayLayer = 0;
    color_image_view.subresourceRange.layerCount = 1;
    color_image_view.viewType = VK_IMAGE_VIEW_TYPE_2D;
    color_image_view.flags = 0;

    sc_buffer.image = swapchainImages[i];

    color_image_view.image = sc_buffer.image;

    res = vkCreateImageView(info.device, &color_image_view, NULL, &sc_buffer.view);
    info.buffers.push_back(sc_buffer);
    assert(res == VK_SUCCESS);
  }
  free(swapchainImages);
  info.current_buffer = 0;

  if (NULL != presentModes)
  {
    free(presentModes);
  }

  return res;
}

VkResult VK_Depth_Buffer(DeviceInfo &info)
{
  VkResult U_ASSERT_ONLY res;
  bool U_ASSERT_ONLY pass;
  VkImageCreateInfo image_info = {};

  /* allow custom depth formats */

  if (info.depth.format == VK_FORMAT_UNDEFINED)
    info.depth.format = VK_FORMAT_D16_UNORM;


  const VkFormat depth_format = info.depth.format;
  VkFormatProperties props;
  vkGetPhysicalDeviceFormatProperties(info.gpus[0], depth_format, &props);
  if (props.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
  {
    image_info.tiling = VK_IMAGE_TILING_LINEAR;
  }
  else if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
  {
    image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
  }
  else
  {
    /* Try other depth formats? */
    std::cout << "depth_format " << depth_format << " Unsupported.\n";
    exit(-1);
  }
  
  image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  image_info.pNext = NULL;
  image_info.imageType = VK_IMAGE_TYPE_2D;
  image_info.format = depth_format;
  image_info.extent.width = info.width;
  image_info.extent.height = info.height;
  image_info.extent.depth = 1;
  image_info.mipLevels = 1;
  image_info.arrayLayers = 1;
  image_info.samples = NUM_SAMPLES;
  image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  image_info.queueFamilyIndexCount = 0;
  image_info.pQueueFamilyIndices = NULL;
  image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  image_info.flags = 0;

  VkMemoryAllocateInfo mem_alloc = {};
  mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  mem_alloc.pNext = NULL;
  mem_alloc.allocationSize = 0;
  mem_alloc.memoryTypeIndex = 0;

  VkImageViewCreateInfo view_info = {};
  view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  view_info.pNext = NULL;
  view_info.image = VK_NULL_HANDLE;
  view_info.format = depth_format;
  view_info.components.r = VK_COMPONENT_SWIZZLE_R;
  view_info.components.g = VK_COMPONENT_SWIZZLE_G;
  view_info.components.b = VK_COMPONENT_SWIZZLE_B;
  view_info.components.a = VK_COMPONENT_SWIZZLE_A;
  view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  view_info.subresourceRange.baseMipLevel = 0;
  view_info.subresourceRange.levelCount = 1;
  view_info.subresourceRange.baseArrayLayer = 0;
  view_info.subresourceRange.layerCount = 1;
  view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  view_info.flags = 0;

  if (depth_format == VK_FORMAT_D16_UNORM_S8_UINT || depth_format == VK_FORMAT_D24_UNORM_S8_UINT || depth_format == VK_FORMAT_D32_SFLOAT_S8_UINT)
  {
    view_info.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
  }

  VkMemoryRequirements mem_reqs;

  /* Create image */
  res = vkCreateImage(info.device, &image_info, NULL, &info.depth.image);
  assert(res == VK_SUCCESS);

  vkGetImageMemoryRequirements(info.device, info.depth.image, &mem_reqs);

  mem_alloc.allocationSize = mem_reqs.size;
  /* Use the memory properties to determine the type of memory required */
  pass = memory_type_from_properties(info, mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mem_alloc.memoryTypeIndex);
  assert(pass);

  /* Allocate memory */
  res = vkAllocateMemory(info.device, &mem_alloc, NULL, &info.depth.mem);
  assert(res == VK_SUCCESS);

  /* Bind memory */
  res = vkBindImageMemory(info.device, info.depth.image, info.depth.mem, 0);
  assert(res == VK_SUCCESS);

  /* Create image view */
  view_info.image = info.depth.image;
  res = vkCreateImageView(info.device, &view_info, NULL, &info.depth.view);
  assert(res == VK_SUCCESS);

  return res;
}

VkResult VK_Renderpass(DeviceInfo & info, bool include_depth, bool clear, VkImageLayout finalLayout)
{
  /* DEPENDS on init_swap_chain() and init_depth_buffer() */

  VkResult U_ASSERT_ONLY res;
  /* Need attachments for render target and depth buffer */
  VkAttachmentDescription attachments[2];
  attachments[0].format = info.format;
  attachments[0].samples = NUM_SAMPLES;
  attachments[0].loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
  attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  attachments[0].finalLayout = finalLayout;
  attachments[0].flags = 0;

  if (include_depth) {
    attachments[1].format = info.depth.format;
    attachments[1].samples = NUM_SAMPLES;
    attachments[1].loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachments[1].flags = 0;
  }

  VkAttachmentReference color_reference = {};
  color_reference.attachment = 0;
  color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depth_reference = {};
  depth_reference.attachment = 1;
  depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass = {};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.flags = 0;
  subpass.inputAttachmentCount = 0;
  subpass.pInputAttachments = NULL;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_reference;
  subpass.pResolveAttachments = NULL;
  subpass.pDepthStencilAttachment = include_depth ? &depth_reference : NULL;
  subpass.preserveAttachmentCount = 0;
  subpass.pPreserveAttachments = NULL;

  VkRenderPassCreateInfo rp_info = {};
  rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  rp_info.pNext = NULL;
  rp_info.attachmentCount = include_depth ? 2 : 1;
  rp_info.pAttachments = attachments;
  rp_info.subpassCount = 1;
  rp_info.pSubpasses = &subpass;
  rp_info.dependencyCount = 0;
  rp_info.pDependencies = NULL;

  res = vkCreateRenderPass(info.device, &rp_info, NULL, &info.render_pass);
  assert(res == VK_SUCCESS);

  return res;
}

VkResult VK_Framebuffers(DeviceInfo & info, bool IncludeDepth)
{
  /* DEPENDS on init_depth_buffer(), init_renderpass() and
  * init_swapchain_extension() */

  VkResult U_ASSERT_ONLY res;
  VkImageView attachments[2];
  attachments[1] = info.depth.view;

  VkFramebufferCreateInfo fb_info = {};
  fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  fb_info.pNext = NULL;
  fb_info.renderPass = info.render_pass;
  fb_info.attachmentCount = IncludeDepth ? 2 : 1;
  fb_info.pAttachments = attachments;
  fb_info.width = info.width;
  fb_info.height = info.height;
  fb_info.layers = 1;

  uint32_t i;

  info.framebuffers = (VkFramebuffer *)malloc(info.swapchainImageCount * sizeof(VkFramebuffer));

  for (i = 0; i < info.swapchainImageCount; i++) {
    attachments[0] = info.buffers[i].view;
    res = vkCreateFramebuffer(info.device, &fb_info, NULL, &info.framebuffers[i]);
    assert(res == VK_SUCCESS);
  }

  return res;
}

VkResult VK_Shutdown_Sequence(DeviceInfo &info)
{
  return VK_SUCCESS;
}

bool memory_type_from_properties(DeviceInfo & info, uint32_t typeBits, VkFlags requirements_mask, uint32_t * typeIndex)
{
  // Search memtypes to find first index with those properties
  for (uint32_t i = 0; i < info.memory_properties.memoryTypeCount; i++) {
    if ((typeBits & 1) == 1) {
      // Type is available, does it match user properties?
      if ((info.memory_properties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask) {
        *typeIndex = i;
        return true;
      }
    }
    typeBits >>= 1;
  }
  // No memory types matched, return failure
  return false;
}

glm::mat4 GLMModelSRT(glm::vec3 scale, glm::vec3 const & Rotate, glm::vec3 const &Translate)
{
  glm::mat4 Model, S, R, T = glm::mat4(1.0f);
  // this is per model
  S = glm::scale(glm::mat4(1.0f), scale);

  R = glm::rotate(R, glm::radians(Rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
  R = glm::rotate(R, glm::radians(Rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
  R = glm::rotate(R, glm::radians(Rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

  T = glm::translate(T, Translate);

  return T * R * S;
}
