#!/usr/bin/python
import re
import hashlib


class PersistenceClass:
    def __init__(self,profilename):
        self.profilename = profilename
        self.toolset = "any"
        self.callchains = {}
        self.useProfileInId = False
        self.useEnvInId = False
        self.useCmdLine = False
        self.granularity = 0
        self.regexes = []
        self.callchains = set()
        self.envfilters = set()
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
    def _granularity_command(self,arguments):
        for granchar in arguments:
            if granchar == "n": 
                self.granularity = 0
            elif granchar == "u":
                self.granularity = 1
            elif granchar == "t":
                self.granularity = 2
            elif granchar == "b":
                self.granularity = 3
            elif granchar == "c":
                self.granularity = 4
            elif granchar == "x":
                self.granularity = 5
            elif granchar == "w":
                self.granularity = 5
            elif granchar == "p":
                self.granularity = 7
            elif granchar == "P":
                self.useProfileInId = True
            elif granchar == "E":
                self.useEnvInId = True
            elif granchar == "C":
                self.useCmdLine = True    
        return
    def _toolset_command(self,arguments):
        self.toolset = arguments
        return
    def _cmdregex_command(self,arguments):
        try:
            newregex = re.compile(arguments)
            self.useEnvInId = True
            self.regexes.append(newregex)
        except:
            pass
        return
    def _callchain_command(self,arguments):
        self.callchains.add(arguments)
        return
    def _env_command(self,arguments):
        self.envfilters.add(arguments)
        return
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
        match = re.match(r"#minorfs\s+(\w+)\s+(\w.*),", line)
        if (match) :
            command = match.group(1)
            arguments = match.group(1)
            if command == "granularity":
                self._granularity_command(arguments)
            elif command == "toolset":
                self._toolset_command(arguments)
            elif command == "cmdregex":
                self._env_command(arguments)
            elif command == "callchain":
                self._callchain_command(arguments)
            elif command == "env":
                self._env_command(arguments)
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


