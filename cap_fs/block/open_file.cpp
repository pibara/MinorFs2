#include "open_file.hpp"

open_file::open_file(std::string filename,node_type ntype) {
   mode_t mode=0;;
   switch (ntype) {
     case data_file: mode=00600;
                    break;
     case exe_file:  mode=00700;
                    break;
     case directory: mode=04700;
                    break;
     case symbolic_link: mode=04600;
                    break;
   }
   mFd=open(filename.c_str(),O_RDWR|O_CREAT|O_LARGEFILE|O_NOATIME,mode);
   if (mFd == -1) {
      throw std::system_error(errno,std::system_category());
   }
}
open_file::~open_file() { 
   close(mFd);
}
ssize_t open_file::read(void *buf, size_t count){
        ssize_t rval = ::read(mFd,buf,count);
        if (rval == -1) {
            throw std::system_error(errno,std::system_category());
        }
        return rval;
}
ssize_t open_file::write(const void *buf, size_t count) {
        ssize_t rval= ::write(mFd,buf,count);
        if (rval == -1) {
            throw std::system_error(errno,std::system_category());
        }
        return rval;
}
off_t open_file::lseek(off_t offset, int whence) {
        off_t rval=::lseek(mFd,offset,whence);
        if (rval == (off_t) -1) {
            throw std::system_error(errno,std::system_category());
        }
        return rval;
}
