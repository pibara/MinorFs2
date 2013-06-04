#ifndef CAPFS_CRYPTO_BUFFER
#define CAPFS_CRYPTO_BUFFER
namespace capfs {
  namespace crypto {
    class Buffer {
        char *mBuffer;
        size_t mBufferSize;
        char *mFEK;
        off_t mCtr;
        Buffer(Buffer const &);
        Buffer &operator=(Buffer const &);
       public:
        Buffer(size_t buffsize,char * key, off_t ctr);
        ~Buffer();
        char * encrypted();
        void setencrypedsize(size_t sz);
        size_t size();
        int read(char *buf, size_t size);
        int write(char const *buf, size_t size,off_t offset);
    };
  }
}
#endif
