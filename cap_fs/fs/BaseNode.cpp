//  Copyright (c) 2012, Rob J Meijer
//
//Permission is hereby granted, free of charge, to any person or organization
//obtaining a copy of the software and accompanying documentation covered by
//this license (the "Software") to use, reproduce, display, distribute,
//execute, and transmit the Software, and to prepare derivative works of the
//Software, and to permit third-parties to whom the Software is furnished to
//do so, all subject to the following:
//
//The copyright notices in the Software and this entire statement, including
//the above license grant, this restriction and the following disclaimer,
//must be included in all copies of the Software, in whole or in part, and
//all derivative works of the Software, unless such copies or derivative
//works are solely in the form of machine-executable object code generated by
//a source language processor.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
//SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
//FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//DEALINGS IN THE SOFTWARE.
#include <BaseNode.hpp>
#include <string.h>

bool mAccess;
    std::string mRelPath;

BaseNode::BaseNode():mAccess(false),mRelPath(""){}
BaseNode::BaseNode(std::string relpath):mAccess(true),mRelPath(relpath){}
int BaseNode::stat(struct stat *s) { 
  if (mAccess == false) {
    return -EPERM;
  }
  memset(s, 0, sizeof(struct stat));
  if (mRelPath == "/") {
     s->st_mode = S_IFDIR | 0755;
     s->st_nlink = 3; 
     return 0;
  }
  return -ENOENT;  
}
int BaseNode::readlink(char *b, size_t l) { return -EPERM;}
int BaseNode::mknod(mode_t m, dev_t d) { return -EPERM;}
int BaseNode::mkdir(mode_t m) { return -EPERM;}
int BaseNode::unlink() { return -EPERM;}
int BaseNode::rmdir() { return -EPERM;}
int BaseNode::symlink(std::string l) {return -EPERM;}
int BaseNode::rename(BaseNode l) {return -EPERM;}
int BaseNode::link(BaseNode l) {return -EPERM;}
int BaseNode::chmod(mode_t m) {return -EPERM;}
int BaseNode::truncate(off_t off) {return -EPERM;}
int BaseNode::getxattr(const char *n, char *v, size_t s)  {return -EPERM;}
int BaseNode::listxattr(char *l, size_t s) {return -EPERM;}
int BaseNode::access(int m) {return -EPERM;}
int BaseNode::bmap(size_t blocksize, uint64_t *idx) {return -EPERM;}
int BaseNode::open(uint64_t *fh,int flags) {return -EPERM;}
int BaseNode::opendir(uint64_t *fh)  {return -EPERM;}
int BaseNode::create(uint64_t *fh,mode_t m) {return -EPERM;}
