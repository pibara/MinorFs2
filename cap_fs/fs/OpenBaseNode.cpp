#include "OpenBaseNode.hpp"
#include "../crypto/Buffer.hpp"
#include "../crypto/Metrics.hpp"
#include <errno.h>
#include <unistd.h>

namespace capfs {
namespace fs {
OpenBaseNode::OpenBaseNode(uint64_t fh,
                           OpenFileHandleCache & fhc, 
                           std::map<uint64_t,OpenBaseNode> & opennodes):
                      mFh(fh),mFhc(fhc),mOpennodes(opennodes){
}
int OpenBaseNode::release(){ 
  if (mFh == 0) { 
    return -EBADF;
  }
  mOpennodes.erase(mFh);
  mFh = 0;
  return 0;
}
int OpenBaseNode::releasedir(){ 
  if (mFh == 0) {
    return -EBADF;
  }
  mOpennodes.erase(mFh);
  mFh = 0;
  return 0;
}
int OpenBaseNode::read(char *b, size_t s, off_t o){ 
  if (mFh ==0) {
    return -EBADF;
  }
  //FIXME: function needs error handling.
  //Use a metrics helper for working with crypto blocks.
  capfs::crypto::Metrics metrics(o,s);
  size_t rawpresize = metrics.presize(); //o % 16;
  size_t rawpostsize = metrics.postsize(); //15 - ((o+s+15)%16);
  size_t rawsize = s + rawpresize + rawpostsize;
  off_t rawoffset = o - rawpresize;
  //We use a crypto buffer that implements the actual crypto,
  capfs::crypto::Buffer buffer(rawsize,mFek,metrics.ctr()); 
  //Read the encrypted bytes into the buffer
  lseek(mFh,rawoffset, SEEK_SET);
  size_t readbytes=::read(mFh,buffer.encrypted(),buffer.size());
  //Notify the buffer of the actual number of bytes read.
  buffer.setencrypedsize(readbytes);
  //Now ask the buffer for the decrypted version of the data.
  return buffer.read(b,s);
}
int OpenBaseNode::write(const char *b, size_t s, off_t o){ 
  if (mFh ==0) {
    return -EBADF;
  }
  //FIXME: function needs error handling.
  capfs::crypto::Metrics metrics(o,s);
  size_t rawpresize = metrics.presize();
  size_t rawpostsize = metrics.postsize();
  size_t rawsize = s + rawpresize + rawpostsize;
  off_t rawoffset = o - rawpresize;
  //We use a crypto buffer that implements the actual crypto,
  // note that the third argument designates the first 16 byte block within our file.
  capfs::crypto::Buffer buffer(rawsize,mFek,metrics.ctr());
  if ((rawpresize + rawpostsize) !=0) {
    //If there is old encrypted data surounding this write, first read all existing encrypted bytes into the buffer
    lseek(mFh,rawoffset, SEEK_SET);
    size_t readbytes=::read(mFh,buffer.encrypted(),buffer.size());
    buffer.setencrypedsize(readbytes);
  } else {
    buffer.setencrypedsize(0);
  }
  //Now write the plaintext to the buffer so it will get encrypted.
  buffer.write(b,s,rawpresize);
  //Finaly write the encrypted data to the file at the right spot.
  lseek(mFh,rawoffset, SEEK_SET);
  ::write(mFh,buffer.encrypted(),buffer.size());
  return 0;
}
int OpenBaseNode::flush(){
  if (mFh ==0) {
    return -EBADF;
  }
  return 0;
}
int OpenBaseNode::fsync(int isdatasync){ 
  if (mFh ==0) {
    return -EBADF;
  }
  return 0;
}
int OpenBaseNode::readdir(void *buf, fuse_fill_dir_t filler, off_t offset){ 
  if (mFh == 0) { //the CapFs rootdir
     filler(buf, ".", NULL, 0);
     filler(buf, "..", NULL, 0);
     return 0;
  }
  return -EPERM;
}
int OpenBaseNode::fsyncdir(int isdatasync){ 
  if (mFh ==0) {
    return -EBADF;
  }
  return 0;
}
int OpenBaseNode::ftruncate(off_t l){
  if (mFh ==0) {
    return -EBADF;
  }
  return 0;
}
int OpenBaseNode::fstat(struct stat *s){
  if (mFh ==0) {
    return -EBADF;
  }
  return -EPERM;
}
int OpenBaseNode::lock(int cmd, struct flock *fl){
  if (mFh ==0) {
    return -EBADF;
  }
  return -EPERM;
}
int OpenBaseNode::ioctl(int cmd, void *arg, unsigned int flags, void *data){
  if (mFh ==0) {
    return -EBADF;
  }
  return -EPERM;
}
int OpenBaseNode::poll(fuse_pollhandle *ph, unsigned *reventsp){
  if (mFh ==0) {
    return -EBADF;
  }
  return -EPERM;
}
}
}
