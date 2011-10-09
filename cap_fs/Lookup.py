#!/usr/bin/python
import hashlib
import base64
class Lookup:
    def __init__(self,rawstorage,salt):
        self.rawstorage
        self.salt=salt
    def __call__(self,basecap,relpath):
        #A valid basecap is 52 characters long.
        if len(basecap) != 52:
            return None
        #There are two scenario's:
        # 1) basecap could be basekey1 (a rw cap)
        # 2) basecap could be basekey2 (a ro key that is also the crypto key)
        #Calculate a new raw and base32 version of a  key that is either basekey2 (scenario 1) or basekey3 (scenario 2) 
        rawhash = str(hashlib.sha256(basecap + self.salt ).digest())
        b32key = base64.b32encode(rawhash)[:52]
        #First lets test the hypothesis this is scenario 2. If this hyothesis is true, firstrawhash        
        b32path = "/" + b32hash[:2] + "/" + b32hash[2:4] + "/" + b32hash[4:]
        basenode = self.rawstorage.pathToNode(b32path)        
        if basenode.exists():
            #The hypothesis checks out, we have a valid readonly cap as base cap.
            readonly = True
            b32basekey2 = basecap 
            if relpath == "":
                #If the relpath is empty, the basenode needs the binary version of basecap as a crypto key.
                try:
                    cryptokey = base64.b32decode(b32basekey2 + "====")
                    basenode.setCryptoKey(cryptokey)
                    basenode.setReadOnly(True)
                    return basenode
                except:
                    return None
        else:
            #The hypothesis doesn't check out, lets check if scenario 1 checks out.
            readonly = False
            b32basekey2 = b32key
            rawbasekey2 = rawhash
            b32basekey3 = base64.b32encode(str(hashlib.sha256(b32basekey2 + self.salt ).digest()))[:52]
            b32path = "/" + b32hash[:2] + "/" + b32hash[2:4] + "/" + b32hash[4:]
            basenode = self.rawstorage.pathToNode(b32path)
            if basenode.exists():
                #The alternative hypothesis checks out, we have a valid read/write cap as basecap.
                if relpath == "":
                    #If the relpath is empty, the basenode needs the binary version of basecap as a crypto key.
                    basenode.setCryptoKey(rawbasekey2)
                    basenode.setReadOnly(False)
                    return basenode
            else:
                return None
        #Walk the relative path down to the designated sub path.
        for subnodename in relpath.split("/"):
            b32subkey1 = base64.b32encode(str(hashlib.sha256( b32basekey2 + self.salt + subnodename ).digest()))[:52]
            rawsubkey2 = str(hashlib.sha256( b32subkey1 + self.salt ).digest())
            b32subkey2 = base64.b32encode(rawsubkey2)[:52]
            b32subkey3 = base64.b32encode(str(hashlib.sha256( b32subkey2 + self.salt).digest()))[:52]
            b32path = "/" + b32subkey3[:2] + "/" + b32subkey3[2:4] + "/" + b32subkey3[4:]
            subnode = self.rawstorage.pathToNode(b32path)
            if subnode.exists() == False:
                return None
        subnode.setCryptoKey(rawsubkey2)
        subnode.setReadOnly(readonly)
        return subnode


         
        
