#!/usr/bin/python
import re

class PersistenceClass:
    def __init__(self,defaultregex):
        self.classno = 0
        self.defaultregex = defaultregex
        self.regexes = set()
        self.envnames = set()
        self.toolset = ""
    def addEnvName(self,envname)
        self.envnames.add(envname)
    def addRegex(self,pattern):
        try :
            newre = re.compile(pattern)
            self.regexes.add(newre)
        except re.error:
            pass
    def updateClass(self,classno):
        if (classno > self.classno) and (classno in [1,2,3,4,5,6]):
            self.classno = classno
    def addToolSet(self,toolset):
        self.toolset.add(toolset)
    def _secure_hash(self,plaindata):
        #TODO: determine a secure hash and return it as a string.
    def _maphash(self,pid):
        #TODO: make a sorted list of all binaries in the map file and return its secure hash
    def _rmaphash(self,pid):
        rpid=pid
        plaindata=""
        while True:
            if plaindata:
                plaindata = plaindata + ":"
            plaindata = plaindata + self._maphash()
            if rpid == 1:
                return self._secure_hash(plaindata)
            rpid = self._parent(rpid)
    def _envhash(self,pid):
        #TODO: create a sorted list of all relevant envinronment variables and return its secure hash.
    def _cmdhash(self,pid):
        #TODO: fetch the relevant part of the commandline using the regexes and return the secure hash of all captured data.
    def _getAppArmorProfile(self,pid):
        #TODO: return the apparmor profile this pid is running under.
    def _class1(self,pid):
        main_id = self._class2(pid)
        if main_id == "":
            return ""
        slot = self._getslot(mainid,pid)
        plaintext = mainid + ":" + str(slot)
        return self._secure_hash(plaintext)
    def _class2(self,pid):
        prov = _getAppArmorProfile(pid):
        if prov != "" :
            mapshash = self._rmaphash(pid)
            envhash = self._envhash(pid)
            cmdhash = self._cmdhash(pid)
            return self._secure_hash(mapshash + ":" + envhash + ":" + cmdhash + ":" + prov)
        return ""
    def _class3(self,pid):
        prov = _getAppArmorProfile(pid):
        if prov != "" :
            mapshash = self._maphash(pid)
            envhash = self._envhash(pid)
            cmdhash = self._cmdhash(pid)
            return self._secure_hash(mapshash + ":" + envhash + ":" + cmdhash + ":" + prov)
        return ""
    def _class4(self,pid):
        prov = _getAppArmorProfile(pid):
        if prov != "" :
            return self._secure_hash(prov + ":" + self.toolset
        return ""        
    def _class5(self,pid):
        mapshash = self._maphash(pid)
        envhash = self._envhash(pid)
        cmdhash = self._cmdhash(pid)
        return self._secure_hash(mapshash + ":" + envhash + ":" + cmdhash)
    def __call__(self,pid):
        if self.classno ==0 :
            return "" #Class 0 meand no persistence-id
        elif self.classno == 1 :
            return self._class1(pid)
        elif self.classno == 2 :
            return self._class2(pid)
        elif self.classno == 3 :
            return self._class3(pid)
        elif self.classno == 4 :
            return self._class4(pid)
        elif self.classno == 5 :
            return self._class5(pid)
        else:
            return self.toolset #Class 6 should have a toolset name defined for it that gets used as persistence-id.

