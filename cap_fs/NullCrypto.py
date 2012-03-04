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
        os.lseek(self.fh,offset,os.SEEK_SET)
        return os.read(self.fh,n)
    def readall(self):
        return self.read(100000000,0)
    def write(self,string,offset):
        os.lseek(self.fh,offset,os.SEEK_SET)
        return os.write(self.fh,string)
    def close(self):
        if self.closed==False:
            self.closed=True
            return os.close(self.fh)
        return 

class NullCrypto:
    def fileProxy(self,fh,mode):
        return NullFhProxy(fh)

class CryptoModule:
    """This is an alternative NULL crypto module for the MinorFs CapFs file-system.
It doesn't do any crypto but it should be faster than the default weak XOR crypto module.
"""
    def Crypto(self,key):
        "This method returns a new NULL crypto module for a node. The file encryption key is simply ignored."
        return NullCrypto()
