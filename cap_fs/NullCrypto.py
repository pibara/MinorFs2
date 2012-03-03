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
    def read(self,n):
        return os.read(self.fh,n)
    def readall(self):
        return os.read(self.fh,10000000)
    def write(self,string):
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
