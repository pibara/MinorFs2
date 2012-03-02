#!/usr/bin/python
#Hmm, these imports polute the mudule namespace, how does this work in python?
import hashlib
import hmac
import base64
import random

class _Key:
    def __init__(self,rwkey,rokey,core):
        self.rwkey=rwkey
        self.rokey=rokey
        self.core=core
        self.lkey=None
    def getRwCap(self):
        if self.rwkey:
            return "rw-" + base64.b32encode(self.rwkey)[:52]
        else:
            return None
    def getRawCryptoKey(self):
        if self.rokey==None:
            self.rokey=self.core._rw2ro(self.rwkey)
        return self.rokey
    def getRoCap(self):
        return "ro-" + base64.b32encode(self.getRawCryptoKey())[:52]
    def __getRawLocationKey(self):
        if self.lkey == None:
            self.lkey = self.core._ro2loc(self.getRawCryptoKey())
        return self.lkey
    def getLocation(self):
        k = base64.b32encode(self.__getRawLocationKey())[:52]
        return "/"+k[:2]+"/"+k[2:4]+"/"+k[4:]
    def getSubNodeKey(self,name):
        if self.rwkey:
            return _Key(self.core._ro2subrw(self.getRawCryptoKey(),name),None,self.core)
        else:
            return _Key(None,self.core._rw2ro(self.core._ro2subrw(self.getRawCryptoKey(),name)),self.core)
    def getRoKey(self):
        if self.rwkey==None:
            return self
        return _Key(0,self.getRawCryptoKey(),self.core)
    def isReadOnly(self):
        if self.rwkey==None:
            return True
        return False        

class Core:
    def __init__(self,secretsalt):
        self._secretsalt=hashlib.sha256(secretsalt).hexdigest() 
    def _rw2ro(self,rwkey):
        return hmac.new(str(rwkey),'#readonly', hashlib.sha256).digest()
    def _ro2loc(self,rokey):
        return hmac.new(str(rokey),'#location', hashlib.sha256).digest()
    def _ro2subrw(self,rokey,name):
        rval= hmac.new(str(rokey),name + self._secretsalt,hashlib.sha256).digest()
        return rval
    def _getCapRaw(self,romode,key):
        if romode:
            return _Key(None,key,self)
        else:
            return _Key(key,None,self)
    def _getCap(self,romode,asciikey):
        try:
            rawkey=base64.b32decode(asciikey + "====")
        except TypeError:
            return None
        return self._getCapRaw(romode,rawkey)
    def getCap(self,asciicap):
        if len(asciicap) != 55:
            return None
        if asciicap[2:3] != "-":
            return None
        asciicaptype=asciicap[:2]
        if asciicaptype == "rw":        
            return self._getCap(False,asciicap[3:])
        if asciicaptype == "ro":
            return self._getCap(True,asciicap[3:])
        return None
    def newRandomKey(self):
        return _Key(hashlib.sha256(str(random.SystemRandom().getrandbits(256))).digest(),None,self)

if __name__ == "__main__":
    core=Core("0c8b608f87be8dfa6ec8fad6d7d9252e90a1a081eeb88595cd7bc279d891a057")
    baserwkey=core.newRandomKey()
    print "Testing if ro->sub == sub->r:"
    rwsubnode=baserwkey.getSubNodeKey("foo")
    cap1=rwsubnode.getRoCap()
    robasekey=baserwkey.getRoKey()
    rosubkey=robasekey.getSubNodeKey("foo")
    cap2=rosubkey.getRoCap()
    if (cap1 != None) and (cap1 == cap2):
        print "   OK"
    else:
        print "   BROKEN: ",cap1,"!=",cap2
    print "Testing reconstruction of read only key from ascii cap:"
    reconstructedkey1=core.getCap(cap1)
    cap3=reconstructedkey1.getRoCap()
    if (cap1 != None) and (cap1 == cap3):
        print "   OK"
    else:
        print "   BROKEN: ",cap1,"!=",cap3
    print "Testing reconstruction of read-write key from ascii cap:"
    cap4=baserwkey.getRwCap()
    reconstructedkey2=core.getCap(cap4)
    cap5=reconstructedkey2.getRwCap()
    if (cap4 != None) and (cap4 == cap5):
        print "   OK"
    else:
        print "   BROKEN: ",cap4,"!=",cap5
    print "Testing if read only keys are deeply read only:"
    cap6=rosubkey.getRwCap()
    if cap6 == None:
        print "   OK"
    else:
        print "   BROKEN"
