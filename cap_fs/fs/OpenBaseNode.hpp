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
#ifndef MINORFS_CAPFS_OPENBASENODE_HPP
#define MINORFS_CAPFS_OPENBASENODE_HPP
#include <inttypes.h>
#define FUSE_USE_VERSION 26
#include <fuse.h>
class OpenBaseNode {
    uint64_t mFh;
  public:
    OpenBaseNode(uint64_t fh);
    int release();
    int releasedir();
    int read(char *b, size_t s, off_t o);
    int write(const char *b, size_t s, off_t o);
    int flush();
    int fsync(int isdatasync);
    int readdir(void *buf, fuse_fill_dir_t filler, off_t offset);
    int fsyncdir(int isdatasync);
    int ftruncate(off_t l);
    int fstat(struct stat *s);
    int lock(int cmd, struct flock *fl);
    int ioctl(int cmd, void *arg, unsigned int flags, void *data);
    int poll(fuse_pollhandle *ph, unsigned *reventsp);
};
#endif
