#!/usr/bin/python
import os
import json
import sys
import CapCore
import DataDir
from CapFs import FileSystem
#Uncomment exactly one of the following CryptoModule lines to pick an appropriate crypto module.
#from NullCrypto import CryptoModule
from XorCrypto import CryptoModule

if __name__ == "__main__":
    configpath="/etc/minorfs/capfs.json"
    if not os.path.exists(configpath):
        print "ERROR: MinorFS2 was not installed properly,",configpath," is missing."
        exit(1)
    try:
        infile=open(configpath,"r")
    except IOError:
        print "ERROR: Unexpected IO error trying to open existing config " + configpath
        sys.exit(2)
    try:
        conf=json.load(infile)
    except ValueError:
        print "ERROR: The content of " + configpath + " is not valid json."
        sys.exit(3)
    infile.close()
    if not conf.has_key("salt"):
        print "ERROR: " + configpath + " does not contain a 'salt' section."
        sys.exit(4)
    salt=conf["salt"]
    if len(salt) < 52:
        print "ERROR: " + configpath + " value of 'salt' should be a 52 byte base32 string."
        sys.exit(5)
    if not conf.has_key("datadir"):
        print "ERROR: " + configpath + " does not contain a 'datadir' section."
        sys.exit(7)
    datadir=conf["datadir"]
    if not os.path.isdir(datadir):
        print "ERROR: " + configpath + " value of 'datadir' does not point to an existing directory."
        sys.exit(9)
    if not os.access(datadir, os.W_OK):
        print "ERROR: " + configpath + " value of 'datadir' points to a directory that is not writable to the current user."
        sys.exit(9)
    if not conf.has_key("mountpoint"):
        print "ERROR: " + configpath + " does not contain a 'mountpoint' section."
        sys.exit(10)
    mountpoint=str(conf["mountpoint"])
    if not os.path.isdir(mountpoint):
        print "ERROR: " + configpath + " value of 'mountpoint' does not point to an existing directory."
        sys.exit(9)
    core=CapCore.Core(salt)
    cryptomodule = CryptoModule()
    repository=DataDir.Repository(datadir,cryptomodule)
    capfs=FileSystem(repository,core,mountpoint)
    capfs.flags = 0
    capfs.multithreaded = 0
    capfs.fuse_args.add('allow_other')
    capfs.fuse_args.setmod('foreground')
    capfs.fuse_args.mountpoint = mountpoint
    capfs.main()
