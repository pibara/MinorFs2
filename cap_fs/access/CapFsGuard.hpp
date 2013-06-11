//  Copyright (c) 2012, Rob J Meijer
//
//Permission is hereby granted, free of charge, to any person or organization
//obtaining a copy of the software and accompanying documentation covered by
//this license (the "Software") to use, reproduce, display, distribute,
//execute, and transmit the Software, and to prepare derivative works of the
//Software, and to permit third-parties to whom the Software is furnished to
//do so, all subject to the following:
//
//The copyright notices in the Software and this entire statement, including
//the above license grant, this restriction and the following disclaimer,
//must be included in all copies of the Software, in whole or in part, and
//all derivative works of the Software, unless such copies or derivative
//works are solely in the form of machine-executable object code generated by
//a source language processor.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
//SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
//FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//DEALINGS IN THE SOFTWARE.
#ifndef MINORFS_CAPFS_GUARD_HPP
#define MINORFS_CAPFS_GUARD_HPP
#include "../fs/CapFs.hpp"
#include "../fs/OpenBaseNode.hpp"
#include "NoAccessFs.hpp"
#include "AppArmorCheck.hpp"
#include <sys/types.h>
#include <grp.h>
#include <unistd.h>
#include "../../util/openfilecollection.hpp"
namespace capfs {
namespace access {
//This class guards access to the CapFs filesystem so that:
//* The root is always accessible.
//* CapFs is accessible to high level minorfs filesystems.
//* CapFs is accessible to AppArmor confined p[rocesses.
//* Creation of private root dirrectories is only accessible to high level minorfs filesystems.
class CapFsGuard {
    //The gid of high level minorfs filesystems.
    gid_t mMinorFsGid;
    //Helper object for checking if process runs AppArmor confined.
    AppArmorCheck mAppArmorConfined;
    //FileSystem object that gives access only to the top level directory. This is exposed to non confined processes.
    NoAccessFs mNoAccess;
    //The CapFs filesystem. This is exposed to confined processes and high level minorfs filesystems.
    capfs::fs::CapFs  mAccess;
    //Container for open files that is shared between access and no_access fs
    openfilecollectiontype mOpenFileCollection;
    //Static helper function for looking up mMinorFsGid
    static gid_t getgrnamgid();
  public:
    CapFsGuard(std::string acpath,std::string secretsalt);
    //Exposing the proper file-system abstraction node to processes.
    capfs::fs::BaseFs & operator()(gid_t gid,pid_t pid);
    //Looking up if a process is a high level filesystem that is allowed to create private root directories.
    bool operator()(gid_t gid);
};
}
}
#endif
