#ifndef HAVE_SETXATTR
#define HAVE_SETXATTR 1
#endif
#include "CapFs.hpp"
#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <errno.h>
#ifndef HAVE_SETXATTR
#error "This filesystem depends on XATTR support."
#endif
#include <string.h>
#include <exception>
#include <stdlib.h>
capfs::fs::BaseFs &get_fs() {
  struct fuse_context *context=fuse_get_context();
  capfs::fs::CapFs *capfsp=reinterpret_cast<capfs::fs::CapFs *>(context->private_data); 
  return (*capfsp);
}
//A dumb wrapper for string literals used to keep compilers from complaning about constness issues.
class string_literal_wrapper {
    std::string mString;
    char * mNonConst;
    string_literal_wrapper(string_literal_wrapper const &);
    string_literal_wrapper &operator=(string_literal_wrapper const &);
  public:
    string_literal_wrapper(const char * const cs):mString(cs),mNonConst(0) {
       mNonConst=static_cast<char *>(calloc(mString.size()+1,1));
       if (mNonConst == NULL) {
         throw std::bad_alloc();
       }
    }
    ~string_literal_wrapper() {
       free(mNonConst);
    }
    operator char *() {
       return strcpy(mNonConst,mString.c_str());
    }
};

void *capfs_init(struct fuse_conn_info *conn) {
   //FIXME: the hardcoded salt should be replaced by one from a secure source when possible.
   return reinterpret_cast<void*>(new capfs::fs::CapFs("ThisShouldBeASecretSalt"));
}
void capfs_destroy(void *capfsvoid){
  capfs::fs::CapFs *capfs=reinterpret_cast<capfs::fs::CapFs *>(capfsvoid);
  delete capfs;
}

int capfs_chown(const char *, uid_t, gid_t){ return -EPERM;} //chown is never permited by capfs.
int capfs_utime(const char *, struct utimbuf *) { return -EPERM;} //utime is never permitted by capfs.
int capfs_setxattr(const char *, const char *, const char *, size_t, int) { return -EPERM;} //Setting extended attributes is never permitted by capfs.
int capfs_removexattr(const char *, const char *) { return -EPERM;} //Removing extended attributes is never permitted by capfs.
int capfs_utimens(const char *, const struct timespec tv[2]) { return -EPERM;} //utime is never permitted by capfs.

int capfs_getattr(const char *p, struct stat *s) {
  return get_fs()[p].stat(s);
}
int capfs_readlink(const char *p, char *b, size_t l) {
  return get_fs()[p].readlink(b,l);
}
int capfs_mknod(const char *p, mode_t m, dev_t d){
  return get_fs()[p].mknod(m,d);
} 
int capfs_mkdir(const char *p, mode_t m){
  struct fuse_context *context=fuse_get_context();
  capfs::fs::CapFs *capfsp=reinterpret_cast<capfs::fs::CapFs *>(context->private_data); 
  return (*capfsp)[p].mkdir(m,context->gid == capfsp->getCreatorGid());
}
int capfs_unlink(const char *p){
  return get_fs()[p].unlink();
}
int capfs_rmdir(const char *p) {
  return get_fs()[p].rmdir();
}
int capfs_symlink (const char *p, const char *l) {
  return get_fs()[p].symlink(l);
}
int capfs_rename(const char *p, const char *p2){
  capfs::fs::BaseFs &fs=get_fs();
  return fs[p].rename(fs[p2]);
}
int capfs_link(const char *p1, const char *p2){
  capfs::fs::BaseFs &fs=get_fs();
  return fs[p1].link(fs[p2]);
}
int capfs_chmod(const char *p, mode_t m){
  return get_fs()[p].chmod(m);
}
int capfs_truncate(const char *p, off_t off){
  return get_fs()[p].truncate(off);
}
int capfs_statfs(const char *, struct statvfs *s){
  return get_fs().statfs(s);
}
int capfs_getxattr(const char *p, const char *n, char *v, size_t s){
  return get_fs()[p].getxattr(n,v,s);
}
int capfs_listxattr(const char *p, char *l, size_t s){
  return get_fs()[p].listxattr(l,s);
}
int capfs_access(const char *p, int m){
  return get_fs()[p].access(m);
}
int capfs_bmap(const char *p, size_t blocksize, uint64_t *idx){
  return get_fs()[p].bmap(blocksize,idx);
}

