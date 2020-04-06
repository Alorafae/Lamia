
#pragma once
#include "..\..\include\physfs\physfs.h"
#include <map>
#include <string>

enum LAMIA_RESULT;

class LamiaFile
{
  public:
    LamiaFile();
    ~LamiaFile();

    const char* GetFileData(const char* filename);
    LAMIA_RESULT ReleaseFileData(const char* filename);
    LAMIA_RESULT ReleaseAllData(void);

    std::map<std::string, char*> &GetBufferMap(void);
  private:
    std::map<std::string, char*> buffers_;
};

bool LamiaFileInit(void);
LamiaFile* LamiaFileGetSystem(void);
