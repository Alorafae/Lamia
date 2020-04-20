
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
  // check our map if we already have it loaded so we don't read it in again


  // open the file for reading
  PHYSFS_file *file = PHYSFS_openRead(filename);

  if (file == NULL)
    return NULL;

  PHYSFS_Stat *fstats = new PHYSFS_Stat();
  PHYSFS_stat(filename, fstats);

  char* fbuffer = new char[fstats->filesize]; // needs to be tracked and deleted
  memset(fbuffer, 0, fstats->filesize);

  PHYSFS_readBytes(file, fbuffer, fstats->filesize);

  g_LamiaFile->GetBufferMap()[filename] = fbuffer;

  return fbuffer;
}

LAMIA_RESULT LamiaFile::ReleaseFileData(const char * filename)
{
  // find our data by key
  auto iter = g_LamiaFile->GetBufferMap().find(filename);

  // check that it exists, if end return out with error
  if (iter == g_LamiaFile->GetBufferMap().end())
    return LAMIA_E_FILE_NOT_FOUND;

  if (iter->second != NULL)
  {
    // free the array memory
    delete[] iter->second;
  }
  else
    return LAMIA_E_FILE_DBL_FREE;

  // remove the key from the map not needed with ReleaseAllData added
  //g_LamiaFile->GetBufferMap().erase(filename);
  iter->second = NULL;

  return LAMIA_E_SUCCESS;
}

LAMIA_RESULT LamiaFile::ReleaseAllData(void)
{
  // iterate through each existing key
  for (auto iter = g_LamiaFile->GetBufferMap().begin(); iter != g_LamiaFile->GetBufferMap().end(); ++iter)
  {
    ReleaseFileData(iter->first.c_str());
  }

  // clear the whole map
  g_LamiaFile->GetBufferMap().clear();

  return LAMIA_E_SUCCESS;
}

std::map<std::string, char*> &LamiaFile::GetBufferMap(void)
{
  // return a reference to our map of buffers
  return buffers_;
}

bool LamiaFileInit(void)
{
  g_LamiaFile = new LamiaFile();

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

LamiaFile * LamiaFileGetSystem(void)
{
  return g_LamiaFile;
}