//The folowing set of functions are mapped directly to the BaseFs returned by the CapFsGuard, using a path to designate a node.
//These calls return a file handle that can be used in other calls.
int capfs_open(const char *p, struct fuse_file_info *finfo) {
  return get_fs()[p].open(&(finfo->fh),finfo->flags);
}
int capfs_create(const char *p, mode_t m, struct fuse_file_info *finfo){
  return get_fs()[p].create(&(finfo->fh),m);
}
int capfs_opendir(const char *p, struct fuse_file_info *finfo){
  return get_fs()[p].opendir(&(finfo->fh));
}
//The folowing set of functions are mapped directly to the BaseFs returned by the CapFsGuard, using a file handle to designate a node.
//The functions will result in the file handle being released
int capfs_release(const char *, struct fuse_file_info *finfo) {
  return get_fs()[finfo->fh].release();
}
int capfs_releasedir(const char *, struct fuse_file_info *finfo){
  return get_fs()[finfo->fh].releasedir();
}
//The folowing set of functions are mapped directly to the BaseFs returned by the CapFsGuard, using a file handle to designate a node.
int capfs_read(const char *, char *b, size_t s, off_t o, struct fuse_file_info *finfo) {
  return get_fs()[finfo->fh].read(b,s,o);
}
int capfs_write(const char *, const char *b, size_t s, off_t o, struct fuse_file_info *finfo){
  return get_fs()[finfo->fh].write(b,s,o);
}
int capfs_flush(const char *, struct fuse_file_info *finfo){
  return get_fs()[finfo->fh].flush();
}
int capfs_fsync(const char *, int isdatasync, struct fuse_file_info *finfo){
  return get_fs()[finfo->fh].fsync(isdatasync);
}
int capfs_readdir(const char *, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *finfo){
  return get_fs()[finfo->fh].readdir(buf,filler,offset);
}
int capfs_fsyncdir(const char *, int isdatasync, struct fuse_file_info *finfo){
  return get_fs()[finfo->fh].fsyncdir(isdatasync);
}
int capfs_ftruncate(const char *, off_t l, struct fuse_file_info *finfo){
  return get_fs()[finfo->fh].ftruncate(l);
}
int capfs_fgetattr(const char *, struct stat *s, struct fuse_file_info *finfo){
  return get_fs()[finfo->fh].fstat(s);
}
int capfs_lock(const char *, struct fuse_file_info *finfo, int cmd, struct flock *fl){
  return get_fs()[finfo->fh].lock(cmd,fl);
}
int  capfs_ioctl(const char *, int cmd, void *arg, struct fuse_file_info *finfo, unsigned int flags, void *data){
  return get_fs()[finfo->fh].ioctl(cmd,arg,flags,data);
}
int capfs_poll(const char *, struct fuse_file_info *finfo, struct fuse_pollhandle *ph, unsigned *reventsp){
  return get_fs()[finfo->fh].poll(ph,reventsp);
}

int main(int argc, char *argv[])
{
  int fuseargcount=1;
  char *fuseargv[6];
  struct fuse_operations capfs_oper;
  capfs_oper.init        = capfs_init,
  capfs_oper.destroy     = capfs_destroy,
  capfs_oper.getattr     = capfs_getattr,
  capfs_oper.fgetattr    = capfs_fgetattr,
  capfs_oper.access      = capfs_access,
  capfs_oper.readlink    = capfs_readlink,
  capfs_oper.readdir     = capfs_readdir,
  capfs_oper.mknod       = capfs_mknod,
  capfs_oper.mkdir       = capfs_mkdir,
  capfs_oper.symlink     = capfs_symlink,
  capfs_oper.unlink      = capfs_unlink,
  capfs_oper.rmdir       = capfs_rmdir,
  capfs_oper.rename      = capfs_rename,
  capfs_oper.link        = capfs_link,
  capfs_oper.chmod       = capfs_chmod,
  capfs_oper.chown       = capfs_chown,
  capfs_oper.truncate    = capfs_truncate,
  capfs_oper.ftruncate   = capfs_ftruncate,
  capfs_oper.utimens     = capfs_utimens,
  capfs_oper.create      = capfs_create,
  capfs_oper.open        = capfs_open,
  capfs_oper.read        = capfs_read,
  capfs_oper.write       = capfs_write,
  capfs_oper.statfs      = capfs_statfs,
  capfs_oper.release     = capfs_release,
  capfs_oper.opendir     = capfs_opendir,
  capfs_oper.releasedir  = capfs_releasedir,
  capfs_oper.fsync       = capfs_fsync,
  capfs_oper.flush       = capfs_flush,
  capfs_oper.fsyncdir    = capfs_fsyncdir,
  capfs_oper.lock        = capfs_lock,
  capfs_oper.bmap        = capfs_bmap,
  capfs_oper.ioctl       = capfs_ioctl,
  capfs_oper.poll        = capfs_poll,
  capfs_oper.setxattr    = capfs_setxattr,
  capfs_oper.getxattr    = capfs_getxattr,
  capfs_oper.listxattr   = capfs_listxattr,
  capfs_oper.removexattr = capfs_removexattr,
  capfs_oper.flag_nullpath_ok = 0;
  fuseargv[0]=argv[0];
  if ((argc > 1) && (argv[1][0] == '-')) {
      fuseargv[1]=argv[1];
      fuseargcount=2;
  }
  string_literal_wrapper mino("-o");
  fuseargv[fuseargcount]=mino;     //After the argv[0] and optionaly a -d argument set some options:
  string_literal_wrapper allow_other("allow_other");
  fuseargv[fuseargcount+1]=allow_other;  //* allow all users to access the filesystem.
  string_literal_wrapper mins("-s");
  fuseargv[fuseargcount+2]=mins; //Run the filesystem as single threaded.
  string_literal_wrapper mountpoint("/minorfs/cap");
  fuseargv[fuseargcount+3]=mountpoint;
  return fuse_main(4+fuseargcount, fuseargv, &capfs_oper,NULL);
}
