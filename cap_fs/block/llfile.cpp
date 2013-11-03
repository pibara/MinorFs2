#include "llfile.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

llfile::llfile():mFd(0),mFilePath(""),mNtype(data_file),mOpenFlags(0),mReOpenFlags(0),mState(ll_closed) {}

llfile::preOpen(std::string filename,node_type ntype,int flags) {
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
llfile::~llfile() {
   close(mFd);
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
