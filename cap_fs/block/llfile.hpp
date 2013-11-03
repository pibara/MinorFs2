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
typedef enum {ll_closed,ll_open,ll_tempclosed} node_state;

class llfile {
    int mFd;
    std::string mFilePath;
    node_type mNtype;
    int mOpenFlags;
    int mReOpenFlags;
    llfile(open_file const &);
    llfile const & operator=(open_file const &);
  public:
    llfile(std::string filename,node_type ntype,int flags);
    ~llfile();
    void preOpen(std::string filename,node_type ntype,int flags);
    void void postClose();
    void lowLevelOpen();
    void lowLevelClose(); 
    ssize_t read(void *buf, size_t count);
    ssize_t write(const void *buf, size_t count);
    off_t lseek(off_t offset, int whence);
};
#endif
