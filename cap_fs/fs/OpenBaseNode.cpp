#include "OpenBaseNode.hpp"
#include "../crypto/Buffer.hpp"
#include <errno.h>
namespace capfs {
namespace fs {
OpenBaseNode::OpenBaseNode(uint64_t fh):mFh(fh){
}
int OpenBaseNode::release(){ 
  return 0;
}
int OpenBaseNode::releasedir(){ 
  return 0;
}
int OpenBaseNode::read(char *b, size_t s, off_t o){ 
  //FIXME: function needs error handling.
  /*We need to read each 16 byte block that contains any of the bytes we want to read*/
  size_t rawpresize = o % 16;
  size_t rawpostsize = 15 - ((o+s+15)%16);
  size_t rawsize = s + rawpresize + rawpostsize;
  off_t rawoffset = o - rawpresize;
  //We use a crypto buffer that implements the actual crypto,
  // note that the third argument designates the first 16 byte block within our file.
  capfs::crypto::Buffer buffer(rawsize,mFek,rawoffset/16);
  //Read the encrypted bytes into the buffer
  size_t readbytes=read(mFh,buffer.encrypted(),buffer.size());
  //Notify the buffer of the actual number of bytes read.
  buffer.setencrypedsize(readbytes);
  //Now ask the buffer for the decrypted version of the data.
  return buffer.read(b);
}
int OpenBaseNode::write(const char *b, size_t s, off_t o){ 
  //FIXME: function needs error handling.
  /*We need to read each 16 byte block that contains any of the bytes we want to (over)write*/
  size_t rawpresize = o % 16;
  size_t rawpostsize = 15 - ((o+s+15)%16);
  size_t rawsize = s + rawpresize + rawpostsize;
  off_t rawoffset = o - rawpresize;
  //We use a crypto buffer that implements the actual crypto,
  // note that the third argument designates the first 16 byte block within our file.
  capfs::crypto::Buffer buffer(rawsize,mFek,rawoffset/16);
  if ((rawpresize + rawpostsize) !=0) {
    //If there is old encrypted data surounding this write, first read all existing encrypted bytes into the buffer
    size_t readbytes=read(mFh,buffer.encrypted(),buffer.size());
    buffer.setencrypedsize(readbytes);
  } else {
    buffer.setencrypedsize(0);
  }
  //Now write the plaintext to the buffer so it will get encrypted.
  buffer.write(b,s,rawpresize);
  //Finaly write the encrypted data to the file at the right spot.
  size_t writecount=write(mFh,buffer.encrypted(),buffer.size());
  return 0;
}
int OpenBaseNode::flush(){ 
  return 0;
}
int OpenBaseNode::fsync(int isdatasync){ 
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
  return 0;
}
int OpenBaseNode::ftruncate(off_t l){
  return 0;
}
int OpenBaseNode::fstat(struct stat *s){
  return -EPERM;
}
int OpenBaseNode::lock(int cmd, struct flock *fl){
  return -EPERM;
}
int OpenBaseNode::ioctl(int cmd, void *arg, unsigned int flags, void *data){
  return -EPERM;
}
int OpenBaseNode::poll(fuse_pollhandle *ph, unsigned *reventsp){
  return -EPERM;
}
}
}
