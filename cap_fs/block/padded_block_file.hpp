#ifndef CAPFS_PADDED_BLOCKFILE.HPP
#define CAPFS_PADDED_BLOCKFILE.HPP
#include "open_file.hpp"

template <typename CtrCrypto>
class padded_blockfile {
    CtrCrypto mCtrCrypto;
    open_file mFile;
    off_t mCursize;
  public:
    padded_blockfile(std::string filename,const char *key,node_type ntype):mCtrCrypto(key),mFile(filename,ntype),mCursize(mFile.lseek(0,SEEK_END)){} 
    int read(char *buffer, size_t sz, off_t off) {
        char block[32];
        size_t frontcount = (off % 32);
        off_t realoff = off - frontcount;
        size_t realsz = (off % 32) + sz;
        tailcount = (32 - (realsz % 32)) % 32;
        realsz += tailcount;
        off_t basectr=realoff/32;
        for (size_t blockno=0; blockno < (realsz / 32);blockno++) {
            mFile.lseek(realoff + 32 * blockno,SEEK_SET);
            mfile.read(block,32);
            size_t frontpaddingno = off - realoff;
            size_t endpaddingno = 
            size_t skipend=0;
            mCtrCrypto.decrypt(block,basectr+blockno);
            //fixme    
        } 
    }
    write(char *buffer, size_t sz, off_t off) {

    }  
};

#endif

