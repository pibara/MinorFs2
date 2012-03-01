#!/usr/bin/python
import hashlib
import hmac
import base64

class RoKey:
    def __init__(self,key,core):
        self.key=key
        self.core=core
        self.lkey=None
    def getRwKey(self):
        return None
    def getCryptoKey(self):
        return self.key
    def getRoKey(self):
        return base64.b32encode(self.getCryptoKey())[:52]
    def __getRawLocationKey(self):
        if self.lkey == None:
            self.lkey = self.core.ro2loc(self.key)
        return self.lkey 
    def getLocationKey(self):
        k = base64.b32encode(self.__getRawLocationKey())[:52]
        return "/"+k[:2]+"/"+k[2:4]+"/"+k[4:]
    def getSubNodeRwKey(self,name):
        return None
    def getSubNodeRoKey(self,name):
        return RoKey(self.core.rw2ro(self.core.ro2subrw(self.key,name)),self.core)

class RwKey:
    def __init__(self,key,core):
        self.key=key
        self.core=core
        self.rokey=None
        self.lkey=None
    def getRwKey(self):
        return base64.b32encode(self.key)[:52]
    def getCryptoKey(self):
        if self.rokey==None:
            self.rokey=self.core._rw2ro(self.key)
        return self.rokey
    def getRoKey(self):
        return base64.b32encode(self.getCryptoKey())[:52]
    def __getRawLocationKey(self):
        if self.lkey == None:
            self.lkey = self.core._ro2loc(self.getRoKey())
        return self.lkey
    def getLocationKey(self):
        k = base64.b32encode(self.__getRawLocationKey())[:52]
        return "/"+k[:2]+"/"+k[2:4]+"/"+k[4:]
    def getSubNodeRwKey(self,name):
        return RwKey(self.core._ro2subrw(self.getRoKey(),name),self.core)
    def getSubNodeRoKey(self,name):
        return RoKey(self.core._rw2ro(self.core.ro2subrw(self.getRoKey()),name),self.core)

class CapCore:
    def __init__(self,secretsalt):
        self._secretsalt=secretsalt
    def _rw2ro(self,rwkey):
        return hmac.new(rwkey,'#readonly', hashlib.sha256).digest()
    def _ro2loc(self,rokey):
        return hmac.new(rokey,'#location', hashlib.sha256).digest()
    def _ro2subrw(self,rokey,name):
        return hmac.new(rokey,name + self._secretsalt,hashlib.sha256).digest()
    def getCap(self,romode,key):
        if romode:
            return RoKey(key,self)
        else:
            return RwKey(key,self)


