
#pragma once
#include "..\..\include\physfs\physfs.h"
#include <map>
#include <string>
#include <vector>

struct FileBuffer
{
  char* fbuffer_;
};

class LamiaFile
{
  public:
    LamiaFile();
    ~LamiaFile();

    const char* GetFileData(const char* filename);

    std::map<std::string, char*> &GetBufferMap(void);
  private:
    std::map<std::string, char*> buffers_;
};

bool LamiaFileInit(void);
