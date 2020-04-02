
#include "LamiaFile.h"
#include "Systems.h"

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
  int ret = PHYSFS_init(NULL);

  // physfs error handling


  // set up physfs
  const char* baseDir = PHYSFS_getBaseDir();

  ret = PHYSFS_setWriteDir(baseDir);

  const char* writeDir = PHYSFS_getWriteDir();
  // things are fine return
  return (bool)ret;
}
