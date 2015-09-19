MinorFs2
========

This repository is no longer active. MinorFS2 is being split up into severall new github projects

Filesystems:
* [RumpelstiltskinFS](https://github.com/pibara/RumpelstiltskinFS) : Formaly known as MinorFS::capfs, the core sparse-cap filesystem
* [MillersdoughterFS](https://github.com/pibara/MillersdaughterFS) : formaly known as MinorFS::tmpfs, the per-process $TMP tilesystem
* [StrawFS](https://github.com/pibara/StrawFS)           : formaly known as MinorFS2::granfs, multi granular per process private links to RumpelstiltskinFS
* [SpinningwheelFS](https://github.com/pibara/SpinningwheelFS)   : formaly known as MinorFs2::homefs, retrofitting file-system for allowing non-minorfs aware apps to use private storage.

Libraries:
* [Rumpeltree++](https://github.com/pibara/Rumpeltreepp) [**complete**]: C++ Library for managing DACs with sparse capabilities.
* [AESsparse++](https://github.com/pibara/AESsparsepp)       : C++ Library for sparse friendly AES encrypted files.
* [LibMinorFS2](https://github.com/pibara/LibMinorFs2)       : C/C++ Library for facilitating MinorFS2 aware applications to use attentuation, decomposition and delegation.
* [Sesame++](https://github.com/pibara/sesamepp) [**active**]          : C++ library for virtually keeping large amounts of files opened. For use in FUSE based loop-back type user space file-systems. 

Misc:

* Pseudodomde       : Daemon for looking up different attributes relevant to pseudo persistent process designations.
* Rumpeltools       : A set of leveraged authority user-level tools for MinorFs2. 

Please check the github pages if you want to keep up with MinorFS2 development.
I will be slowly moving all code in this repository to the individual github projects.


