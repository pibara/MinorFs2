#!/usr/bin/python
import hashlib
import hmac
import base64
import random
class _Key:
    """This class represents all the keys and sparse-caps that represent a node in CapFs.
Consult http://minorfs.polacanthus.net/wiki/Minorfs2_cap_fs for information on the underlying base design."""
    def __init__(self,rwkey,rokey,core):
        """Don't use this constructor, construct a _Key using an apropriate method from the Core class"""
        self.rwkey=rwkey
        self.rokey=rokey
        self.core=core
        self.lkey=None
    def getRwCap(self):
        """Fetch the read-write sparse-cap for this key. This method may return None if the key is read only"""
        if self.rwkey:
            return "rw-" + base64.b32encode(self.rwkey)[:52]
        else:
            return None
    def getRawCryptoKey(self):
        """Fetch the binary cryptography-key for encrypting/decrypting the raw file that this node is or would be stored in.""" 
        if self.rokey==None:
            self.rokey=self.core._rw2ro(self.rwkey)
        return self.rokey
    def getRoCap(self):
        """Fetch the read-only sparse-cap for this key."""
        return "ro-" + base64.b32encode(self.getRawCryptoKey())[:52]
    def __getRawLocationKey(self):
        if self.lkey == None:
            self.lkey = self.core._ro2loc(self.getRawCryptoKey())
        return self.lkey
    def getLocation(self):
        """Fetch the relative path where the raw file for this node is or would be stored."""
        k = base64.b32encode(self.__getRawLocationKey())[:52]
        return "/"+k[:2]+"/"+k[2:4]+"/"+k[4:]
    def getSubNodeKey(self,name):
        """This method returns a key object for a named sub-node.
The returned sub node key will have the same ro/rw property as its parent node."""  
        if self.rwkey:
            return _Key(self.core._ro2subrw(self.getRawCryptoKey(),name),None,self.core)
        else:
            return _Key(None,self.core._rw2ro(self.core._ro2subrw(self.getRawCryptoKey(),name)),self.core)
    def getRoKey(self):
        """This method returns a read-only attenuated version of this key.
If this key hapens already to represent a read-only attenuated node, the key itself is returned.""" 
        if self.rwkey==None:
            return self
        return _Key(0,self.getRawCryptoKey(),self.core)
    def isReadOnly(self):
        """This method returns a boolean indicating if this key represents a read only node."""
        if self.rwkey==None:
            return True
        return False        

class Core:
    """
The Core class is the interface for the CapCore module. 
It is the holder of the file-system secret-salt and 
can  be used to do two things:

* create a new random top-level node key
* reconstruct a key from an ascii sparse-cap

"""
    def __init__(self,secretsalt):
        """
The Core constructor takes a secret-salt as argument. 
Make sure that the salt used is:

* Stored persistently in a safe place that only the file-system can access.
* Composed with at least 256 bits of entrophy (the salt is hashed so only 256 bits at most are actualy used)
"""
        self._secretsalt=hashlib.sha256(secretsalt).hexdigest() 
    def _rw2ro(self,rwkey):
        return hmac.new(str(rwkey),'#readonly', hashlib.sha256).digest()
    def _ro2loc(self,rokey):
        return hmac.new(str(rokey),'#location', hashlib.sha256).digest()
    def _ro2subrw(self,rokey,name):
        rval= hmac.new(str(rokey),name + self._secretsalt,hashlib.sha256).digest()
        return rval
    def _getKeyRaw(self,romode,key):
        if romode:
            return _Key(None,key,self)
        else:
            return _Key(key,None,self)
    def _getKey(self,romode,asciikey):
        try:
            rawkey=base64.b32decode(asciikey + "====")
        except TypeError:
            return None
        return self._getKeyRaw(romode,rawkey)
    def getKey(self,asciicap):
        """This method reconstructs a key from a n ascii sparse-cap"""
        if len(asciicap) != 55:
            return None
        if asciicap[2:3] != "-":
            return None
        asciicaptype=asciicap[:2]
        if asciicaptype == "rw":        
            return self._getKey(False,asciicap[3:])
        if asciicaptype == "ro":
            return self._getKey(True,asciicap[3:])
        return None
    def newRandomKey(self):
        """This method should be used sparsinly. It generates a new top-level node for cap-fs"""
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
    reconstructedkey1=core.getKey(cap1)
    cap3=reconstructedkey1.getRoCap()
    if (cap1 != None) and (cap1 == cap3):
        print "   OK"
    else:
        print "   BROKEN: ",cap1,"!=",cap3
    print "Testing reconstruction of read-write key from ascii cap:"
    cap4=baserwkey.getRwCap()
    reconstructedkey2=core.getKey(cap4)
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
