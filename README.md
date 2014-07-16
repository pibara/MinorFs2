MinorFs2
========

This repository is no longer active. MinorFS2 is being split up into severall new github projects

Filesystems:
* RumpelstiltskinFS : Formaly known as MinorFS::capfs, the core sparse-cap filesystem
* MillersdoughterFS : formaly known as MinorFS::tmpfs, the per-process $TMP tilesystem
* StrawFS           : formaly known as MinorFS2::granfs, multi granular per process private links to RumpelstiltskinFS
* SpinningwheelFS   : formaly known as MinorFs2::homefs, retrofitting file-system for allowing non-minorfs aware apps to use private storage.

Libraries:
* Sesame++          : Library for keeping large sets of files virtualy opened.
* AESsparse++       : Library for sparse friendly AES encrypted files.
* LibMinorFS2       : Library for facilitating MinorFS2 aware applications to use attentuation, decomposition and delegation.

Misc:

* Rumpelbuntu       : Ubuntu server install CD tuned for use of MinorFS2 and more extensive use of AppArmor.

Please check the github pages if you want to keep up with MinorFS2 development.
I will be slowly moving all code in this repository to the individual github projects.


