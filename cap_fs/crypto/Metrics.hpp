#ifndef CAPFS_CRYPTO_METRICS
#define CAPFS_CRYPTO_METRICS
#include <sys/types.h>
#include <unistd.h>
namespace capfs {
  namespace crypto {
    class Metrics {
        size_t const mPreSize;
        size_t const mPostSize;
        off_t const mCtr;
       public:
        Metrics(off_t o,size_t s);
        size_t presize();
        size_t postsize();
        off_t ctr();
    };
  }
}
#endif
