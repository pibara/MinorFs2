#ifndef MODEBITS_HPP
#define MODEBITS_HPP
#include <sys/types.h>
#include <sys/stat.h>
typedef enum {nodetype_unknown,data_file,exe_file,directory,symbolic_link} node_type;
class modebits {
   mode_t mLlMode;
   mode_t mHlMode;
   size_t mPadLen;
  public:
   modebits(mode_t llmode);
   modebits(mode_t hlmode,size_t padlen);
   modebits(node_type typ);
   mode_t llmode();
   mode_t hlmode();
   size_t paddingsize();
   void updatepadding(size_t padlen);
};
#endif
