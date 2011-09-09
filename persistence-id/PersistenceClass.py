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
    #Method for retreiving a proccess its parent process id from /proc/$PID/status
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
    #Method for retreiving the executable path from the /proc/$PID/exe symlink
    def _getExePath(self,pid):
        try:
            rval=os.readlink("/proc/" + str(pid) + "/exe")
            return rval
        except:
            return None
    #Method for retreiving the executable path from the /proc/$PID/exe symlink for the process and all its ancestors.
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
    #Check if this process has a matching callchain entry configured for it.
    def _callChainDefined(self,pid):
        if self._getCallChainExePaths(pid) in self.callchains:
            return True
        return False
    #Method for retreiving the relevant parts of the process environment variables from /proc/$PID/maps
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
    #Method for retreiving the relevant parts of the process environment variables from /proc/$PID/maps for the process and all its ancestors.
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
    #FIXME:TODO 
    def _getSlotNumber(self,pid,basehash):
        #FIXME
        return 0
    #Method for retreiving the relevant parts of the process environment variables from /proc/$PID/environ
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
    #Method for retreiving the relevant parts of the process commandline from /proc/$PID/cmdline
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
                regexcount = regexcount+1
            if relevantparts:
                return relevantparts
            return "[NOREGEXMATCH]"
        except:
            return None 
    #Function meant to process an 'granularity' configuration line.
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
    #Function meant to process an 'toolset' configuration line.
    def _toolset_command(self,arguments):
        self.toolset = arguments
        return
    #Function meant to process an 'cmdregex' configuration line.
    def _cmdregex_command(self,arguments):
        try:
            newregex = re.compile(arguments)
            self.useEnvInId = True
            self.regexes.append(newregex)
        except:
            pass
        return
    #Function meant to process an 'callchain' configuration line.
    def _callchain_command(self,arguments):
        self.callchains.add(arguments)
        return
    #Function meant to process an 'env' configuration line.
    def _env_command(self,arguments):
        self.envfilters.add(arguments)
        return
    #The basic hashing stuff for creating the persistence-Id.
    def _calcDigestString(self,text):
        print text
        return hashlib.sha224(text).hexdigest()
    #Helper method for __call__ that calculates the persistence Id based on the given granularity 'plaintext', and some non granularity dependant
    #configuration parameters.
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
    #The parseLine function is used to process the AppArmor config piggyback config lines for the AppArmor profile this object coresponds with.
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
    #Invocation will when given process id and user id return the persistence-ID (or None) in a way determined by previous invocations of parseLine.
    #Basicaly this call is one big switch case for the levels of granularity this object may be configured for.
    def __call__(self,pid,uid):
        if self.granularity == 1 : #User granularity.
            return self._calculatePersistenceId("user-granularity:uid=" + str(uid),pid)
        elif self.granularity == 2 : #Toolset granularity.
            return self._calculatePersistenceId("toolset-granularity:uid=" + str(uid) + ":toolset=" + self.toolset,pid)
        elif self.granularity == 3 : #Binary granularity.
            exepath = self._getExePath(pid)
            if exepath == None:
                return None
            return self._calculatePersistenceId("binary-granularity:uid=" + str(uid) + ":exepath=" + exepath,pid)
        elif self.granularity == 4 : #Callchain granularity.
            if self._callChainDefined(pid) :
                cpexepath = self._getCallChainExePaths(pid)
                if cpexepath == None:
                    return None
                return self._calculatePersistenceId("callchain-granularity:uid=" + str(uid) + ":ccexepath="  + cpexepath,pid)
            else:
                return None
        elif self.granularity == 5 : #XMap granularity.
            exepath = self._getExePath(pid)
            if exepath == None:
                return None 
            mstring = self._getMapsString(pid)
            if mstring == None:
                return None
            return self._calculatePersistenceId("xmap-granularity:uid=" + str(uid) + ":exepath=" +exepath + ":mapsstring=" + mstring,pid)
        elif self.granularity == 6: #Worker granularity.
            callchainmapsstring = self._getCallChainMapsString(pid)
            if callchainmapsstring == None:
                return None
            callchainexepaths = self._getCallChainExePaths(pid)
            if callchainexepaths == None:
                return None
            return self._calculatePersistenceId("worker-granularity:uid=" + str(uid) + ":ccexepath=" + callchainexepaths + ":ccmapsstring=" + callchainmapsstring,pid)
        elif self.granularity == 7: #Pseudo persistent process granularity.
            callchainmapsstring = self._getCallChainMapsString(pid)
            if callchainmapsstring == None:
                return None
            callchainexepaths = self._getCallChainExePaths(pid)
            if callchainexepaths == None:
                return None
            basehash = self._calculatePersistenceId("worker-granularity:uid=" + str(uid) + ":ccexepath=" + callchainexepaths + ":ccmapsstring=" + callchainmapsstring,pid)
            if basehash == None:
                return None
            slot=self._getSlotNumber(pid,basehash)
            if slot == None:
                return None
            return self._calculatePersistenceId("pseudo-persistent-process-granularity:slot=" +str(slot) + ":basehash="+ basehash)
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
