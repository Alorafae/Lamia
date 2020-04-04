
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
  PHYSFS_file *file = PHYSFS_openRead(filename);

  if (file == NULL)
    return NULL;

  PHYSFS_Stat *fstats = new PHYSFS_Stat();
  PHYSFS_stat(filename, fstats);

  char* fbuffer = new char[fstats->filesize]; // needs to be tracked and deleted
  memset(fbuffer, 0, fstats->filesize);

  PHYSFS_readBytes(file, fbuffer, fstats->filesize);

  FileBuffer* fb = new FileBuffer();
  fb->fbuffer_ = fbuffer;

  g_LamiaFile->GetBufferMap()[filename] = fbuffer;

  return fbuffer;
}

std::map<std::string, char*> &LamiaFile::GetBufferMap(void)
{
  return buffers_;
}

bool LamiaFileInit(void)
{
  g_LamiaFile = new LamiaFile;
  int ret = PHYSFS_init(NULL);

  // physfs error handling


  // set up physfs
  const char* baseDir = PHYSFS_getBaseDir();

  ret = PHYSFS_setWriteDir(baseDir);
  ret = PHYSFS_mount(baseDir, "", 1);

  const char* writeDir = PHYSFS_getWriteDir();
  // things are fine return
  return (bool)ret;
}
