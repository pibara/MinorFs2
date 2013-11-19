#ifndef CAPFS_LLFILE_HPP
#define CAPFS_LLFILE_HPP
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <system_error>

typedef enum {nodetype_unknown,data_file,exe_file,directory,symbolic_link} node_type;
typedef enum {ll_closed,ll_open,ll_tempclosed,ll_invalidated} node_state;

class llfile {
    int mFd;
    std::string mFilePath;
    node_type mNtype;
    node_state mState;
    int mOpenFlags;
    int mReOpenFlags;
  public:
    llfile(llfile const &)=delete;
    llfile & operator=(llfile const &) = delete;
    llfile& operator=(llfile &&) = delete;
    llfile(); 
    llfile(std::string filename,node_type ntype,int flags);
    llfile(llfile&& n);
    ~llfile();
    void lowLevelOpen();
    void lowLevelClose(); 
    ssize_t read(void *buf, size_t count);
    ssize_t write(const void *buf, size_t count);
    off_t lseek(off_t offset, int whence);
};
#endif
