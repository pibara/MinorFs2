#ifndef CAPFS_CRYPTO_METRICS
#define CAPFS_CRYPTO_METRICS
namespace capfs {
  namespace crypto {
    class Metrics {
        off_t offset;
        size_t size;
       public:
        Metrics(off_t o,size_t s);
        size_t presize();
        size_t postsize();
        off_t ctr();
    };
  }
}
#endif
