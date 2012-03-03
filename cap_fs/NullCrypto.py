#!/usr/bin/python
import os
class NullFhProxy:
    def __init__(self,fh):
        self.fh=fh
        self.closed=False
        return
    def __del__(self):
        if self.closed==False:
            os.close(self.fh)
    def read(self,n,offset):
        os.lseek(self.fd,offset,os.SEEK_SET) #FIXME: check
        return os.read(self.fh,n)
    def readall(self):
        return self.read(100000000,0) #This method is meant for link and dir json stuff, a dir should not exceed 100MB of json probably.
    def write(self,string,offset):
        os.lseek(self.fd,offset,os.SEEK_SET) #FIXME: check
        return os.write(self.fh,string)
    def close(self):
        if self.closed==False:
            self.closed=True
            return os.close(self.fh)
        return 

class NullCrypto:
    def fileProxy(self,fh,mode):
        return NullFhProxy(fh)

class Module:
    def Crypto(self,key):
        return NullCrypto()
