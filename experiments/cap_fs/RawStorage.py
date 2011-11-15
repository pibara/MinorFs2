#!/usr/bin/python
import os
import json

class NullCryptoNode:
    def encrypt(self,indata,dataoffset):
        return indata
    def decrypt(self,indata,dataoffset):
        return indata

class NullCryptoAlgo:
    def __call__(self,key):
        return NullCryptoNode()

class RawDir:
    def __init__(self,rawpath,cryptonode):
        self.rawpath=rawpath
        self.cryptonode=cryptonode
        self.dirset = {}
        try:
            fil = open(self.rawpath, 'r')
            self.dirset=json.loads(cryptonode.decrypt(fil.read(),0))
            fil.close()
        except:
            pass
    def create(self,mode):
        d = os.path.dirname(self.rawpath)
        if not os.path.exists(d):
            os.makedirs(d)
        fil = open(self.rawpath, 'w')
        fil.write(self.cryptonode.encrypt(json.dumps(self.dirset),0))
        fil.close() 
    def addSubNode(self,nodename):
        self.dirset[nodename] = 1
        fil = open(self.rawpath, 'w')
        fil.write(self.cryptonode.encrypt(json.dumps(self.dirset),0))
        fil.close()


class RawFile:
    def __init__(self,rawpath,cryptonode):
        self.rawpath=rawpath
        self.cryptonode=cryptonode
        self.file=None
    def create(self,mode):
        d = os.path.dirname(self.rawpath)
        if not os.path.exists(d):
            os.makedirs(d)
        fil = open(self.rawpath, 'w')
        fil.close()
    def open(self,mode):
        self.file=open(self.rawpath, mode)
    def close(self):
        self.file.close()
    def write(self,data):
        print "FIXME: write to file"
    def read(self,count):
        print "FIXME: read from file"

class RawLink:
    def __init__(self,rawpath,cryptonode):
        self.rawpath=rawpath
        self.cryptonode=cryptonode
        self.oldpath = None
        try:
            fil = open(self.rawpath, 'r')
            self.oldpath=json.loads(cryptonode.decrypt(fil.read(),0))[0]
            fil.close()
        except:
            pass
    def create(self,oldpath):
        self.oldpath=oldpath
        d = os.path.dirname(self.rawpath)
        if not os.path.exists(d):
            os.makedirs(d)
        fil = open(self.rawpath, 'w')
        fil.write(self.cryptonode.encrypt(json.dumps([self.oldpath]),0))
        fil.close()
    def readlink(self):
        return self.oldpath

class RawNode:
    def __init__(self,rawpath,cryptoalgo):
        self.cryptoalgo=cryptoalgo
        self.isfile = False
        self.isdir = False
        self.islink = False
        self.exists = False
        self.rawpath=""
        self.cryptonode=None
        self.readonly=False
        self.filepath = rawpath + ".cdata"
        self.dirpath  = rawpath + ".cjson"
        self.linkpath = rawpath + ".clink"
        self.rawdir=None
        self.rawfile=None
        self.rawlink=None
        if os.path.exists(self.filepath):
            self.isfile = True
            self.rawpath=self.filepath
            self.exists = True
        if os.path.exists(self.dirpath):
            self.isdir = True
            self.rawpath=self.dirpath
            self.exists = True
        if os.path.exists(self.linkpath):
            self.islink=True
            self.rawpath=self.linkpath
            self.exists = True
    def exists(self):
        return self.exists
    def setCryptoKey(self,key):
        self.cryptonode=self.cryptoalgo(key)
        if self.isfile:
            self.rawfile=RawFile(self.rawpath,self.cryptonode)
        if self.isdir:
            self.rawdir = RawDir(self.rawpath,self.cryptonode)
        if self.islink:
            self.rawlink = RawLink(self.rawpath,self.cryptonode)
    def setReadOnly(self):
        self.readonly=True
    def createAsDir(self,mode):
        self.isdir = True
        self.exists = True
        self.rawpath=self.dirpath
        self.rawdir = RawDir(self.rawpath,self.cryptonode)
        self.rawdir.create(mode)
    def createAsFile(self,mode):
        self.isfile = True
        self.exists = True
        self.rawpath=self.filepath
        self.rawfile = RawFile(self.rawpath,self.cryptonode)
        self.rawfile.create(mode)
    def createAsLink(self,oldpath):
        self.islink = True
        self.exists = True
        self.rawpath=self.linkpath
        self.rawlink = RawLink(self.rawpath,self.cryptonode)
        self.rawlink.create(oldpath)
    def isFile(self):
        return self.isfile
    def isDir(self):
        return self.isdir
    def isLink(self):
        return self.islink
    def asDir(self):
        return self.rawdir
    def asLink(self):
        return self.rawlink
    def asFile(self):
        return self.rawfile

class RawStorage:
    def __init__(self,topdir,cryptoalgo):
        self.topdir = topdir
        self.cryptoalgo = cryptoalgo
    def pathToNode(self,path):
        return RawNode(self.topdir + path,self.cryptoalgo)

if __name__ == "__main__":
    algo=NullCryptoAlgo()
    storage=RawStorage("/tmp/capfs_test",algo)
    node1=storage.pathToNode("/4F/5W/VQ5YN3QZNXMV7TRC6EIISVKJQ55JO7ANBNNG2MCL2NNEHAGQ")
    if node1.exists:
        print "ERROR: Node exists"
    else:
        node1.setCryptoKey("");
        node1.createAsDir(0755);
        if not node1.exists:
            print "Problem creating non existing node"
        dir1=node1.asDir()
        dir1.addSubNode("foo")
        dir1.addSubNode("bar")
    node2=storage.pathToNode("/01/12/VN5YN3QZNXMV7TRC6EIISVKJQ55JO7ANBNNG2MCL2NNEABAB")
    if node2.exists:
        print "ERROR: Node exists"
    else:
        node2.setCryptoKey("")
        node2.createAsFile(0644)
        file2=node2.asFile()
        file2.open("w")
        file2.write("Hi there\n")
        file2.close()
    node3=storage.pathToNode("/01/23/456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEF")
    if node3.exists:
        print "ERROR: Node exists"
    else:
        node3.setCryptoKey("")
        node3.createAsLink("foobar")
        
