#ifndef CAPFS_OPEN_FILE_HPP
#define CAPFS_OPEN_FILE_HPP
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <system_error>

typedef enum {data_file,exe_file,directory,symbolic_link} node_type;

class open_file {
    int mFd;
    open_file(open_file const &);
    open_file const & operator=(open_file const &);
  public:
    open_file(std::string filename,node_type ntype);
    ~open_file();
    ssize_t read(void *buf, size_t count);
    ssize_t write(const void *buf, size_t count);
    off_t lseek(off_t offset, int whence);
};
#endif
