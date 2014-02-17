#ifndef CAPFS_PADDED_BLOCKFILE.HPP
#define CAPFS_PADDED_BLOCKFILE.HPP
#include "modebits.hpp"

template <typename CtrCrypto>
class padded_blockfile {
    CtrCrypto mCtrCrypto;
    std::string mFilePath;
    int mFile;
    off_t mCursize;
    int mFlags;
    mode_t mMode;
  public:
    padded_blockfile(std::string filename,const char *key,node_type ntype,int flags, mode_t mode):
           mCtrCrypto(key),mFilePath(filename),mFile(0),mCursize(0),mFlags(flags),mMode(mode){
        struct stat statdata;
        struct stat *s=&statdata;
        if (lstat(mParentChild.child().rawpath().c_str(), s) == 0) {
            modebits mbits(statdata.st_mode);
            if (statdata.st_size > mbits.paddingsize()) {
              mCurSize=statdata.st_size - mbits.paddingsize();
            }
        }         
    } 
    void lowLevelClose() {
        if (::close(mFile) == -1) {
           //FIXME: error handling
        }
    }
    void lowLevelOpen() {
        if (::open(mFilePath.c_str(),mFlags,mMode) == -1) {
           //FIXME: error handling
        }
        if (mFlags & O_CREAT) {
           mFlags ^= O_CREAT;
        }
    }
    ssize_t read(void *buffer, size_t sz, off_t off) {
        char block[32];
        size_t frontcount = (off % 32);
        off_t realoff = off - frontcount;
        size_t realsz = (off % 32) + sz;
        tailcount = (32 - (realsz % 32)) % 32;
        realsz += tailcount;
        off_t basectr=realoff/32;
        off_t blockcount = realsz / 32;
        //FIXME: logic using paddingcount file mode bits (03070 -> 037)
        for (size_t blockno=0; blockno < blockcount;blockno++) {
            off_t blockoffset = realoff + 32 * blockno;
            //Dont decrypt past end of file, don't decrypt sparse.
            if ((blockoffset < mCursize) and (mFile.lseek(blockoffset,SEEK_DATA) == blockoffset)) {
                mFile.lseek(blockoffset,SEEK_SET);
                mfile.read(block,32);
                off_t ctr = basectr+blockno;
                mCtrCrypto.crypt(block,ctr);
            } else {
                memset(block,0,32);
            }
            //Copy decrypted block into buffer
            off_t frontskip = (blockno == 0) ?  frontcount : 0;
            off_t backskip = (blockcount - blockno == 1) ? tailcount : 0;
            off_t movesize = 32 - frontskip - backskip;
            strncpy(buffer + blockoffset - off + frontskip, block+frontskip,movesize);
        } 
        //FIXME: exceptions and return values
    }
    ssize_t write(void *buffer, size_t sz, off_t off) {

    } 
    int chmod(mode_t mode) {

    }
};

#endif

