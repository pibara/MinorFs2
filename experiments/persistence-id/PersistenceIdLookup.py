#!/usr/bin/python
import PersistenceClass
import SlotMachine
import re

class PersistenceIdLookup:
    def _lookupAppArmorProfile(self,pid):
        try:
            currentprofilefile=open("/proc/" + str(pid) + "/attr/current")
            for line in currentprofilefile:
                match = re.match(r'^(/\S+)',line)
                if match:
                    currentprofilefile.close()
                    return match.groups()[0]
            currentprofilefile.close()
            return None
        except:
            return None        
    def parseLine(self,profile,line):
        if not self.classes.has_key(profile):
            self.classes[profile] = PersistenceClass.PersistenceClass(profile,self.slotmachine)
        self.classes[profile].parseLine(line)
    def __init__(self):
        self.slotmachine = SlotMachine.SlotMachine()
        self.classes = {}
    def __call__(self,pid,uid):
        profile = self._lookupAppArmorProfile(pid)
        if profile != None:
            if self.classes.has_key(profile):
                return self.classes[profile](pid,uid)
        return None
    
if __name__ == "__main__":
    lookup =  PersistenceIdLookup()
    lookup.parseLine("/usr/lib/chromium-browser/chromium-browser","  #minorfs granularity       uP,")
    lookup.parseLine("/usr/lib/chromium-browser/chromium-browser","  #minorfs callchain         /sbin/init:/usr/lib/chromium-browser/chromium-browser,")
    print lookup(11533,1001)
    print lookup(11924,1001)
    print lookup(23963,1001) 
