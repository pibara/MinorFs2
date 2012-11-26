#!/usr/bin/python
import os
class _Node:
    def __init__(self,fullpath,crypto):
        self.__fullpath=fullpath
        self.__crypto=crypto
        self.__excists=None
        self.__stat=None
        self.__nodetype=None
    def exists(self):
        if self.__exists==None:
            self.__exists=False
            if os.path.exists(self.fullpath):
                self.__exists=True
        return self.__excists
    def stat(self):
        if self.__stat==None:
            self.__stat=os.stat(self.__fullpath)
            #The group bits determine if this is a regular file or an (encrypted) json dir or link representation. 
            self.__nodetype = (self.__stat.st_mode & 070) >> 3
            #Depending on the type update the mode.
            if self.__nodetype == 4: #A regular file
                #A file is either non executable (0666) or executable (0777)
                if self.__stat.st_mode & 0100:
                    self.__stat.st_mode = 0777
                else:
                    self.__stat.st_mode = 0666
            elif self.__odetype == 6:
                #Directories have mode 0777
                self.__stat.st_mode = 0777 & stat.S_IFDIR
            elif self.__nodetype == 7:
                #Symbolic links have mode 0777
                self.__stat.st_mode = 0777 & stat.S_IFLNK
        return self.__stat       
    def chmod(self,mode):
        self.stat()
        #All node's are to be r+w, so only the 'x' flag of the mode is relevant.
        exeflag = mode & 0100
        #The 'group' bits are the node type.
        nodetypemask = (self.__nodetype & 0007) << 3
        newmode = exeflag | 0600 |  nodetypemask
        os.chmod(self.__fullpath,newmode)
        #If the node type is 'file', we update our external mode according to the exe flag.
        if self.__nodetype == 4:
            if exeflag:
                self.__stat.st_mode = 0777
            else:
                self.__stat.st_mode = 0666
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
        return self.__crypto.fileProxy(self.__open(flags,mode,4),mode)
    def getJson(self):
        readfile=self.__crypto.fileProxy(self.__open(os.O_RDONLY,0700,nodetype),"r")
        jsondata=readfile.readall()
        readfile.close()
        return readfile.close()
    def __putJson(self,jsonstring,nodetype):
        writefile=self.__crypto.fileProxy(self.__open(os.O_CREAT|os.O_WRONLY,0700,nodetype),"w")
        writefile.write(jsonstring,0)
        writefile.close    
        return
    def putJsonDir(self,jsonstring):
        self.__putJson(jsonstring,6)
        return
    def putJsonLink(self,jsonstring):
        self.__putJson(jsonstring,7)
        return

class Repository:
    def __init__(self,basedir,cryptomodule):
        if not os.path.isdir(basedir):
            raise IOError ("Repository basedir not found",basedir)
        self.basedir=basedir
        self.cryptomodule=cryptomodule
    def getNode(self,relpath,cryptokey):
        return _Node(self.basedir + "/" + relpath,self.cryptomodule.Crypto(cryptokey))
           
if __name__ == "__main__":
    from XorCrypto import CryptoModule
    cryptomodule = CryptoModule()
    if not os.path.exists("./test"):
        os.mkdir("./test")
    rep = Repository("./test",cryptomodule)
    dirnode  = rep.getNode("/AB/CD/L22DPVG6T4KUQ3IFJPTUEG42SWOUCBGEUJGCSMTBRFYPMY6A","fakekey1fakekey1fakekey1fakekey1")
    dirnode.putJsonDir('{ "content" : ["foo"] }')
    filenode = rep.getNode("/EF/GH/L22DPVG6T4KUQ3IFJPTUEG42SWOUCBGEUJGCSMTBRFYPMY6A","fakekey2fakekey2fakekey2fakekey2")
    fh=filenode.open(os.O_CREAT|os.O_WRONLY,0644)
    fh.write("just testing",0)
    fh.close()
    filenode2 = rep.getNode("/EF/GH/M22DPVG6T4KUQ3IFJPTUEG42SWOUCBGEUJGCSMTBRFYPMY6B","fakekey3fakekey3fakekey3fakekey3")
    fh=filenode2.open(os.O_CREAT|os.O_WRONLY,0755)
    fh.write("One more test.\n",0)
    fh.write("""Now lets test with a larger chunk of data. 
We start this data at an unaligned offset so we test the crypto module used also.
""",15)
    fh.close()
    filenode2 = rep.getNode("/EF/GH/M22DPVG6T4KUQ3IFJPTUEG42SWOUCBGEUJGCSMTBRFYPMY6B","fakekey3fakekey3fakekey3fakekey3")
    fh=filenode2.open(os.O_RDONLY,0700)
    print fh.read(1000,0)
    fh.close()
    linknode = rep.getNode("/AB/GH/L22DPVG6T4KUQ3IFJPTUEG42SWOUCBGEUJGCSMTBRFYPMY6A","fakekey4fakekey4fakekey4fakekey4")
    linknode.putJsonLink('{ "link" : "/home/rob/test" }')
