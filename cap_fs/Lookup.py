#!/usr/bin/python
import hashlib
import base64
class Lookup:
    def __init__(self,rawstorage,salt):
        self.rawstorage = rawstorage
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
        b32path = "/" + b32key[:2] + "/" + b32key[2:4] + "/" + b32key[4:]
        basenode = self.rawstorage.pathToNode(b32path)        
        if basenode.exists():
            #The hypothesis checks out, we have a valid readonly cap as base cap.
            readonly = True
            b32basekey2 = basecap 
            if relpath == "":
                #If the relpath is empty, the basenode needs the binary version of basecap as a crypto key.
                try:
                    #Not sure why the base64 module requires these ==== characters to decode base32..
                    cryptokey = base64.b32decode(b32basekey2 + "====")
                    basenode.setCryptoKey(cryptokey)
                    basenode.setReadOnly(True)
                    return basenode
                except:
                    return None
        else:
            #The hypothesis doesn't check out, lets check if scenario 1 checks out (basecap being a rw cap)
            readonly = False
            b32basekey2 = b32key
            rawbasekey2 = rawhash
            b32basekey3 = base64.b32encode(str(hashlib.sha256(b32basekey2 + self.salt ).digest()))[:52]
            b32path = "/" + b32basekey3[:2] + "/" + b32basekey3[2:4] + "/" + b32basekey3[4:]
            basenode = self.rawstorage.pathToNode(b32path)
            if relpath == "":
                #If the relpath is empty, the basenode needs the binary version of basecap as a crypto key.
                #Note, we also return the basenode if it does not (yet) exist. Its up to our client code to 
                #determine what to do with non existing nodes.
                basenode.setCryptoKey(rawbasekey2)
                basenode.setReadOnly(False)
                return basenode
            if basenode.exists() == False:
                return None
        #Walk the relative path down to the designated sub path.
        for subnodename in relpath.split("/"):
            if b32basekey2 == "":
                #The parent node MUST exist.
                return None
            #Calculate the first key from the key2
            b32subkey1 = base64.b32encode(str(hashlib.sha256( b32basekey2 + self.salt + subnodename ).digest()))[:52]
            rawsubkey2 = str(hashlib.sha256( b32subkey1 + self.salt ).digest())
            b32subkey2 = base64.b32encode(rawsubkey2)[:52]
            b32subkey3 = base64.b32encode(str(hashlib.sha256( b32subkey2 + self.salt).digest()))[:52]
            b32path = "/" + b32subkey3[:2] + "/" + b32subkey3[2:4] + "/" + b32subkey3[4:]
            subnode = self.rawstorage.pathToNode(b32path)
            if subnode.exists() == False:
                b32basekey2 = ""
            else:
                b32basekey2 = b32subkey2
        #Note: we also return the subnode if it does not (yet) exist.
        subnode.setCryptoKey(rawsubkey2)
        subnode.setReadOnly(readonly)
        return subnode

if __name__ == "__main__":
    class FakeNode:
        def __init__(self,storepath):
            self.basepath=storepath
            self.key=""
            self.roflag=True
        def exists(self):
            #Make the base cap a r/w one by declaring the ro cap hypothesis invalid
            if self.basepath == "/PK/2I/FH4LHPZ47CRNHV4RTYWQMV6BNHE65WYXQ2YRGB6WSCTUEIPQ":
                return False
            #Declare the leave node non exixting.
            if self.basepath == "/HI/RH/SBCY3AEXIPNPELPZG6VRB24NXP7AQPLJR7AIXE5VJZQUNH5A":
                return False
            return True
        def setCryptoKey(self,key):
            self.key=key
        def setReadOnly(self,roflag):
            self.roflag=roflag
        def printit(self):
            print "    $BASEDIR" + self.basepath
            print "    exists =", self.exists()
            print "    readOnly=",self.roflag
            print "    cryptoKey=", base64.b32encode(self.key)
    class FakeStorage:
        def pathToNode(self,path):
            rval=FakeNode(path)
            return rval
    fakestore=FakeStorage()        
    lookup = Lookup(fakestore,"fakesalt")
    for fakekey in  ["KRRKFRQCU3V4VSVWOZ2WWVWH4756NN6CP7OORQLCYPI4XR7365EA","HRZWQYJSGU3CASCBKNECA33CNJSWG5BAIAQDA6BSG43GGYZTGA7A"]:
        for fakepath in ["","foo","foo/foo","foo/foo/foo","foo/foo/foo/bar.txt"]:
            node=lookup(fakekey,fakepath);
            print "$MOUNTPOINT/" + fakekey + "/"  + fakepath + ":"
            node.printit()
