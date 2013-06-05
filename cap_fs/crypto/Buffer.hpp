#ifndef CAPFS_CRYPTO_BUFFER
#define CAPFS_CRYPTO_BUFFER
#include <sys/types.h>
#include <unistd.h>
namespace capfs {
  namespace crypto {
    class Buffer {
        char * const mBuffer;
        size_t const mBufferSize;
        size_t mEncSize;
        char * const mFEK;
        off_t const mCtr;
        Buffer(Buffer const &);
        Buffer &operator=(Buffer const &);
       public:
        Buffer(size_t buffsize,char * const key, off_t ctr);
        ~Buffer();
        char * encrypted() const;
        void setencrypedsize(size_t sz);
        size_t size() const;
        int read(char *buf, size_t size) const;
        int write(char const *buf, size_t size,off_t offset);
    };
  }
}
#endif
