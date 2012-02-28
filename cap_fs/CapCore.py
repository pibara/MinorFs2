#!/usr/bin/python
import hashlib
import base64

class RoKey:
    def __init__(self,key,core):
        self.key=key
        self.core=core
        self.lkey=None
    def getRwKey():
        return None
    def getRoKey():
        return self.key
    def getLocationKey():
        if self.lkey == None:
            self.lkey = self.core.ro2loc(self.key)
        return self.lkey 
    def getSubNodeRwKey(name):
        return None
    def getSubNodeRoKey(name):
        return RoKey(self.core.rw2ro(self.core.ro2subrw(self.key,name)),self.core)

class RwKey:
    def __init__(self,key,core):
        self.key=key
        self.core=core
        self.rokey=None
        self.lkey=None
    def getRwKey():
        return self.key
    def getRoKey():
        if self.rokey==None:
            self.rokey=self.core.rw2ro(self.key)
        return self.rokey
    def getLocationKey():
        if self.lkey == None:
            self.lkey = self.core.ro2loc(self.getRoKey())
        return self.lkey
    def getSubNodeRwKey(name):
        return RwKey(self.core.ro2subrw(self.getRoKey(),name),self.core)
    def getSubNodeRoKey(name):
        return RoKey(self.core.rw2ro(self.core.ro2subrw(self.getRoKey()),name),self.core)

class RoCore:
    def __init__(self,secretsalt):
        self._secretsalt=secretsalt
    def rw2ro(self,rwkey):
        pass
    def ro2loc(self,rokey):
        pass
    def ro2subrw(self,rokey,name):
        pass
    def getCap(self,romode,key):
        if romode:
            return RoKey(key,self)
        else:
            return RwKey(key,self)

