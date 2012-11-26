#!/usr/bin/python
import os
def Xor(a,b):
    rval=""
    index=0
    l=len(a)
    if l == 0:
        return ""
    for achar in a:
        bchar=b[index]
        cchar=chr(ord(achar) ^ ord(bchar))
        rval += cchar
        index +=1
    return rval

class XorFhProxy:
    def __init__(self,fh,key):
        self.fh=fh
        self.key=key
        self.keylen=len(key)
        self.closed=False
        return
    def __del__(self):
        if self.closed==False:
            os.close(self.fh)
    def __someCrypt(self,data,reloffset):
        datalen=len(data)
        leadingsize=(self.keylen - reloffset) % self.keylen
        leading=data[:leadingsize]
        leadingkey=self.key[self.keylen-leadingsize:]
        rval = Xor(leading,leadingkey)
        trailingsize=(datalen-leadingsize) % self.keylen
        corelen=datalen-leadingsize-trailingsize
        corecount = corelen / self.keylen
        if corecount > 0:
            pieceindex=0
            while pieceindex < corecount:
                fullindex=leadingsize+self.keylen*pieceindex
                piece=data[fullindex:fullindex+self.keylen]
                rval += Xor(piece,self.key)
                pieceindex +=1
        trailer=data[datalen-trailingsize:]
        rval += Xor(trailer,self.key)
        return rval
    def read(self,n,offset):
        os.lseek(self.fh,offset,os.SEEK_SET)
        return self.__someCrypt(os.read(self.fh,n),offset % self.keylen)
    def readall(self):
        return self.__someCrypt(self.read(100000000,0),0)
    def write(self,string,offset):
        os.lseek(self.fh,offset,os.SEEK_SET)
        return os.write(self.fh,self.__someCrypt(string,offset % self.keylen))
    def close(self):
        if self.closed==False:
            self.closed=True
            return os.close(self.fh)
        return 

class XorCrypto:
    def __init__(self,key):
        self.key=key
    def fileProxy(self,fh,mode):
        return XorFhProxy(fh,self.key)

class CryptoModule:
    """This is the default crypto module for the MinorFs CapFs file-system.
It doesn't use strong crypto but instead uses a simple repeating non-chained
XOR operation on the data using the 256 bit file encryption key.
This should basically be just enough to deter a nosy system administrator.
Basically, as CapFs runs under a dedicated user-id and group-id and only
MinorFs and root can gain access to the raw files, this should be sufficient
security for most users. If your security demands go beyond this level, than
you may want to create a CryptoModule implementation based on a strong
cryptographic algoritms. If performance is what you want, there is an alternative
Null crypto module that should be faster than this XOR based module.
"""
    def Crypto(self,key):
        "This method returns a new XOR crypto module for a node given its file encryption key"
        return XorCrypto(key)
