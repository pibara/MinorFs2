Process Identity FileSystem
===========================

The Process Identity FileSystem logically is part of the MinorFs2 
HomeFs infrastructure. Its implemented as a seperate file-system for 
two reasons:

* The sub-system needs to run as root in order to have access to all
  the /proc/$PID info needed to determine multi granular process
  identity. Running the full HomeFs as root would be a bad idea, even
  when running AppArmore confined. For this reason we put this 
  functionality into a seperate sub-system.
* The original design of MinorFs2 aimed to provide this functionality 
  in a dbus service, while being a cleaner basic design, resulted
  in the fact that HomeFs could be started only after dbus. Given that
  dbus starts reletively late, this design would thus have unduly 
  delayed the availability of HomeFs.

The file-system is accessible only to the homfs user and implements a
simple lookup service using the file-system symbolic link interface.

Basically the symlinks returned are the relative paths (relative to 
the homefs private root) for a given PID at a given granularity level.

Basically:

$MP/$granularity/$pid -> $relpath

for example:

/minor/pid/tool/1234 -> 1002/tool/1fa6ed843c2cda3c8c66d564748653747bc88ebb
