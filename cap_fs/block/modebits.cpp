#include "modebits.hpp"

modebits::modebits(mode_t llmode):mLlMode(llmode),mHlMode(0),mPadlen(0) {
  switch(llmode & (S_ISUID | S_IXUSR)) {
    case 0: /*regular file*/
         mHlMod = S_IFREG | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
         break;
    case S_IXUSR: /*exe file */
         mHlMode = S_IFREG|S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH;
         break;
    case S_ISUID: /*symlink */
         mHlMode = S_IFLNK|S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH;
         break;
    case (S_ISUID | S_IXUSR): /* dir */
         mHlMode = S_IFDIR|S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH;
         break;
  }
  if (llmod & S_ISGID) mPadlen+= 8;
  if (llmod & S_IRGRP) mPadlen+= 4;
  if (llmod & S_IWGRP) mPadlen+= 2;
  if (llmod & S_IXGRP) mPadlen+= 1;   
}

modebits::modebits(mode_t hlmode,size_t padlen):mLlMode(S_IFREG|S_IRUSR|S_IWUSR),
                                                mHlMode(hlmode),
                                                mPadlen(padlen) {
   if (mPadlen & 8) mLlMode |= S_ISGID;
   if (mPadlen & 4) mLlMode |= S_IRGRP;
   if (mPadlen & 2) mLlMode |= S_IWGRP;
   if (mPadlen & 1) mLlMode |= S_IXGRP;
   if (S_ISDIR(mHlMode))  mLlMode |= S_ISUID | S_IXUSR;
   if (S_ISLNK(mHlMode))  mLlMode |= S_ISUID;
   if (S_ISREG(mHlMode) && (mHlMode & S_IXUSR)) mLlMode |= S_IXUSR;
}

modebits(node_type typ):mLlMode(S_IFREG|S_IRUSR|S_IWUSR),
                      mHlMode(S_IFREG | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH),
                      mPadlen(0) {
  switch(typ) {
    case exe_file:
        mLlMode |= S_IXUSR;
        mHlMode |= S_IXUSR | S_IXGRP | S_IXOTH;
        break;
    case directory:
        mLlMode |= S_ISUID | S_IXUSR;
        mHlMode |= S_IFDIR | S_IXUSR | S_IXGRP | S_IXOTH;
        break;
    case symbolic_link:
        mLlMode |= S_IXUSR;
        mHlMode |= S_IFLNK | S_IXUSR | S_IXGRP | S_IXOTH;
  }
}

mode_t modebits::llmode() {
  return mLlMode;
}

mode_t modebits::hlmode() {
  return mHlMode;
}

size_t modebits::paddingsize() {
  return mPadLen;
}

void updatepadding(size_t padlen) {
   mPadLen=padlen;
   mLlMode &= ~(S_ISGID|S_IRGRP|S_IWGRP|S_IXGRP);
   if (mPadlen & 8) mLlMode |= S_ISGID;
   if (mPadlen & 4) mLlMode |= S_IRGRP;
   if (mPadlen & 2) mLlMode |= S_IWGRP;
   if (mPadlen & 1) mLlMode |= S_IXGRP;
}
