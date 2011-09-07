#!/usr/bin/python
import re
import hashlib


class PersistenceClass:
    def __init__(self,profilename,defaultregex):
        self.defaultregex=defaultregex
        self.profilename = profilename
        self.toolset = "any"
        self.callchains = {}
        self.useProfileInId = False
        self.useEnvInId = False
        self.useCmdLine = True
    def _callChainDefined(self,pid):
        #FIXME
        return False
    def _getExePath(self,pid):
        #FIXME
        return "BOGUS"
    def _getCallChainExePaths(self,pid):
        #FIXME
        return "BOGUS"
    def _getMapsString(self,pid):
        #FIXME
        return "BOGUS"
    def _getCallChainMapsString(self,pid):
        #FIXME
        return "BOGUS"
    def _getSlotNumber(self,pid,basehash):
        #FIXME
        return 0
    def _getEnvString(self):
        return "BOGUS"
    def _getCmdCaptureText(self):
        return "BOGUS"
    def _calcDigestString(self,text):
        return hashlib.sha224(text).hexdigest()
    def _calculatePersistenceId(self,plaintext,pid):
        text=plaintext
        if self.useProfileInId:
            text = text + "-PROFILE:" + self.profilename
        if self.useEnvInId:
            text = text + "-ENV:" + self._getEnvString()
        if self.useCmdLine:
            text = text + "-CMD:" + self._getCmdCaptureText()
        return _calcDigestString(text)
    def parseLine(self,line):
        #FIXME
        return
    def __call__(self,pid,uid):
        if self.classno == 1 :
            return _calculatePersistenceId(str(uid),pid)
        elif self.classno == 2 :
            return _calculatePersistenceId(str(uid) + self.toolset,pid)
        elif self.classno == 3 :
            return _calculatePersistenceId(str(uid) + _getExePath(pid),pid)
        elif self.classno == 4 :
            if self._callChainDefined(pid) :
                return _calculatePersistenceId(str(uid) + _getCallChainExePaths(pid),pid)
            else:
                return None
        elif self.classno == 5 :
            return _calculatePersistenceId(str(uid) + _getExePath(pid) + _getMapsString(pid),pid)
        elif self.classno == 6:
            return _calculatePersistenceId(str(uid) + _getCallChainExePaths(pid) + _getCallChainMapsString(pid),pid)
        elif self.classno == 7:
            basehash = _calculatePersistenceId(str(uid) + _getCallChainExePaths(pid) + _getCallChainMapsString(pid),pid)
            slot=self._getSlotNumber(pid,basehash)
            return _calculatePersistenceId(str(slot) + basehash)
        else:
            return None


