Raw Cap Access Controll FileSystem
==================================

The Cap Access Controll FileSystem logically is part of the CapFs 
filesystem. It is used by CapFs to determine if a non MinorFs process 
is running AppArmor confined and thus is allowed access to CapFs using
sparse caps. This functionality is implemented as a seperate file-system 
for the following reasons:

* The sub-system needs to run as root in order to have access to
  the /proc/$PID/attr/current info needed to determine multi granular 
  process identity. Running the full CapFs as root would be a bad idea, 
  even when running AppArmore confined. For this reason we put this 
  functionality into a seperate sub-system.
* The original design of MinorFs2 aimed to provide functionality like 
  this  in a dbus service, while being a cleaner basic design, resulted
  in the fact that CapFs could be started only after dbus. Given that
  dbus starts reletively late, this design would thus have unduly 
  delayed the availability of CapFs.

The file-system is accessible only to the capfs user and implements a
simple lookup service using the stat filesystem interface.

Basically a lookup return an empty file stat if a process runs confined,
or return a no-entity error if it runs unconfined.

Basically a stat of:

$MP/$pid 

will thus either return an existing 0 byte long file, or an error indicating that
file does not exist. For example:

/minor/ac/1234 : File does not exist
/minor/ac/1235 : Empty file.
