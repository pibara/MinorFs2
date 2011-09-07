#!/usr/bin/python
import re

class PersistenceClass:
    def __init__(self,profilename,defaultregex):
        self.defaultregex=defaultregex
        self.profilename = profilename
        self.toolset = "any"
        self.callchains = {}
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
    def _calculatePersistenceId(self,plaintext):
        #FIXME
        return "BOGUS"
    def parseLine(self,line):
        #FIXME
        return
    def __call__(self,pid,uid):
        if self.classno == 1 :
            return _calculatePersistenceId(str(uid))
        elif self.classno == 2 :
            return _calculatePersistenceId(str(uid) + self.toolset)
        elif self.classno == 3 :
            return _calculatePersistenceId(str(uid) + _getExePath(pid))
        elif self.classno == 4 :
            if self._callChainDefined(pid) :
                return _calculatePersistenceId(str(uid) + _getCallChainExePaths(pid))
            else:
                return None
        elif self.classno == 5 :
            return _calculatePersistenceId(str(uid) + _getExePath(pid) + _getMapsString(pid))
        elif self.classno == 6:
            return _calculatePersistenceId(str(uid) + _getCallChainExePaths(pid) + _getCallChainMapsString(pid))
        elif self.classno == 7:
            basehash = _calculatePersistenceId(str(uid) + _getCallChainExePaths(pid) + _getCallChainMapsString(pid))
            slot=self._getSlotNumber(pid,basehash)
            return _calculatePersistenceId(str(slot) + basehash)
        else:
            return None

