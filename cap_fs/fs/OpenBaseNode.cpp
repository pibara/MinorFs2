#include "OpenBaseNode.hpp"
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
  return 0;
}
int OpenBaseNode::write(const char *b, size_t s, off_t o){ 
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
