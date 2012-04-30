#!/usr/bin/python
from fuse import Fuse

class FileSystem(Fuse):
    def __init__(self,repository,core,mountpoint):
        self.repository = repository
        self.core = core
        self.mountpoint = mountpoint
        Fuse.__init__(self)
        class CapFsFile(object):
            def __init__(self, path, flags, *mode):
                lonodes=core.lookup(path)
                self.node=lonodes[0]
                parent=lonodes[1]
                return self.node.open(parent,flags,*mode)
            def read(self, length, offset):
                return self.node.read(length, offset)
            def write(self, buf, offset):
                return self.node.write(buf, offset)
            def release(self, flags):
                return self.node.release(flags)
            def fsync(self, isfsyncfile):
                return self.node.fsync(isfsyncfile)
            def flush(self):
                return self.node.flush()
            def fgetattr(self):
                return self.node.getattr()
            def ftruncate(self, len):
                return self.node.truncate(len)
        self.file_class = CapFsFile    
    def getattr(self, path):
        lonodes=core.lookup(path)
        node=lonodes[0]
        return node.getattr()
    def readlink ( self, path ):
        lonodes=core.lookup(path)
        node=lonodes[0]
        return node.readlink()
    def readdir(self,path,offset):
        lonodes=core.lookup(path)
        node=lonodes[0]
        return node.readdir(offset)
    def unlink(self,path):
        lonodes=core.lookup(path)
        node=lonodes[0]
        parent=lonodes[1]
        return parent.unlink(node)
    def rmdir(self,path):
        lonodes=core.lookup(path)
        node=lonodes[0]
        parent=lonodes[1]
        return parent.rmdir(node)
    def symlink(self,pathold,pathnew):
        lonodes=core.lookup(pathnew)
        node=lonodes[0]
        parent=lonodes[1]
        return parent.symlink(pathold,node)
    def rename(self,pathold,pathnew):
        linkok=self.link(pathold,pathnew)
        if (linkok >= 0):
            return self.unlink(pathold)
        else:
            return linkok
    def link(self,pathold,pathnew):
        lonodesnew=core.lookup(pathnew)
        newnode=lonodesnew[0]
        newparent=lonodesnew[1]
        lonodesold=core.lookup(pathold)
        oldnode=lonodesold[0]
        return newparent.link(oldnode,newparent,newnode)
    def chmod(self,path,mode):
        lonodes=core.lookup(path)
        node=lonodes[0]
        return node.chmod(mode)
    def chown(self,path,user,group):
        return -errno.EPERM
    def truncate(self,path,leng):
        lonodes=core.lookup(path)
        node=lonodes[0]
        return node.truncate(leng)
    def mknod(self,path,mode,dev):
        return -errno.EPERM
    def mkdir(self,path,mode):
        lonodes=core.lookup(path)
        node=lonodes[0]
        parent=lonodes[1]
        return parent.mkdir(node)
    def utime(self,path,times):
        lonodes=core.lookup(path)
        node=lonodes[0]
        return node.utime(times)
    def access(self,path,mode):
        lonodes=core.lookup(path)
        node=lonodes[0]
        return node.access(mode)
    def getxattr(self,path,name,size):
        lonodes=core.lookup(path)
        node=lonodes[0]
        return node.getxattr(node,size)
    def listxattr(self,path,size):
        lonodes=core.lookup(path)
        node=lonodes[0]
        return node.listxattr(size)
    def statfs(self):
        return -errno.ENOSYS   

    

