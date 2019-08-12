
#include "LamiaFile.h"

static LamiaFile* g_LamiaFile;

LamiaFile::LamiaFile()
{
}

LamiaFile::~LamiaFile()
{
}

const char * LamiaFile::GetFileData(const char * filename)
{
  return nullptr;
}

bool LamiaFileInit(void)
{
  g_LamiaFile = new LamiaFile;

  return false;
}
