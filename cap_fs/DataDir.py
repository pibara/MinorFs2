#!/usr/bin/python
import os
class _Node:
    def __init__(self,fullpath):
        self.__fullpath=fullpath
        self.__excists=None
        self.__stat=None
    def exists(self):
        if self.__exists==None:
            self.__exists=False
            if os.path.exists(self.fullpath):
                self.__exists=True
        return self.__excists
    def stat(self):
        if self.__stat==None:
            self.__stat=os.stat(self.__fullpath) 
            nodetype = (self.__stat.st_mode & 070) >> 3
            #Depending on the type update the mode.
            if nodetype == 4:
                #A file is either non executable (0666) or executable (0777)
                if self.__stat.st_mode & 0100:
                    self.__stat.st_mode = 0777
                else:
                    self.__stat.st_mode = 0666
            elif nodetype == 6:
                #Directories have mode 0777
                self.__stat.st_mode = 0777 & stat.S_IFDIR
            elif nodetype == 7:
                #Symbolic links have mode 0777
                self.__stat.st_mode = 0777 & stat.S_IFLNK
        return self.__stat       
    def __open(self,flags,mode,nodetype):
        #The 'group' bits are the node type.
        nodetypemask = (nodetype & 0007) << 3
        #All node's are to be r+w, so only the 'x' flag of the mode is relevant.
        exeflag = mode & 0100
        filemode=exeflag | 0600 | nodetypemask
        if flags & os.O_CREAT:
            if self.__excists==False:
                self.__excists=None
            self.__stat=None
            l2dir=os.path.dirname(self.__fullpath)
            if not os.path.isdir(l2dir):
                l1dir=os.path.dirname(l2dir)
                if not os.path.isdir(l1dir):
                    os.mkdir(l1dir,0700)
                os.mkdir(l2dir,0700)
        return os.open(self.__fullpath,flags,filemode)
    def open(self,flags,mode):
        return self.__open(flags,mode,4)
    def openRawDirFile(flags,mode):
        return self.__open(flags,mode,6)
    def openRawLinkFile(flags,mode):
        return self.__open(flags,mode,7)

class Repository:
    def __init__(self,basedir):
        if not os.path.isdir(basedir):
            raise IOError ("Repository basedir not found",basedir)
        self.basedir=basedir
    def getNode(self,relpath):
        return _Node(self.basedir + "/" + relpath)
           
