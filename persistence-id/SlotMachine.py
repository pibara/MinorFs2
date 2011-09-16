#/usr/bin/python
import os
class SlotMachine:
    #Constructor
    def __init__(self):
        self.pidtohash = {}
        self.hashslots = {}
        self.activeprocesses = set()
        self.notifymap = {}
    #A new process we hadn't seen before needs to be registered.
    def _registerProcess(self,pid,slothash):
        self.pidtohash[str(pid)]=slothash
        if not self.hashslots.has_key(slothash) :
            self.hashslots[slothash] = [ str(pid) ]
        else:
            bestfit = len(self.hashslots[slothash])
            slot=0
            for candidate in self.hashslots[slothash]:
                if candidate == None:
                    if slot < bestfit:
                        bestfit=slot
                slot=slot+1
            if bestfit == len(self.hashslots[slothash]):
                self.hashslots[slothash].append(str(pid))
            else:
                self.hashslots[slothash][bestfit]=str(pid)
        self.activeprocesses.add(str(pid)) 
    #A process that died recently needs to be unregistered.
    def _unregisterProcess(self,pid):
        slothash = self.pidtohash[str(pid)]
        del self.pidtohash[str(pid)]
        slot=0
        actslots=0
        for candidate in self.hashslots[slothash]:
            if candidate == str(pid):
                self.hashslots[slothash][slot] = None
            if self.hashslots[slothash][slot] != None:
                actslots = actslots + 1
            slot = slot + 1
        if actslots == 0:
            del self.hashslots[slothash]
        self.activeprocesses.remove(str(pid))
        if self.notifymap.has_key(pid):
            self.notifymap[pid].invalidate(pid)
            del self.notifymap[pid]
    def requestPidNotification(self,pid,pclass):
        self.notifymap[pid] = pclass
    #When tick gets called, SlotMachine will check all running processes and unregister those needed.
    def tick(self):
        procdirs = os.listdir("/proc")
        realactiveprocesses = set()
        for procd in procdirs:
            realactiveprocesses.add(str(procd))
        diedprocesses = self.activeprocesses - realactiveprocesses
        for diedprocess in diedprocesses:
            self._unregisterProcess(diedprocess)
    #Request what slot this pid is running in.
    def __call__(self,pid,slothash):
        if not self.pidtohash.has_key(str(pid)):
            self._registerProcess(str(pid),slothash)
        slot=0
        for candidate in self.hashslots[slothash]:
            if candidate == str(pid):
                return slot
            slot = slot +1
        return None
    
if __name__ == "__main__":
    slotmachine = SlotMachine()
    slotmachine(1,"bogus1")
    slotmachine(1234,"bogus1")
    slotmachine(3,"bogus1")
    slotmachine(4,"bogus1") 
    slotmachine(5,"bogus1")
    slotmachine.tick() 
    slotmachine(6,"bogus1")       
    if slotmachine(6,"bogus1") == 5:
        print "ERROR: slotmachine still broken"
