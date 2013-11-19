#include "llfile.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

llfile::llfile():mFd(0),mFilePath(""),mNtype(data_file),mOpenFlags(0),mReOpenFlags(0),mState(ll_invalidated) {}

llfile::llfile(std::string filename,node_type ntype,int flags):mFd(0),mFilePath(filename),mNtype(ntype),mOpenFlags(flags),mReOpenFlags(flags),mState(ll_closed) {
   struct stat nodestat;
   bool statok = (lstat(filename.c_str(),&nodestat) == 0);
   bool fileexists = true;
   if (!statok) {
     if ((errno != ENOENT) ||((flags & O_CREAT) == 0)) {
        throw std::system_error(errno,std::system_category());
     }
     fileexists = false;
   }
   if (fileexists) {
     switch (nodestat.st_mode & 04100) {
        case 0: ntype=data_file;
                break;
        case 00100: ntype=exe_file;
                break;
        case 04000: ntype=symbolic_link;
                break;
        case 04100: ntype=directory;
                break;
     }
   } else {
     //FIXME: create parent directories if needed.
   }
   mFilePath=filename;
   mNtype=ntype
   mMode=0;
   switch (ntype) {
     case data_file: mMode=00600;
                    break;
     case exe_file:  mMode=00700;
                    break;
     case directory: mMode=04700;
                    break;
     case symbolic_link: mMode=04600;
                    break;
   }
   mFd=open(filename.c_str(),flags|O_LARGEFILE|O_NOATIME,mMode);
   if (mFd == -1) {
      throw std::system_error(errno,std::system_category());
   }
}

llfile::llfile(llfile&& n):mFd(n.mFd),mFilePath(n.mFilePath),mNtype(n.mNtype),mState(n.mState) {
  if (&n != this) {
    n.mState=ll_invalidated;
  }
}

llfile::~llfile() {
   if (mState == ll_open) {
      ::close(mFd);
   }
}

void llfile::lowLevelOpen() {
    
}

void llfile::lowLevelClose() {

}

ssize_t llfile::read(void *buf, size_t count){
        ssize_t rval = ::read(mFd,buf,count);
        if (rval == -1) {
            throw std::system_error(errno,std::system_category());
        }
        return rval;
}
ssize_t llfile::write(const void *buf, size_t count) {
        ssize_t rval= ::write(mFd,buf,count);
        if (rval == -1) {
            throw std::system_error(errno,std::system_category());
        }
        return rval;
}
off_t llfile::lseek(off_t offset, int whence) {
        off_t rval=::lseek(mFd,offset,whence);
        if (rval == (off_t) -1) {
            throw std::system_error(errno,std::system_category());
        }
        return rval;
}
