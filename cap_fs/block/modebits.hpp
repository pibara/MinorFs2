#ifndef MODEBITS_HPP
#define MODEBITS_HPP
#include <sys/types.h>
#include <sys/stat.h>
class modebits {
   mode_t mLlMode;
   mode_t mHlMode;
   size_t mPadLen;
  public:
   modebits(mode_t llmode);
   modebits(mode_t hlmode,size_t padlen);
   mode_t llmode();
   mode_t hlmode();
   size_t paddingsize();
};
#endif
