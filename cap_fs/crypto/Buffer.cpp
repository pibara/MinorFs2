#include "Buffer.hpp"
#include <stdlib.h>
#include <string.h>
namespace capfs {
  namespace crypto {
    Buffer::Buffer(size_t buffsize,char * const key, off_t ctr):mBuffer((char *)calloc(buffsize,1)),
             mBufferSize(buffsize),mEncSize(0),mFEK(key),mCtr(ctr){}
    Buffer::~Buffer(){
        free(mBuffer);
    }
    char * Buffer::encrypted() const {
        return mBuffer;
    }
    void Buffer::setencrypedsize(size_t sz) {
        mEncSize=sz;
    }
    size_t Buffer::size() const {
        return mBufferSize;
    }
    int Buffer::read(char *buf, size_t size) const {
        //FIXME, no real crypto yet
        memcpy(buf,mBuffer,size < mEncSize ? size : mEncSize);        
        return 0;
    }
    int Buffer::write(char const *buf, size_t size,off_t offset) {
        //FIXME, no real crypto yet
        memcpy(mBuffer+offset,buf,(mEncSize-offset) < size ? mEncSize-offset : size);
        return 0;
    }
  }
}
