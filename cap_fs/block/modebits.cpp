#include "modebits.hpp"
/*Constructor for determining high level mode bits and padding size from mode returned by low level stat*/
/*Note: will try to patch insecure 'other'bits too.*/
modebits::modebits(mode_t llmode):mLlMode(llmode & ^(S_IROTH|S_IWOTH|S_IXOTH)), //clear 'other' bits JIC
                                  mHlMode(0), 
                                  mPadlen(0) {
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
  /*Calculate pad length from mode bits*/
  if (llmod & S_ISGID) mPadlen+= 8;
  if (llmod & S_IRGRP) mPadlen+= 4;
  if (llmod & S_IWGRP) mPadlen+= 2;
  if (llmod & S_IXGRP) mPadlen+= 1;   
}

/*Constructor fo determining low level mode when we are passed a high level mode from 
  a fuse function that needs to be translated to a low level mode.
  We also need the padd length to determine the low level mode.
  Note, this constructor will also update the given high level mode to one appropriate.
*/
modebits::modebits(mode_t hlmode,size_t padlen):mLlMode(S_IFREG|S_IRUSR|S_IWUSR), //Common ll mode bits
      mHlMode((S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)&^(S_IXUSR|S_IXGRP|S_IXOTH)),//and hl too.
      mPadlen(padlen) { 
   //Encode padding size in apropriate mode bits.
   if (mPadlen & 8) mLlMode |= S_ISGID;
   if (mPadlen & 4) mLlMode |= S_IRGRP;
   if (mPadlen & 2) mLlMode |= S_IWGRP;
   if (mPadlen & 1) mLlMode |= S_IXGRP;
   
   if (S_ISDIR(hlmode)) { 
      //Set propper low level mode bits to indicate directory data.
      mLlMode |= S_ISUID | S_IXUSR;
      //Set high level ode bits appropriate for directory representation.
      mHlMode |= S_IFDIR | S_IXUSR | S_IXGRP | S_IXOTH;
   }
   else if (S_ISLNK(hlmode)){
      //Set propper low level mode bits to indicate symbolic link.
      mLlMode |= S_ISUID;
      //Set high level ode bits appropriate for symbolic link representation.
      mHlMode |= S_IFLNK | S_IXUSR | S_IXGRP | S_IXOTH;
   }
   else if (S_ISREG(hlmode) && (hlmode & S_IXUSR)) {
      //Set propper low level mode bits to indicate  an executable regular file.
      mLlMode |= S_IXUSR;
      //Set high level ode bits appropriate for executable regular file.
      mHlMode |= S_IFREG | S_IXUSR | S_IXGRP | S_IXOTH;  
   } else {
      //Set high level ode bits appropriate for non-executable regular file.
      mHlMode |= S_IFREG;
   }
}

/*Convenience constructor for use when creating a new known node type with zero length*/
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

/*Update the mode if the paddlengt changes.*/
void updatepadding(size_t padlen) {
   mPadLen=padlen;
   mLlMode &= ~(S_ISGID|S_IRGRP|S_IWGRP|S_IXGRP);
   if (mPadlen & 8) mLlMode |= S_ISGID;
   if (mPadlen & 4) mLlMode |= S_IRGRP;
   if (mPadlen & 2) mLlMode |= S_IWGRP;
   if (mPadlen & 1) mLlMode |= S_IXGRP;
}
