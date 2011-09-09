#!/usr/bin/python
import re
import hashlib
import os

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
    def _getPPid(self,pid):
        try:
            statusfile=open("/proc/" + str(pid) + "/status")
            for line in statusfile:
                match = re.match(r'^PPid:\s+(\d+).*',line)
                if match:
                    statusfile.close()
                    return int(match.groups()[0])
            statusfile.close()
            return None
        except:
            return None
    def _getExePath(self,pid):
        try:
            rval=os.readlink("/proc/" + str(pid) + "/exe")
            return rval
        except:
            return None
    def _getCallChainExePaths(self,pid):
        tpid=pid
        s=""
        while True:
            epath = self._getExePath(tpid)
            if epath == None:
                return None
            if s:
                s = epath + ":" + s
            else:
                s = epath
            if tpid == 1:
                break
            tpid = self._getPPid(tpid)
            if tpid == None:
                return None
        return s
    def _callChainDefined(self,pid):
        if self._getCallChainExePaths(pid) in self.callchains:
            return True
        return False
    def _getMapsString(self,pid):
        mappedfiles = set()
        try:
            mapsfile = open("/proc/" + str(pid) + "/maps")
            for line in mapsfile:
                match = re.match(r'.*x\w\s+\S.*\s(\/\w.*)',line)
                if match:
                    mfile = match.groups()[0]
                    mappedfiles.add(match.groups()[0]) 
            mapsfile.close()
        except:
            return None            
        if len(mappedfiles) == 0:
            return None
        rval = repr(sorted(mappedfiles))
        return rval
    def _getCallChainMapsString(self,pid):
        tpid=pid
        s=""
        while True:
            mapsstring = self._getMapsString(tpid)
            if mapsstring == None:
                return None
            if s:
                s = s + ":" + mapsstring
            else :
                s= mapsstring
            if tpid == 1:
                break
            tpid = self._getPPid(tpid)
            if tpid == None:
                return None
        return s
    def _getSlotNumber(self,pid,basehash):
        #FIXME
        return 0
    def _getEnvString(self,pid):
        if self.useEnvInId == False:
            return "[NOENV]"
        try:
            envfile=open("/proc/" + str(pid) + "/environ")
            envdata=envfile.read(10000000)
            envfile.close()
            envtokens = envdata.split("\000")
            usealltokens = False
            if len(self.envfilters) == 0:
                usealltokens = True
            relevantenvtokens = set()
            for envtoken in envtokens:
                if usealltokens:
                    relevantenvtokens.add(envtoken)
                else:
                    keyval = envtoken.split("=")
                    if keyval[0] in self.envfilters:
                        relevantenvtokens.add(envtoken)
            rval = repr(sorted(relevantenvtokens))
            return rval
        except:
            return None 
    def _getCmdCaptureText(self,pid):
        if self.useCmdLine == False:
            return "[NOCMDLINE]"
        try:
            cmdlinefile = open("/proc/" + str(pid) + "/cmdline")
            cmdline = cmdlinefile.read(10000000)
            if len(self.regexes) == 0:
                return cmdline
            relevantparts = ""
            regexcount = 0
            for regex in self.regexes:
                match = regex.match(cmdline)
                if match:
                    if relevantparts :
                        relevantparts = relevantparts + ":re[" + str(regexcount) + "]"
                    else:
                        relevantparts = "re[" + str(regexcount) + "]"
                    for groupeddata in match.groups():
                        relevantparts = relevantparts + ":" + groupeddata
            if relevantparts:
                return relevantparts
            return "[NOREGEXMATCH]"
        except:
            return None 
    def _granularity_command(self,arguments):
        for granchar in arguments:
            if granchar == "n": #None 
                self.granularity = 0
            elif granchar == "u": #User granularity
                self.granularity = 1
            elif granchar == "t": #Toolset granularity
                self.granularity = 2
            elif granchar == "b": #Binary granularity
                self.granularity = 3
            elif granchar == "c": #Callchain granularity
                self.granularity = 4
            elif granchar == "x": #Executable granularity
                self.granularity = 5
            elif granchar == "w": #Worker granularity
                self.granularity = 5
            elif granchar == "p": #Persistent process granularity
                self.granularity = 7
            elif granchar == "P": #AppArmor profile sensitive
                self.useProfileInId = True
            elif granchar == "E": #Envinronment variable sensitive
                self.useEnvInId = True
            elif granchar == "C": #Commandline argument sensitive
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
            envstring = self._getEnvString(pid)
            if envstring == None:
                return None
            text = text + "-ENV:" + envstring
        if self.useCmdLine:
            cmdlinedata = self._getCmdCaptureText(pid)
            if cmdlinedata == None:
                return None
            text = text + "-CMD:" + cmdlinedata
        return self._calcDigestString(text)
    def parseLine(self,line):
        match = re.match(r".*#minorfs\s+(\w+)\s+([^\s]+)", line)
        if (match) :
            command = match.group(1)
            arguments = match.group(2)
            if command == "granularity":
                self._granularity_command(arguments)
            elif command == "toolset":
                self._toolset_command(arguments)
            elif command == "cmdregex":
                self._cmdregex_command(arguments)
            elif command == "callchain":
                self._callchain_command(arguments)
            elif command == "env":
                self._env_command(arguments)
        return
    def __call__(self,pid,uid):
        if self.granularity == 1 : #User granularity.
            return self._calculatePersistenceId(str(uid),pid)
        elif self.granularity == 2 : #Toolset granularity.
            return self._calculatePersistenceId(str(uid) + self.toolset,pid)
        elif self.granularity == 3 : #Binary granularity.
            exepath = self._getExePath(pid)
            if exepath == None:
                return None
            return self._calculatePersistenceId(str(uid) + exepath,pid)
        elif self.granularity == 4 : #Callchain granularity.
            if self._callChainDefined(pid) :
                cpexepath = self._getCallChainExePaths(pid)
                if cpexepath == None:
                    return None
                return self._calculatePersistenceId(str(uid) + cpexepath,pid)
            else:
                return None
        elif self.granularity == 5 : #XMap granularity.
            exepath = self._getExePath(pid)
            if exepath == None:
                return None 
            mstring = self._getMapsString(pid)
            if mstring == None:
                return None
            return self._calculatePersistenceId(str(uid) + exepath + mstring,pid)
        elif self.granularity == 6: #Worker granularity.
            callchainmapsstring = self._getCallChainMapsString(pid)
            if callchainmapsstring == None:
                return None
            callchainexepaths = self._getCallChainExePaths(pid)
            if callchainexepaths == None:
                return None
            return self._calculatePersistenceId(str(uid) + callchainexepaths + callchainmapsstring,pid)
        elif self.granularity == 7: #Pseudo persistent process granularity.
            callchainmapsstring = self._getCallChainMapsString(pid)
            if callchainmapsstring == None:
                return None
            callchainexepaths = self._getCallChainExePaths(pid)
            if callchainexepaths == None:
                return None
            basehash = self._calculatePersistenceId(str(uid) + callchainexepaths + callchainmapsstring,pid)
            if basehash == None:
                return None
            slot=self._getSlotNumber(pid,basehash)
            if slot == None:
                return None
            return self._calculatePersistenceId(str(slot) + basehash)
        else: 
            return None

if __name__ == "__main__":
   pclass = PersistenceClass("usr.bin.test")
   pclass.parseLine("   #minorfs granularity       wPEC")
   pclass.parseLine("   #minorfs callchain         /sbin/init:/usr/sbin/gdm-binary:/usr/lib/gdm/gdm-simple-slave:/usr/lib/gdm/gdm-session-worker:/usr/bin/gnome-session:/usr/bin/zeitgeist-datahub")
   pclass.parseLine("   #minorfs env               PATH")
   pclass.parseLine("   #minorfs env                 LANG")
   pclass.parseLine("   #minorfs cmdregex            .*-(.*)")
   pclass.parseLine("   #minorfs cmdregex            .*(\w-\w).*")
   print pclass(2177,1000)
   print pclass(2177,1001)
